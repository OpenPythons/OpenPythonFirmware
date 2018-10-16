#include "py/obj.h"
#include "py/objstr.h"
#include "py/mphal.h"
#include "py/runtime.h"
#include "py/lexer.h"
#include "py/compile.h"
#include "py/parse.h"
#include "py/repl.h"
#include "lib/utils/pyexec.h"
#include "lib/mp-readline/readline.h"
#include "genhdr/mpversion.h"


#define EXEC_FLAG_PRINT_EOF (1)
#define EXEC_FLAG_ALLOW_DEBUGGING (2)
#define EXEC_FLAG_IS_REPL (4)
#define EXEC_FLAG_SOURCE_IS_RAW_CODE (8)
#define EXEC_FLAG_SOURCE_IS_VSTR (16)
#define EXEC_FLAG_SOURCE_IS_FILENAME (32)

STATIC mp_obj_t ucode_repl_input() {
    vstr_t line;
    vstr_init(&line, 32);

    int ret = readline(&line, ">>> ");
    while (mp_repl_continue_with_input(vstr_null_terminated_str(&line))) {
        vstr_add_byte(&line, '\n');
        ret = readline(&line, "... ");
        if (ret == CHAR_CTRL_C) {
            mp_hal_stdout_tx_str("\r\n");
            return mp_const_none;
        } else if (ret == CHAR_CTRL_D) {
            break;
        }
    }

    return mp_obj_new_str_from_vstr(&mp_type_str, &line);
}

MP_DEFINE_CONST_FUN_OBJ_0(ucode_repl_input_obj, ucode_repl_input);


STATIC mp_obj_t ucode_repl_compile(mp_obj_t code, mp_obj_t globals_obj) {
    mp_obj_dict_t *volatile old_globals = mp_globals_get();
    if (!MP_OBJ_IS_TYPE(globals_obj, &mp_type_dict))
        mp_raise_ValueError("globals must be dict");

    nlr_buf_t nlr;
    if (nlr_push(&nlr) == 0) {
        mp_globals_set(globals_obj);

        size_t len = 0;
        const char *str = mp_obj_str_get_data(code, &len);
        mp_lexer_t *lex = mp_lexer_new_from_str_len(MP_QSTR__lt_stdin_gt_, str, len, 0);

        mp_parse_tree_t parse_tree = mp_parse(lex, MP_PARSE_SINGLE_INPUT);
        mp_obj_t func = mp_compile(&parse_tree, lex->source_name, MP_EMIT_OPT_NONE, EXEC_FLAG_IS_REPL);

        mp_globals_set(old_globals);
        nlr_pop();

        return func;
    } else {
        mp_globals_set(old_globals);
        nlr_raise(nlr.ret_val);
    }
}

MP_DEFINE_CONST_FUN_OBJ_2(ucode_repl_compile_obj, ucode_repl_compile);


STATIC mp_obj_t ucode_repl_call(mp_obj_t func, mp_obj_t locals_obj) {
    mp_obj_dict_t *volatile old_locals = mp_locals_get();
    if (!MP_OBJ_IS_TYPE(locals_obj, &mp_type_dict))
        mp_raise_ValueError("locals must be dict");

    nlr_buf_t nlr;
    if (nlr_push(&nlr) == 0) {
        mp_locals_set(locals_obj);

        mp_obj_t result = mp_call_function_0(func);

        mp_locals_set(old_locals);
        nlr_pop();

        return result;
    } else {
        mp_locals_set(old_locals);
        nlr_raise(nlr.ret_val);
    }
}

MP_DEFINE_CONST_FUN_OBJ_2(ucode_repl_call_obj, ucode_repl_call);


STATIC const MP_DEFINE_STR_OBJ(ucode_MICROPY_GIT_TAG_obj, MICROPY_GIT_TAG);
STATIC const MP_DEFINE_STR_OBJ(ucode_MICROPY_BUILD_DATE_obj, MICROPY_BUILD_DATE);
STATIC const MP_DEFINE_STR_OBJ(ucode_MICROPY_HW_BOARD_NAME_obj, MICROPY_HW_BOARD_NAME);
STATIC const MP_DEFINE_STR_OBJ(ucode_MICROPY_HW_MCU_NAME_obj, MICROPY_HW_MCU_NAME);


STATIC const mp_rom_map_elem_t ucode_module_globals_table[] = {
        {MP_ROM_QSTR(MP_QSTR___name__), MP_ROM_QSTR(MP_QSTR_ucode)},

        {MP_ROM_QSTR(MP_QSTR_repl_input),               MP_ROM_PTR(&ucode_repl_input_obj)},
        {MP_ROM_QSTR(MP_QSTR_repl_compile),             MP_ROM_PTR(&ucode_repl_compile_obj)},
        {MP_ROM_QSTR(MP_QSTR_repl_call),                MP_ROM_PTR(&ucode_repl_call_obj)},

        {MP_ROM_QSTR(MP_QSTR_MICROPY_GIT_TAG),          MP_ROM_PTR(&ucode_MICROPY_GIT_TAG_obj)},
        {MP_ROM_QSTR(MP_QSTR_MICROPY_BUILD_DATE),       MP_ROM_PTR(&ucode_MICROPY_BUILD_DATE_obj)},
        {MP_ROM_QSTR(MP_QSTR_MICROPY_HW_BOARD_NAME),    MP_ROM_PTR(&ucode_MICROPY_HW_BOARD_NAME_obj)},
        {MP_ROM_QSTR(MP_QSTR_MICROPY_HW_MCU_NAME),      MP_ROM_PTR(&ucode_MICROPY_HW_MCU_NAME_obj)},
};

STATIC MP_DEFINE_CONST_DICT(ucode_module_globals, ucode_module_globals_table);

const mp_obj_module_t mp_module_ucode = {
        .base = {&mp_type_module},
        .globals = (mp_obj_dict_t *) &ucode_module_globals,
};
