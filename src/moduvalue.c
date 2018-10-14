#include "py/obj.h"
#include "py/runtime.h"
#include "syscall.h"
#include "msgpack.h"
#include "machine.h"
#include "uvalue.h"

mp_obj_t wrap_result(int code);


mp_obj_t object_hook_obj = mp_const_none;


STATIC mp_obj_t uvalue_dispose(mp_obj_t raw_value_obj) {
    uvalue_obj_t *self = MP_OBJ_TO_PTR(raw_value_obj);
    if (self->value != mp_const_none) {
        msgpack_result_t result = msgpack_dumps(self); // UValue
        wrap_result(__syscall2(SYS_VALUE_DISPOSE, (int)result.data, (int)result.size));
        self->value = mp_const_none;
    }

    return mp_const_none;
}

MP_DEFINE_CONST_FUN_OBJ_1(uvalue_dispose_obj, uvalue_dispose);


STATIC void uvalue_print(const mp_print_t *print, mp_obj_t self_in, mp_print_kind_t kind) {
    (void)kind;
    uvalue_obj_t *self = MP_OBJ_TO_PTR(self_in);
    mp_obj_t value = self->value;
    (void)value;

    mp_printf(print, "<UValue at 0x%p>", self); // TODO: print value?
}

STATIC void uvalue_attr(mp_obj_t self_in, qstr attr, mp_obj_t *dest) {
    uvalue_obj_t *self = MP_OBJ_TO_PTR(self_in);
    if (dest[0] == MP_OBJ_NULL) {
        // load attribute
        if (MP_QSTR_value == attr) {
            dest[0] = MP_OBJ_TO_PTR(self->value);
        } else if (MP_QSTR___del__ == attr || MP_QSTR_dispose == attr) {
            mp_obj_t method = (mp_obj_t)MP_OBJ_TO_PTR(&uvalue_dispose_obj);
            mp_convert_member_lookup(self, MP_ROM_PTR(&uvalue_type), method, dest);
        }
    } else {
        mp_raise_msg(&mp_type_AttributeError, "can't set attribute");
    }
}

const mp_obj_type_t uvalue_type = {
    { &mp_type_type },
    .name = MP_QSTR_UValue,
    .print = uvalue_print,
    .attr = uvalue_attr,
};

mp_obj_t uvalue_new(mp_obj_t value) {
    uvalue_obj_t *o = m_new(uvalue_obj_t, 1);
    o->base.type = &uvalue_type;
    o->value = value;
    return MP_OBJ_FROM_PTR(o);
}


mp_obj_t oc_create_value(mp_obj_t raw_value_obj) {
    mp_obj_t value_obj = uvalue_new(raw_value_obj);
    if (object_hook_obj == mp_const_none) {
        return value_obj;
    }

    return mp_call_function_1(object_hook_obj, value_obj);
}


STATIC mp_obj_t uvalue_hook_value(mp_obj_t hook_obj) {
    object_hook_obj = hook_obj;
    return hook_obj;
}

MP_DEFINE_CONST_FUN_OBJ_1(uvalue_hook_value_obj, uvalue_hook_value);


STATIC mp_obj_t _uvalue__syscall(int syscall_number, size_t n_args, const mp_obj_t *args) {
    switch (syscall_number) {
        case SYS_VALUE_INVOKE:
        case SYS_VALUE_CALL:
        case SYS_VALUE_APPLY:
        case SYS_VALUE_UNAPPLY:
            break;
        default:
            mp_raise_ValueError("invalid call");
    }

    msgpack_result_t result = msgpack_args_dumps(n_args, args);
    return wrap_result(__syscall2(syscall_number, (int)result.data, (int)result.size));
}


STATIC mp_obj_t uvalue_invoke(size_t n_args, const mp_obj_t *args) {
    mp_obj_t values = _uvalue__syscall(SYS_VALUE_INVOKE, n_args, args);
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

MP_DEFINE_CONST_FUN_OBJ_VAR(uvalue_invoke_obj, 2, uvalue_invoke);


STATIC mp_obj_t uvalue_call(size_t n_args, const mp_obj_t *args) {
    return _uvalue__syscall(SYS_VALUE_CALL, n_args, args);
}

MP_DEFINE_CONST_FUN_OBJ_VAR(uvalue_call_obj, 1, uvalue_call);


STATIC mp_obj_t uvalue_apply(size_t n_args, const mp_obj_t *args) {
    return _uvalue__syscall(SYS_VALUE_CALL, n_args, args);
}

MP_DEFINE_CONST_FUN_OBJ_VAR(uvalue_apply_obj, 1, uvalue_apply);


STATIC mp_obj_t uvalue_unapply(size_t n_args, const mp_obj_t *args) {
    return _uvalue__syscall(SYS_VALUE_CALL, n_args, args);
}

MP_DEFINE_CONST_FUN_OBJ_VAR(uvalue_unapply_obj, 1, uvalue_unapply);


STATIC mp_obj_t uvalue_methods(mp_obj_t uvalue_obj) {
    mp_obj_t items[] = {uvalue_obj};
    msgpack_result_t result = msgpack_args_dumps(1, items);
    return wrap_result(__syscall2(SYS_VALUE_METHODS, (int)result.data, (int)result.size));
}

MP_DEFINE_CONST_FUN_OBJ_1(uvalue_methods_obj, uvalue_methods);


STATIC mp_obj_t uvalue_doc(size_t n_args, const mp_obj_t *args) {
    msgpack_result_t result = msgpack_args_dumps(n_args, args);
    return wrap_result(__syscall2(SYS_VALUE_DOC, (int)result.data, (int)result.size));
}

MP_DEFINE_CONST_FUN_OBJ_VAR_BETWEEN(uvalue_doc_obj, 1, 2, uvalue_doc);


STATIC const mp_rom_map_elem_t uvalue_module_globals_table[] = {
        {MP_ROM_QSTR(MP_QSTR___name__), MP_ROM_QSTR(MP_QSTR_uvalue)},

        // objects
        {MP_ROM_QSTR(MP_QSTR_UValue),      MP_ROM_PTR(&uvalue_type)},
        {MP_ROM_QSTR(MP_QSTR_hook_value),  MP_ROM_PTR(&uvalue_hook_value_obj)},

        // invoke
        {MP_ROM_QSTR(MP_QSTR_invoke),      MP_ROM_PTR(&uvalue_invoke_obj)},
        {MP_ROM_QSTR(MP_QSTR_call),        MP_ROM_PTR(&uvalue_call_obj)},
        {MP_ROM_QSTR(MP_QSTR_apply),       MP_ROM_PTR(&uvalue_apply_obj)},
        {MP_ROM_QSTR(MP_QSTR_unapply),     MP_ROM_PTR(&uvalue_unapply_obj)},
        {MP_ROM_QSTR(MP_QSTR_methods),     MP_ROM_PTR(&uvalue_methods_obj)},
        {MP_ROM_QSTR(MP_QSTR_doc),         MP_ROM_PTR(&uvalue_doc_obj)},
};

STATIC MP_DEFINE_CONST_DICT(uvalue_module_globals, uvalue_module_globals_table);

const mp_obj_module_t mp_module_uvalue = {
        .base = {&mp_type_module},
        .globals = (mp_obj_dict_t *) &uvalue_module_globals,
};
