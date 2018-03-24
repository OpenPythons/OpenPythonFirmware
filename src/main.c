#include <stdint.h>
#include <stdio.h>
#include <string.h>

#include "py/nlr.h"
#include "py/compile.h"
#include "py/runtime.h"
#include "py/stackctrl.h"
#include "py/repl.h"
#include "py/gc.h"
#include "py/mperrno.h"
#include "py/mphal.h"
#include "py/lexer.h"
#include "extmod/vfs.h"
#include "lib/utils/interrupt_char.h"
#include "lib/utils/pyexec.h"
#include "lib/memzip/memzip.h"
#include "umport_mcu.h"
#include <math.h>

#define UMPORT_DEBUG(s) (mp_hal_stdout_tx_strn((s), strlen((s))));

void do_str(const char *src, mp_parse_input_kind_t input_kind) {
    nlr_buf_t nlr;
    if (nlr_push(&nlr) == 0) {
        mp_lexer_t *lex = mp_lexer_new_from_str_len(MP_QSTR__lt_stdin_gt_, src, strlen(src), 0);
        mp_parse_tree_t parse_tree = mp_parse(lex, input_kind);
        mp_obj_t module_fun = mp_compile(&parse_tree, MP_QSTR__lt_stdin_gt_, MP_EMIT_OPT_NONE, false);
        mp_call_function_0(module_fun);
        nlr_pop();
    } else {
        // uncaught exception
        mp_obj_print_exception(&mp_plat_print, (mp_obj_t)nlr.ret_val);
    }
}

int main(int argc, char **argv) {
    extern uint32_t _ebss;
    extern uint32_t _sdata;

    // Stack limit should be less than real stack size, so we have a chance
    // to recover from limit hit.  (Limit is measured in bytes.)
    mp_stack_set_top((uint8_t*)&_sdata + UMPORT_CONTROLLER->RAM_SIZE);
    mp_stack_set_limit(UMPORT_CONTROLLER->STACK_SIZE - 1024);

    while (true) {
        gc_init(&_ebss, (uint8_t*)&_sdata + UMPORT_CONTROLLER->RAM_SIZE - UMPORT_CONTROLLER->STACK_SIZE);
        mp_init();
        mp_obj_list_init(mp_sys_path, 0);
        mp_obj_list_append(mp_sys_path, MP_OBJ_NEW_QSTR(MP_QSTR_));
        mp_obj_list_init(mp_sys_argv, 0);

        do_str("for i in range(1):pass", MP_PARSE_FILE_INPUT);

        for (;;) {
            if (pyexec_mode_kind == PYEXEC_MODE_RAW_REPL) {
                if (pyexec_raw_repl() != 0) {
                    break;
                }
            } else {
                if (pyexec_friendly_repl() != 0) {
                    break;
                }
            }
        }
        mp_deinit();
        printf("PYB: soft reboot\n");
    }
    return 0;
}

void gc_collect(void) {
    // WARNING: This gc_collect implementation doesn't try to get root
    // pointers from CPU registers, and thus may function incorrectly.
    void *dummy;
    gc_collect_start();
    gc_collect_root(&dummy, ((mp_uint_t)MP_STATE_THREAD(stack_top) - (mp_uint_t)&dummy) / sizeof(mp_uint_t));
    gc_collect_end();
}


typedef struct _mp_reader_memzip_t {
    byte *data;
    size_t len;
    size_t pos;
} mp_reader_memzip_t;

STATIC mp_uint_t mp_reader_memzip_readbyte(void *data) {
    mp_reader_memzip_t *reader = (mp_reader_memzip_t*)data;
    if (reader->pos >= reader->len) {
        return MP_READER_EOF;
    }
    return reader->data[reader->pos++];
}

STATIC void mp_reader_memzip_close(void *data) {
    mp_reader_memzip_t *reader = (mp_reader_memzip_t*)data;
    m_del_obj(mp_reader_memzip_t, reader);
}

