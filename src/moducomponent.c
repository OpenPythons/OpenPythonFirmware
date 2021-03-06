#include "py/obj.h"
#include "msgpack.h"
#include "syscall.h"

mp_obj_t wrap_result(int code);


STATIC mp_obj_t ucomponent_get_list(size_t n_args, const mp_obj_t *args) {
    if (n_args == 0) {
        return wrap_result(__syscall2(SYS_COMPONENT_LIST, 0, 0));
    } else {
        size_t len = 0;
        const char *buf = mp_obj_str_get_data(args[0], &len);
        return wrap_result(__syscall2(SYS_COMPONENT_LIST, (int) buf, (int) len));
    }
}

MP_DEFINE_CONST_FUN_OBJ_VAR_BETWEEN(ucomponent_get_list_obj, 0, 1, ucomponent_get_list);


STATIC mp_obj_t ucomponent_get_type(mp_obj_t address_obj) {
    mp_obj_t items[] = {address_obj};
    msgpack_result_t result = msgpack_args_dumps(1, items);
    return wrap_result(__syscall2(SYS_COMPONENT_TYPE, (int)result.data, (int)result.size));
}

MP_DEFINE_CONST_FUN_OBJ_1(ucomponent_get_type_obj, ucomponent_get_type);


STATIC mp_obj_t ucomponent_get_slot(mp_obj_t address_obj) {
    mp_obj_t items[] = {address_obj};
    msgpack_result_t result = msgpack_args_dumps(1, items);
    return wrap_result(__syscall2(SYS_COMPONENT_SLOT, (int)result.data, (int)result.size));
}

MP_DEFINE_CONST_FUN_OBJ_1(ucomponent_get_slot_obj, ucomponent_get_slot);


STATIC mp_obj_t ucomponent_get_methods(mp_obj_t address_obj) {
    mp_obj_t items[] = {address_obj};
    msgpack_result_t result = msgpack_args_dumps(1, items);
    return wrap_result(__syscall2(SYS_COMPONENT_METHODS, (int)result.data, (int)result.size));
}

MP_DEFINE_CONST_FUN_OBJ_1(ucomponent_get_methods_obj, ucomponent_get_methods);


STATIC mp_obj_t ucomponent_get_doc(mp_obj_t address_obj, mp_obj_t method_obj) {
    mp_obj_t items[] = {address_obj, method_obj};
    msgpack_result_t result = msgpack_args_dumps(2, items);
    return wrap_result(__syscall2(SYS_COMPONENT_DOC, (int)result.data, (int)result.size));
}

MP_DEFINE_CONST_FUN_OBJ_2(ucomponent_get_doc_obj, ucomponent_get_doc);


STATIC mp_obj_t ucomponent_invoke(size_t n_args, const mp_obj_t *args) {
    msgpack_result_t result = msgpack_args_dumps(n_args, args);
    mp_obj_t values = wrap_result(__syscall2(SYS_COMPONENT_INVOKE, (int)result.data, (int)result.size));
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

MP_DEFINE_CONST_FUN_OBJ_VAR(ucomponent_invoke_obj, 2, ucomponent_invoke);


STATIC mp_obj_t ucomponent_invokes(size_t n_args, const mp_obj_t *args) {
    msgpack_result_t result = msgpack_args_dumps(n_args, args);
    return wrap_result(__syscall2(SYS_COMPONENT_INVOKE, (int)result.data, (int)result.size));
}

MP_DEFINE_CONST_FUN_OBJ_VAR(ucomponent_invokes_obj, 2, ucomponent_invokes);


STATIC const mp_rom_map_elem_t ucomponent_module_globals_table[] = {
        {MP_ROM_QSTR(MP_QSTR___name__), MP_ROM_QSTR(MP_QSTR_ucomponent)},

        // components
        {MP_ROM_QSTR(MP_QSTR_get_list),             MP_ROM_PTR(&ucomponent_get_list_obj)},
        {MP_ROM_QSTR(MP_QSTR_get_type),             MP_ROM_PTR(&ucomponent_get_type_obj)},
        {MP_ROM_QSTR(MP_QSTR_get_slot),             MP_ROM_PTR(&ucomponent_get_slot_obj)},
        {MP_ROM_QSTR(MP_QSTR_get_methods),          MP_ROM_PTR(&ucomponent_get_methods_obj)},
        {MP_ROM_QSTR(MP_QSTR_get_doc),              MP_ROM_PTR(&ucomponent_get_doc_obj)},

        // invoke
        {MP_ROM_QSTR(MP_QSTR_invoke),               MP_ROM_PTR(&ucomponent_invoke_obj)},
        {MP_ROM_QSTR(MP_QSTR_invokes),              MP_ROM_PTR(&ucomponent_invokes_obj)},
};

STATIC MP_DEFINE_CONST_DICT(ucomponent_module_globals, ucomponent_module_globals_table);

const mp_obj_module_t mp_module_ucomponent = {
        .base = {&mp_type_module},
        .globals = (mp_obj_dict_t *) &ucomponent_module_globals,
};
