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
#include "openpie_mcu.h"
#include "gccollect.h"
#include <math.h>

#define OPENPIE_DEBUG(s) (mp_hal_stdout_tx_strn((s), strlen((s))));

mp_uint_t gc_helper_get_regs_and_sp(mp_uint_t *regs);

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
    while (true) {
        mp_stack_set_top((uint8_t*)&_estack);
        mp_stack_set_limit(&_estack - &_ebss - 512);

        #if MICROPY_ENABLE_PYSTACK
        static mp_obj_t pystack[4096];
        mp_pystack_init(&pystack, &pystack[MP_ARRAY_SIZE(pystack)]);
        #endif

        gc_init(&_ram_start, &_ram_end);
        mp_init();
        mp_obj_list_init(mp_sys_path, 0);
        mp_obj_list_append(mp_sys_path, MP_OBJ_NEW_QSTR(MP_QSTR_));
        mp_obj_list_init(mp_sys_argv, 0);

        do_str("from main import *", MP_PARSE_FILE_INPUT);

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

mp_import_stat_t mp_import_stat(const char *path) {
    return mp_vfs_import_stat(path);
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

void Reset_Handler(void) __attribute__((naked));
void Reset_Handler(void) {
    // set stack pointer
    __asm volatile ("ldr r0, =_svec");
    __asm volatile ("ldr r1, [r0]");
    __asm volatile ("mov sp, r1");
    // copy .data section from flash to RAM
    for (uint32_t *src = &_sidata, *dest = &_sdata; dest < &_edata;) {
        *dest++ = *src++;
    }
    // zero out .bss section
    for (uint32_t *dest = &_sbss; dest < &_ebss;) {
        *dest++ = 0;
    }

    OPENPIE_CONTROLLER->PENDING = (uint32_t) &MP_STATE_VM(mp_pending_exception);
    OPENPIE_CONTROLLER->EXCEPTION = (uint32_t) &MP_STATE_VM(mp_kbd_exception);
    OPENPIE_CONTROLLER->INTR_CHAR = (uint32_t) &mp_interrupt_char;

    // jump to board initialisation
    void _start(void);
    _start();
}

const uint32_t startup_vector[] __attribute__((section(".startup"))) = {
    (uint32_t)&_estack,
    (uint32_t)&Reset_Handler,
    // TODO: add custom handler for detect failure (or just turn off computer?)
};

void _start(void) {
    // when we get here: stack is initialised, bss is clear, data is copied

    // now that we have a basic system up and running we can call main
    main(0, NULL);

    // we must not return
    for (;;) {
    }
}
