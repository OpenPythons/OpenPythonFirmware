#include <stdio.h>
#include <string.h>

#include "py/obj.h"
#include "py/objstr.h"
#include "mphalport.h"
#include "openpie_vfs.h"
#include "openpie_mcu.h"
#include "syscall_arch.h"

typedef struct _openpie_syscall_result_t {
    byte *buf;
    size_t len;
} openpie_syscall_result_t;

#define _debug(s) __syscall2(32, (int)s, (int)strlen(s));

mp_obj_t parse_2(void *ret) {
    mp_obj_t usystem_debug(mp_obj_t);

    if (ret == NULL) {
        return mp_const_none;
    } else {
        openpie_syscall_result_t *result = (openpie_syscall_result_t *)ret;
        mp_obj_t obj = mp_obj_new_str_copy(&mp_type_str, result->buf, result->len);
        usystem_debug(obj);
        return obj;
    }
}

mp_obj_t print_hook = NULL;

STATIC mp_obj_t usystem_print_hook(mp_obj_t hook) {
    print_hook = hook;
    return mp_const_none;
}
MP_DEFINE_CONST_FUN_OBJ_1(usystem_print_hook_obj, usystem_print_hook);

STATIC mp_obj_t usystem_example(mp_obj_t obj) {
    // size_t len = 0;
    // const char *buf = mp_obj_str_get_data(obj, &len);
    return mp_const_none;
}
MP_DEFINE_CONST_FUN_OBJ_1(usystem_example_obj, usystem_example);


STATIC mp_obj_t usystem_syscall(mp_obj_t obj) {
    size_t len = 0;
    const char *buf = mp_obj_str_get_data(obj, &len);
    void *result = (void *)__syscall2(1, (int)buf, (int)len);
    return parse_2(result);
}
MP_DEFINE_CONST_FUN_OBJ_1(usystem_syscall_obj, usystem_syscall);


STATIC mp_obj_t usystem_signal() {
    void *result = (void *)__syscall0(2);
    return parse_2(result);
}
MP_DEFINE_CONST_FUN_OBJ_0(usystem_signal_obj, usystem_signal);


STATIC mp_obj_t usystem_components() {
    void *result = (byte *)__syscall0(3);
    return parse_2(result);
}
MP_DEFINE_CONST_FUN_OBJ_0(usystem_components_obj, usystem_components);


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
    void *result = (byte *)__syscall0(31);
    return parse_2(result);
}
MP_DEFINE_CONST_FUN_OBJ_0(usystem_get_stdout_str_obj, usystem_get_stdout_str);


mp_obj_t usystem_debug(mp_obj_t obj) {
    size_t len = 0;
    const char *buf = mp_obj_str_get_data(obj, &len);
    (void)__syscall2(32, (int)buf, (int)len);
    return mp_const_none;
}
MP_DEFINE_CONST_FUN_OBJ_1(usystem_debug_obj, usystem_debug);


STATIC const mp_rom_map_elem_t usystem_module_globals_table[] = {
    { MP_ROM_QSTR(MP_QSTR___name__), MP_ROM_QSTR(MP_QSTR_usystem) },

    { MP_ROM_QSTR(MP_QSTR_print_hook), MP_ROM_PTR(&usystem_print_hook_obj) },
    { MP_ROM_QSTR(MP_QSTR_example), MP_ROM_PTR(&usystem_example_obj) },
    { MP_ROM_QSTR(MP_QSTR_syscall), MP_ROM_PTR(&usystem_syscall_obj) },
    { MP_ROM_QSTR(MP_QSTR_signal), MP_ROM_PTR(&usystem_signal_obj) },
    { MP_ROM_QSTR(MP_QSTR_components), MP_ROM_PTR(&usystem_components_obj) },
    { MP_ROM_QSTR(MP_QSTR_set_stdin_char), MP_ROM_PTR(&usystem_set_stdin_char_obj) },
    { MP_ROM_QSTR(MP_QSTR_get_stdout_str), MP_ROM_PTR(&usystem_get_stdout_str_obj) },
    { MP_ROM_QSTR(MP_QSTR_debug), MP_ROM_PTR(&usystem_debug_obj) },
};
STATIC MP_DEFINE_CONST_DICT(usystem_module_globals, usystem_module_globals_table);


const mp_obj_module_t mp_module_usystem = {
    .base = { &mp_type_module },
    .globals = (mp_obj_dict_t*)&usystem_module_globals,
};
