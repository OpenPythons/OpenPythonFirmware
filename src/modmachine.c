#include <stdio.h>

#include "py/lexer.h"
#include "py/compile.h"
#include "py/parse.h"
#include "py/repl.h"
#include "lib/utils/pyexec.h"
#include "lib/mp-readline/readline.h"

#include "py/obj.h"
#include "py/objstr.h"
#include "py/runtime.h"
#include "openpie_vfs.h"
#include "syscall.h"
#include "msgpack.h"
#include "py/mphal.h"
#include "machine.h"
#include "extmod/machine_mem.h"
#include "lib/mpack/mpack.h"

#include <stdio.h>
#include <string.h>


typedef struct _openpie_syscall_result_t {
    byte *buf;
    size_t len;
} openpie_syscall_result_t;


#define _debug(s) __syscall2(SYS_DEBUG, (int)s, (int)strlen(s));
mp_obj_t usystem_debug(mp_obj_t);

mp_obj_t parse_2(void *ret) {
    if (ret == NULL) {
        return mp_const_none;
    } else {
        openpie_syscall_result_t *result = (openpie_syscall_result_t *) ret;
        return msgpack_loads((const char *)result->buf, result->len);
    }
}


#define EXEC_FLAG_PRINT_EOF (1)
#define EXEC_FLAG_ALLOW_DEBUGGING (2)
#define EXEC_FLAG_IS_REPL (4)
#define EXEC_FLAG_SOURCE_IS_RAW_CODE (8)
#define EXEC_FLAG_SOURCE_IS_VSTR (16)
#define EXEC_FLAG_SOURCE_IS_FILENAME (32)

