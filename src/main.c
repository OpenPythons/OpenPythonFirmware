#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <math.h>

#include "py/nlr.h"
#include "py/compile.h"
#include "py/runtime.h"
#include "py/stackctrl.h"
#include "py/repl.h"
#include "py/gc.h"
#include "py/mperrno.h"
#include "py/mphal.h"
#include "py/lexer.h"
#include "py/objstr.h"
#include "lib/utils/interrupt_char.h"
#include "lib/utils/pyexec.h"
#include "extmod/vfs.h"
#include "gccollect.h"
#include "openpie_mcu.h"
#include "syscall_arch.h"

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
    nlr_buf_t nlr;
    if (nlr_push(&nlr) == 0) {
        mp_stack_set_top((uint8_t*)&_estack);
        mp_stack_set_limit(&_estack - &_ebss - 512);

        #if MICROPY_ENABLE_PYSTACK
        static mp_obj_t pystack[10000];
        mp_pystack_init(&pystack, &pystack[MP_ARRAY_SIZE(pystack)]);
        #endif

        gc_init(&_ram_start, &_ram_end);
        mp_init();
        mp_obj_list_init(mp_sys_path, 0);
        mp_obj_list_append(mp_sys_path, MP_OBJ_NEW_QSTR(MP_QSTR_));
        mp_obj_list_init(mp_sys_argv, 0);
        pyexec_frozen_module("_boot.py");
        pyexec_file("boot.py");

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
    } else {
        // uncaught exception
        mp_obj_print_exception(&mp_plat_print, (mp_obj_t)nlr.ret_val);
        mp_deinit();
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

void Signal_Handler(const byte *buf, size_t len) {
    SVC_FUNCTION_START;
    mp_obj_t signal_buf = mp_obj_new_str_copy(&mp_type_str, buf, len);
    mp_obj_t handler = mp_obj_dict_get(
        MP_OBJ_FROM_PTR(&MP_STATE_VM(dict_main)),
        MP_OBJ_NEW_QSTR(MP_QSTR_signal_handler)
    );

    mp_sched_schedule(handler, signal_buf);
    SVC_CALL(0);
}

void Print_Handler() {
    SVC_FUNCTION_START;
    mp_obj_t handler = mp_obj_dict_get(
        MP_OBJ_FROM_PTR(&MP_STATE_VM(dict_main)),
        MP_OBJ_NEW_QSTR(MP_QSTR_print_handler)
    );

    mp_sched_schedule(handler, mp_const_none);
    __syscall0(0);
}

const uint32_t startup_vector[] __attribute__((section(".startup"))) = {
    (uint32_t)&_estack,
    (uint32_t)&Reset_Handler,
    (uint32_t)&Signal_Handler,
    (uint32_t)&Print_Handler,
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
