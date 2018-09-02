#include <string.h>

#include "py/obj.h"
#include "py/compile.h"
#include "py/gc.h"
#include "py/lexer.h"
#include "py/parse.h"
#include "py/repl.h"
#include "py/runtime.h"
#include "py/stackctrl.h"
#include "lib/utils/pyexec.h"
#include "gccollect.h"
#include "machine.h"
#include "syscall.h"


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
        mp_obj_print_exception(&mp_plat_print, (mp_obj_t) nlr.ret_val);
    }
}

int main(int argc, char **argv) {
    nlr_buf_t nlr;
    int code;

    if (nlr_push(&nlr) == 0) {
        mp_stack_set_top((uint8_t *) &_estack);
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

        code = pyexec_frozen_module("_boot.py");
        if (code != 1) {
            // error or SystemExit
            return 1;
        }

        code = pyexec_frozen_module("_bios.py");
        if (code != 1) {
            // error or SystemExit
            return 1;
        } else {
            // done, give interpreter for now
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
        }
    } else {
        // uncaught exception
        mp_obj_print_exception(&mp_plat_print, (mp_obj_t) nlr.ret_val);
        mp_deinit();
    }

    return 0;
}
