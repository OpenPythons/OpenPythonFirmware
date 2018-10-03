#include <stdio.h>
#include <string.h>

#include "py/obj.h"
#include "py/objstr.h"
#include "py/runtime.h"
#include "mphalport.h"
#include "openpie_vfs.h"
#include "openpie_mcu.h"
#include "syscall.h"
#include "msgpack.h"
#include "lib/mpack/mpack.h"

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

mp_obj_t print_hook = NULL;

STATIC mp_obj_t usystem_print_hook(mp_obj_t hook) {
    print_hook = hook;
    return mp_const_none;
}

MP_DEFINE_CONST_FUN_OBJ_1(usystem_print_hook_obj, usystem_print_hook);


STATIC mp_obj_t usystem_invoke(size_t n_args, const mp_obj_t *args) {
    byte *data;
    size_t size = 0;

    mpack_writer_t *writer = msgpack_dump_new(&data, &size);
    mpack_start_array(writer, n_args);
    for (int i = 0; i < n_args; i++)
        msgpack_dump(writer, args[i]);
    mpack_finish_array(writer);
    msgpack_dump_close(writer);

    void *result = (void *) __syscall2(SYS_INVOKE, (int) data, (int) size);
    return parse_2(result);
}

MP_DEFINE_CONST_FUN_OBJ_VAR(usystem_invoke_obj, 2, usystem_invoke);


STATIC mp_obj_t usystem_signal() {
    void *result = (void *) __syscall1(SYS_REQUEST, 0); // 0 = signal
    return parse_2(result);
}

MP_DEFINE_CONST_FUN_OBJ_0(usystem_signal_obj, usystem_signal);


STATIC mp_obj_t usystem_components() {
    void *result = (byte *) __syscall1(SYS_REQUEST, SYS_REQUEST_COMPONENTS);
    return parse_2(result);
}

MP_DEFINE_CONST_FUN_OBJ_0(usystem_components_obj, usystem_components);


STATIC mp_obj_t usystem_methods(mp_obj_t address_obj, mp_obj_t method_obj) {
    byte *data;
    size_t size = 0;

    mpack_writer_t *writer = msgpack_dump_new(&data, &size);
    mpack_start_array(writer, 2);
        msgpack_dump(writer, address_obj);
        msgpack_dump(writer, method_obj);
    mpack_finish_array(writer);
    msgpack_dump_close(writer);

    void *result = (void *) __syscall3(SYS_REQUEST, SYS_REQUEST_METHODS, (int) data, (int) size);
    return parse_2(result);
}

MP_DEFINE_CONST_FUN_OBJ_2(usystem_methods_obj, usystem_methods);


STATIC mp_obj_t usystem_annotations(mp_obj_t address_obj) {
    byte *data;
    size_t size = 0;

    mpack_writer_t *writer = msgpack_dump_new(&data, &size);
    mpack_start_array(writer, 1);
        msgpack_dump(writer, address_obj);
    mpack_finish_array(writer);
    msgpack_dump_close(writer);

    void *result = (void *) __syscall3(SYS_REQUEST, SYS_REQUEST_ANNOTATIONS, (int) data, (int) size);
    return parse_2(result);
}

MP_DEFINE_CONST_FUN_OBJ_1(usystem_annotations_obj, usystem_annotations);


mp_obj_t usystem_set_stdin_char(mp_obj_t obj) {
    if (MP_OBJ_IS_INT(obj)) {
        mp_int_t value = mp_obj_get_int(obj);
        OPENPIE_IO->RXR = value;
        return mp_const_true;
    }

    return mp_const_false;
}

MP_DEFINE_CONST_FUN_OBJ_1(usystem_set_stdin_char_obj, usystem_set_stdin_char);


mp_obj_t usystem_get_stdout_str() {
    void *result = (byte *) __syscall1(SYS_LEGACY, SYS_LEGACY_usystem_get_stdout_str);
    return parse_2(result);
}

MP_DEFINE_CONST_FUN_OBJ_0(usystem_get_stdout_str_obj, usystem_get_stdout_str);

mp_obj_t usystem_debug(mp_obj_t obj) {
    size_t len = 0;
    const char *buf = mp_obj_str_get_data(obj, &len);
    int result = __syscall2(SYS_DEBUG, (int) buf, (int) len);
    (void)result;
    return mp_const_none;
}

MP_DEFINE_CONST_FUN_OBJ_1(usystem_debug_obj, usystem_debug);


STATIC const mp_rom_map_elem_t usystem_module_globals_table[] = {
    {MP_ROM_QSTR(MP_QSTR___name__),       MP_ROM_QSTR(MP_QSTR_usystem)},

    {MP_ROM_QSTR(MP_QSTR_print_hook),     MP_ROM_PTR(&usystem_print_hook_obj)},
    {MP_ROM_QSTR(MP_QSTR_invoke),         MP_ROM_PTR(&usystem_invoke_obj)},
    {MP_ROM_QSTR(MP_QSTR_signal),         MP_ROM_PTR(&usystem_signal_obj)},
    {MP_ROM_QSTR(MP_QSTR_components),     MP_ROM_PTR(&usystem_components_obj)},
    {MP_ROM_QSTR(MP_QSTR_methods),        MP_ROM_PTR(&usystem_methods_obj)},
    {MP_ROM_QSTR(MP_QSTR_annotations),    MP_ROM_PTR(&usystem_annotations_obj)},
    {MP_ROM_QSTR(MP_QSTR_set_stdin_char), MP_ROM_PTR(&usystem_set_stdin_char_obj)},
    {MP_ROM_QSTR(MP_QSTR_get_stdout_str), MP_ROM_PTR(&usystem_get_stdout_str_obj)},
    {MP_ROM_QSTR(MP_QSTR_debug),          MP_ROM_PTR(&usystem_debug_obj)},
};
STATIC MP_DEFINE_CONST_DICT(usystem_module_globals, usystem_module_globals_table);


const mp_obj_module_t mp_module_usystem = {
        .base = {&mp_type_module},
        .globals = (mp_obj_dict_t *) &usystem_module_globals,
};