STATIC mp_obj_t usystem_repl_input() {
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

MP_DEFINE_CONST_FUN_OBJ_0(usystem_repl_input_obj, usystem_repl_input);


/*
static inline mp_obj_dict_t *mp_globals_get(void) { return MP_STATE_THREAD(dict_globals); }
static inline void mp_globals_set(mp_obj_dict_t *d) { MP_STATE_THREAD(dict_globals) = d; }
 */

STATIC mp_obj_t usystem_repl_compile(mp_obj_t code, mp_obj_t globals_obj) {
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

MP_DEFINE_CONST_FUN_OBJ_2(usystem_repl_compile_obj, usystem_repl_compile);


STATIC mp_obj_t usystem_repl_call(mp_obj_t func, mp_obj_t locals_obj) {
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

MP_DEFINE_CONST_FUN_OBJ_2(usystem_repl_call_obj, usystem_repl_call);


mp_obj_t interrupt_hook_obj = mp_const_none;

STATIC mp_obj_t usystem_interrupt_hook(mp_obj_t hook) {
    interrupt_hook_obj = hook;
    return mp_const_none;
}

MP_DEFINE_CONST_FUN_OBJ_1(usystem_interrupt_hook_obj, usystem_interrupt_hook);


mp_obj_t print_hook_obj = mp_const_none;

STATIC mp_obj_t usystem_print_hook(mp_obj_t hook) {
    print_hook_obj = hook;
    return mp_const_none;
}

MP_DEFINE_CONST_FUN_OBJ_1(usystem_print_hook_obj, usystem_print_hook);


mp_obj_t input_hook_obj = mp_const_none;

STATIC mp_obj_t usystem_input_hook(mp_obj_t hook) {
    input_hook_obj = hook;
    return mp_const_none;
}

MP_DEFINE_CONST_FUN_OBJ_1(usystem_input_hook_obj, usystem_input_hook);


STATIC mp_obj_t usystem_invoke(size_t n_args, const mp_obj_t *args) {
    byte *data = NULL;
    size_t size = 0;

    mpack_writer_t *writer = msgpack_dump_new(&data, &size);
    mpack_start_array(writer, n_args);
    for (int i = 0; i < n_args; i++)
        msgpack_dump(writer, args[i]);
    mpack_finish_array(writer);
    msgpack_dump_close(writer);

    void *result = (void *) __syscall3(SYS_INVOKE, 0, (int) data, (int) size);

    mp_obj_t vals = parse_2(result);

    size_t count = 0;
    mp_obj_t *items = NULL;

    mp_obj_get_array(vals, &count, &items);
    if (count != 2) {
        mp_obj_t exc = mp_obj_new_exception_msg(&mp_type_RuntimeError, "Invalid invoke result");
        mp_obj_exception_add_traceback(exc, MP_QSTR__lt_machine_gt_, __LINE__, MP_QSTR__lt_invoke_gt_);
        nlr_raise(exc);
    }

    if (items[1] != mp_const_none) {
        mp_obj_t exc = mp_obj_new_exception_arg1(&mp_type_SystemError, items[1]);
        mp_obj_exception_add_traceback(exc, MP_QSTR__lt_machine_gt_, 0, MP_QSTR__lt_invoke_gt_);
        nlr_raise(exc);
    }

    if (items[0] == mp_const_none)
        return mp_const_none;

    size_t subcount = 0;
    mp_obj_t *subitems = NULL;
    mp_obj_get_array(items[0], &subcount, &subitems);
    if (subcount == 0)
        return mp_const_none;
    else if (subcount == 1)
        return subitems[0];
    else
        return items[0];
}

MP_DEFINE_CONST_FUN_OBJ_VAR(usystem_invoke_obj, 2, usystem_invoke);


STATIC mp_obj_t usystem_signal(mp_obj_t ticks_obj) {
    mp_int_t ticks = mp_obj_get_int(ticks_obj);
    void *result = (void *) __syscall2(SYS_SIGNAL, SYS_SIGNAL_REQUEST, (int) ticks); // sleep with ticks
    return parse_2(result);
}

MP_DEFINE_CONST_FUN_OBJ_1(usystem_signal_obj, usystem_signal);


STATIC mp_obj_t usystem_components() {
    void *result = (byte *) __syscall1(SYS_REQUEST, SYS_REQUEST_COMPONENTS);
    return parse_2(result);
}

MP_DEFINE_CONST_FUN_OBJ_0(usystem_components_obj, usystem_components);


STATIC mp_obj_t usystem_methods(mp_obj_t address_obj) {
    byte *data = NULL;
    size_t size = 0;

    mpack_writer_t *writer = msgpack_dump_new(&data, &size);
    mpack_start_array(writer, 1);
        msgpack_dump(writer, address_obj);
    mpack_finish_array(writer);
    msgpack_dump_close(writer);

    void *result = (void *) __syscall3(SYS_REQUEST, SYS_REQUEST_METHODS, (int) data, (int) size);
    return parse_2(result);
}

MP_DEFINE_CONST_FUN_OBJ_1(usystem_methods_obj, usystem_methods);


STATIC mp_obj_t usystem_annotations(mp_obj_t address_obj, mp_obj_t method_obj) {
    byte *data = NULL;
    size_t size = 0;

    mpack_writer_t *writer = msgpack_dump_new(&data, &size);
    mpack_start_array(writer, 2);
        msgpack_dump(writer, address_obj);
        msgpack_dump(writer, method_obj);
    mpack_finish_array(writer);
    msgpack_dump_close(writer);

    void *result = (void *) __syscall3(SYS_REQUEST, SYS_REQUEST_ANNOTATIONS, (int) data, (int) size);
    return parse_2(result);
}

MP_DEFINE_CONST_FUN_OBJ_2(usystem_annotations_obj, usystem_annotations);


mp_obj_t usystem_shutdown() {
    __syscall1(SYS_CONTROL, SYS_CONTROL_SHUTDOWN);
    __fatal_error("shutdown failure");
}

MP_DEFINE_CONST_FUN_OBJ_0(usystem_shutdown_obj, usystem_shutdown);


mp_obj_t usystem_reboot() {
    __syscall1(SYS_CONTROL, SYS_CONTROL_REBOOT);
    __fatal_error("reboot failure");
}

MP_DEFINE_CONST_FUN_OBJ_0(usystem_reboot_obj, usystem_reboot);


mp_obj_t usystem_debug(mp_obj_t obj) {
    size_t len = 0;
    const char *buf = mp_obj_str_get_data(obj, &len);
    int result = __syscall2(SYS_DEBUG, (int) buf, (int) len);
    (void)result;
    return mp_const_none;
}

MP_DEFINE_CONST_FUN_OBJ_1(usystem_debug_obj, usystem_debug);


STATIC const mp_rom_map_elem_t machine_module_globals_table[] = {
        {MP_ROM_QSTR(MP_QSTR___name__), MP_ROM_QSTR(MP_QSTR_machine)},

        {MP_ROM_QSTR(MP_QSTR_repl_input),     MP_ROM_PTR(&usystem_repl_input_obj)},
        {MP_ROM_QSTR(MP_QSTR_repl_compile),   MP_ROM_PTR(&usystem_repl_compile_obj)},
        {MP_ROM_QSTR(MP_QSTR_repl_call),      MP_ROM_PTR(&usystem_repl_call_obj)},

        {MP_ROM_QSTR(MP_QSTR_interrupt_hook), MP_ROM_PTR(&usystem_interrupt_hook_obj)},
        {MP_ROM_QSTR(MP_QSTR_input_hook),     MP_ROM_PTR(&usystem_input_hook_obj)},
        {MP_ROM_QSTR(MP_QSTR_print_hook),     MP_ROM_PTR(&usystem_print_hook_obj)},
        {MP_ROM_QSTR(MP_QSTR_invoke),         MP_ROM_PTR(&usystem_invoke_obj)},
        {MP_ROM_QSTR(MP_QSTR_signal),         MP_ROM_PTR(&usystem_signal_obj)},
        {MP_ROM_QSTR(MP_QSTR_components),     MP_ROM_PTR(&usystem_components_obj)},
        {MP_ROM_QSTR(MP_QSTR_methods),        MP_ROM_PTR(&usystem_methods_obj)},
        {MP_ROM_QSTR(MP_QSTR_annotations),    MP_ROM_PTR(&usystem_annotations_obj)},
        {MP_ROM_QSTR(MP_QSTR_shutdown),       MP_ROM_PTR(&usystem_shutdown_obj)},
        {MP_ROM_QSTR(MP_QSTR_reboot),         MP_ROM_PTR(&usystem_reboot_obj)},
        {MP_ROM_QSTR(MP_QSTR_debug),          MP_ROM_PTR(&usystem_debug_obj)},

        {MP_ROM_QSTR(MP_QSTR_mem8),     MP_ROM_PTR(&machine_mem8_obj)},
        {MP_ROM_QSTR(MP_QSTR_mem16),    MP_ROM_PTR(&machine_mem16_obj)},
        {MP_ROM_QSTR(MP_QSTR_mem32),    MP_ROM_PTR(&machine_mem32_obj)},
};
STATIC MP_DEFINE_CONST_DICT(machine_module_globals, machine_module_globals_table);

const mp_obj_module_t mp_module_machine = {
        .base = {&mp_type_module},
        .globals = (mp_obj_dict_t *) &machine_module_globals,
};
