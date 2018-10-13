#include <stdio.h>

#include "py/obj.h"
#include "py/objstr.h"
#include "py/runtime.h"
#include "py/mphal.h"
#include "syscall.h"
#include "msgpack.h"
#include "machine.h"
#include "lib/mpack/mpack.h"

#include <stdio.h>
#include <string.h>


mp_obj_t wrap_result(int code);


STATIC mp_obj_t ucomponents_components(size_t n_args, const mp_obj_t *args) {
    if (n_args == 0) {
        return wrap_result(__syscall2(SYS_COMPONENTS_LIST, 0, 0));
    } else {
        size_t len = 0;
        const char *buf = mp_obj_str_get_data(args[0], &len);
        return wrap_result(__syscall2(SYS_COMPONENTS_LIST, (int) buf, (int) len));
    }
}

MP_DEFINE_CONST_FUN_OBJ_VAR_BETWEEN(ucomponents_components_obj, 0, 1, ucomponents_components);


STATIC mp_obj_t ucomponents_component_count() {
    return wrap_result(__syscall0(SYS_COMPONENTS_COUNT));
}

MP_DEFINE_CONST_FUN_OBJ_0(ucomponents_component_count_obj, ucomponents_component_count);


STATIC mp_obj_t ucomponents_max_components() {
    return wrap_result(__syscall0(SYS_COMPONENTS_MAX));
}

MP_DEFINE_CONST_FUN_OBJ_0(ucomponents_max_components_obj, ucomponents_max_components);


STATIC mp_obj_t ucomponents_methods(mp_obj_t address_obj) {
    mp_obj_t items[] = {address_obj};
    msgpack_result_t result = msgpack_args_dumps(1, items);
    return wrap_result(__syscall2(SYS_COMPONENTS_METHODS, (int)result.data, (int)result.size));
}

MP_DEFINE_CONST_FUN_OBJ_1(ucomponents_methods_obj, ucomponents_methods);


STATIC mp_obj_t ucomponents_doc(mp_obj_t address_obj, mp_obj_t method_obj) {
    mp_obj_t items[] = {address_obj, method_obj};
    msgpack_result_t result = msgpack_args_dumps(2, items);
    return wrap_result(__syscall2(SYS_COMPONENTS_DOC, (int)result.data, (int)result.size));
}

MP_DEFINE_CONST_FUN_OBJ_2(ucomponents_doc_obj, ucomponents_doc);


STATIC mp_obj_t ucomponents_invoke(size_t n_args, const mp_obj_t *args) {
    msgpack_result_t result = msgpack_args_dumps(n_args, args);
    mp_obj_t values = wrap_result(__syscall2(SYS_COMPONENTS_INVOKE, (int)result.data, (int)result.size));
    if (values == mp_const_none)
        return mp_const_none;

    size_t count = 0;
    mp_obj_t *items = NULL;
    mp_obj_get_array(values, &count, &items);
    if (count == 0)
        return mp_const_none;
    else if (count == 1)
        return items[0];
    else
        return values;
}

MP_DEFINE_CONST_FUN_OBJ_VAR(ucomponents_invoke_obj, 2, ucomponents_invoke);


STATIC const mp_rom_map_elem_t ucomponents_module_globals_table[] = {
        {MP_ROM_QSTR(MP_QSTR___name__), MP_ROM_QSTR(MP_QSTR_ucomponents)},

        // components
        {MP_ROM_QSTR(MP_QSTR_components),           MP_ROM_PTR(&ucomponents_components_obj)},
        {MP_ROM_QSTR(MP_QSTR_component_count),      MP_ROM_PTR(&ucomponents_component_count_obj)},
        {MP_ROM_QSTR(MP_QSTR_max_components),       MP_ROM_PTR(&ucomponents_max_components_obj)},
        {MP_ROM_QSTR(MP_QSTR_methods),              MP_ROM_PTR(&ucomponents_methods_obj)},
        {MP_ROM_QSTR(MP_QSTR_doc),                  MP_ROM_PTR(&ucomponents_doc_obj)},

        // invoke
        {MP_ROM_QSTR(MP_QSTR_invoke),               MP_ROM_PTR(&ucomponents_invoke_obj)},
};

STATIC MP_DEFINE_CONST_DICT(ucomponents_module_globals, ucomponents_module_globals_table);

const mp_obj_module_t mp_module_ucomponents = {
        .base = {&mp_type_module},
        .globals = (mp_obj_dict_t *) &ucomponents_module_globals,
};
