#include <stdio.h>

#include "py/obj.h"
#include "py/objstr.h"
#include "py/runtime.h"
#include "py/mphal.h"
#include "syscall.h"
#include "msgpack.h"
#include "machine.h"
#include "lib/mpack/mpack.h"
#include "uvalue.h"

#include <stdio.h>
#include <string.h>

mp_obj_t wrap_result(int retcode);


mp_obj_t object_hook_obj = mp_const_none;

mp_obj_t uvalue_new(mp_obj_t value);

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


STATIC mp_obj_t uvalue_del(mp_obj_t raw_value_obj) {
    mp_int_t pointer = mp_obj_get_int(raw_value_obj);
    (void)pointer; // TODO: syscall
    return mp_const_none;
}

MP_DEFINE_CONST_FUN_OBJ_1(uvalue_del_obj, uvalue_del);


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
        } else if (MP_QSTR___del__ == attr) {
            dest[0] = (mp_obj_t)MP_OBJ_TO_PTR(&uvalue_del_obj);
        }
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


STATIC const mp_rom_map_elem_t uvalue_module_globals_table[] = {
        {MP_ROM_QSTR(MP_QSTR___name__), MP_ROM_QSTR(MP_QSTR_uvalue)},

        // objects
        {MP_ROM_QSTR(MP_QSTR_hook_value),  MP_ROM_PTR(&uvalue_hook_value_obj)},
        {MP_ROM_QSTR(MP_QSTR_UValue),      MP_ROM_PTR(&uvalue_type)},
};

STATIC MP_DEFINE_CONST_DICT(uvalue_module_globals, uvalue_module_globals_table);

const mp_obj_module_t mp_module_uvalue = {
        .base = {&mp_type_module},
        .globals = (mp_obj_dict_t *) &uvalue_module_globals,
};