void mp_reader_new_file(mp_reader_t *reader, const char *filename) {
    mp_reader_memzip_t *rf = m_new_obj(mp_reader_memzip_t);
    void *data;
    size_t len;

    if (memzip_locate(filename, &data, &len) != MZ_OK) {
        mp_raise_OSError(MP_ENOENT);
    }

    rf->data = data;
    rf->len = len;
    rf->pos = 0;
    reader->data = rf;
    reader->readbyte = mp_reader_memzip_readbyte;
    reader->close = mp_reader_memzip_close;
}


mp_lexer_t *mp_lexer_new_from_file(const char *filename) {
    // TODO: check vfs file

    void *data;
    size_t len;

    if (memzip_locate(filename, &data, &len) != MZ_OK) {
        mp_raise_OSError(MP_ENOENT);
    }

    return mp_lexer_new_from_str_len(qstr_from_str(filename), (const char *)data, (mp_uint_t)len, 0);
}

mp_import_stat_t mp_memzip_import_stat(const char *path) {
    MEMZIP_FILE_INFO info;

    if (memzip_stat(path, &info) != MZ_OK) {
        return MP_IMPORT_STAT_NO_EXIST;
    }

    if (info.is_dir) {
        return MP_IMPORT_STAT_DIR;
    }
    return MP_IMPORT_STAT_FILE;
}

mp_import_stat_t mp_import_stat(const char *path) {
    mp_import_stat_t result = mp_vfs_import_stat(path);

    if (result == MP_IMPORT_STAT_NO_EXIST) {
        result = mp_memzip_import_stat(path);
    }

    return result;
}

void nlr_jump_fail(void *val) {
    while (1);
}

void NORETURN __fatal_error(const char *msg) {
    while (1);
}

#ifndef NDEBUG
void MP_WEAK __assert_func(const char *file, int line, const char *func, const char *expr) {
    printf("Assertion '%s' failed, at file %s:%d\n", expr, file, line);
    __fatal_error("Assertion failed");
}
#endif

// this is a minimal IRQ and reset framework for any Cortex-M CPU

extern uint32_t _sidata, _sdata, _edata, _sbss, _ebss;

void Reset_Handler(void) __attribute__((naked));
void Reset_Handler(void) {
    // set stack pointer
    __asm volatile ("ldr r0, =0x08000000");
    __asm volatile ("ldr sp, [r0]");
    // copy .data section from flash to RAM
    for (uint32_t *src = &_sidata, *dest = &_sdata; dest < &_edata;) {
        *dest++ = *src++;
    }
    // zero out .bss section
    for (uint32_t *dest = &_sbss; dest < &_ebss;) {
        *dest++ = 0;
    }

    UMPORT_CONTROLLER->PENDING = (uint32_t) &MP_STATE_VM(mp_pending_exception);
    UMPORT_CONTROLLER->EXCEPTION = (uint32_t) &MP_STATE_VM(mp_kbd_exception);
    UMPORT_CONTROLLER->INTR_CHAR = (uint32_t) &mp_interrupt_char;

    // jump to board initialisation
    void _start(void);
    _start();
}

void Default_Handler(void) {
    for (;;) {
    }
}

const uint32_t isr_vector[] __attribute__((section(".isr_vector"))) = {
    0, // will be set dynamically
    (uint32_t)&Reset_Handler,
    (uint32_t)&Default_Handler, // NMI_Handler
    (uint32_t)&Default_Handler, // HardFault_Handler
    (uint32_t)&Default_Handler, // MemManage_Handler
    (uint32_t)&Default_Handler, // BusFault_Handler
    (uint32_t)&Default_Handler, // UsageFault_Handler
    0,
    0,
    0,
    0,
    (uint32_t)&Default_Handler, // SVC_Handler
    (uint32_t)&Default_Handler, // DebugMon_Handler
    0,
    (uint32_t)&Default_Handler, // PendSV_Handler
    (uint32_t)&Default_Handler, // SysTick_Handler
};

void _start(void) {
    // when we get here: stack is initialised, bss is clear, data is copied

    // SCB->CCR: enable 8-byte stack alignment for IRQ handlers, in accord with EABI
    //*((volatile uint32_t*)0xe000ed14) |= 1 << 9;

    // initialise the cpu and peripherals

    // now that we have a basic system up and running we can call main
    main(0, NULL);

    // we must not return
    for (;;) {
    }
}
