#include <string.h>
#include <stdarg.h>

#include "py/obj.h"
#include "py/compile.h"
#include "py/gc.h"
#include "py/lexer.h"
#include "py/parse.h"
#include "py/repl.h"
#include "py/runtime.h"
#include "py/stackctrl.h"
#include "py/frozenmod.h"
#include "lib/utils/pyexec.h"
#include "gccollect.h"
#include "machine.h"
#include "syscall.h"

void debug_printer(void *self, const char *buf, size_t len) {
    __syscall2(SYS_DEBUG, (int) buf, (int) len);
}

const struct _mp_print_t debug_print = {NULL, debug_printer};

void mp_init_port() {
    MP_STATE_PORT(object_hook_obj) = mp_const_none;
    MP_STATE_PORT(signal_hook_obj) = mp_const_none;
    MP_STATE_PORT(stdin_hook_obj) = mp_const_none;
    MP_STATE_PORT(stdout_hook_obj) = mp_const_none;
    MP_STATE_PORT(stderr_hook_obj) = mp_const_none;
}

void do_str(const char *src, mp_parse_input_kind_t input_kind) { // MP_PARSE_SINGLE_INPUT
    nlr_buf_t nlr;
    if (nlr_push(&nlr) == 0) {
        mp_lexer_t *lex = mp_lexer_new_from_str_len(MP_QSTR__lt_stdin_gt_, src, strlen(src), 0);
        mp_parse_tree_t parse_tree = mp_parse(lex, input_kind);
        mp_obj_t module_fun = mp_compile(&parse_tree, MP_QSTR__lt_stdin_gt_, MP_EMIT_OPT_NONE, false);
        mp_call_function_0(module_fun);
        nlr_pop();
    } else {
        // uncaught exception
        mp_obj_print_exception(&mp_plat_print, (mp_obj_t) nlr.ret_val);
    }
}

void do_frozen(const char *name) {
    void *frozen_data;
    int frozen_type = mp_find_frozen_module(name, strlen(name), &frozen_data);
    if (frozen_type != MP_FROZEN_MPY)
        __fatal_error("frozen_type != MP_FROZEN_MPY");

    mp_obj_t module_fun = mp_make_function_from_raw_code(frozen_data, MP_OBJ_NULL, MP_OBJ_NULL);
    mp_call_function_0(module_fun);
}

int main(int argc, char **argv) {
    nlr_buf_t nlr;

    if (nlr_push(&nlr) == 0) {
        mp_stack_set_top((uint8_t * ) & _estack);
        mp_stack_set_limit(&_estack - &_ebss - 512);

#if MICROPY_ENABLE_PYSTACK
        static mp_obj_t pystack[10000];
        mp_pystack_init(&pystack, &pystack[MP_ARRAY_SIZE(pystack)]);
#endif

        size_t ram_size = (size_t) __syscall0(SYS_INFO_RAM_SIZE);
        gc_init(&_ram_start, ((void *)&_ram_start) + ram_size);

        mp_init();
        mp_init_port();
        mp_obj_list_init(mp_sys_path, 0);
        mp_obj_list_append(mp_sys_path, MP_OBJ_NEW_QSTR(MP_QSTR_));
        mp_obj_list_init(mp_sys_argv, 0);

        do_frozen("bios.py");

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

        nlr_pop();
    } else {
        nlr_buf_t nlr2;
        if (nlr_push(&nlr2) == 0) {
            vstr_t vstr;
            mp_print_t print;
            vstr_init_print(&vstr, 256, &print);

            mp_obj_print_exception(&print, (mp_obj_t) nlr.ret_val);
            char *message = vstr_null_terminated_str(&vstr);
            __fatal_error(message);
            nlr_pop();
        } else {
            vstr_t vstr;
            mp_print_t print;
            vstr_init_print(&vstr, 256, &print);

            mp_obj_print_helper(&print, (mp_obj_t) nlr.ret_val, PRINT_EXC);
            // char *message = vstr_null_terminated_str(&vstr);
            vstr.buf[vstr.len] = '\0';
            char *message = vstr.buf;
            __fatal_error(message);
        }

        mp_deinit();
    }

    return 0;
}
