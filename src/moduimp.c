#include "py/obj.h"


STATIC mp_obj_t uimp_new_module(mp_obj_t name_obj) {
    size_t len = 0;
    const char *name = mp_obj_str_get_data(name_obj, &len);

    qstr qst = qstr_from_strn(name, len);

    // py/objmodule.c

    // create new module object
    mp_obj_module_t *o = m_new_obj(mp_obj_module_t);
    o->base.type = &mp_type_module;
    o->globals = MP_OBJ_TO_PTR(mp_obj_new_dict(MICROPY_MODULE_DICT_SIZE));

    // store __name__ entry in the module
    mp_obj_dict_store(MP_OBJ_FROM_PTR(o->globals), MP_OBJ_NEW_QSTR(MP_QSTR___name__), MP_OBJ_NEW_QSTR(qst));

    // return the new module
    return MP_OBJ_FROM_PTR(o);
}

MP_DEFINE_CONST_FUN_OBJ_1(uimp_new_module_obj, uimp_new_module);


STATIC const mp_rom_map_elem_t uimp_module_globals_table[] = {
        {MP_ROM_QSTR(MP_QSTR___name__), MP_ROM_QSTR(MP_QSTR_uimp)},

        {MP_ROM_QSTR(MP_QSTR_new_module),      MP_ROM_PTR(&uimp_new_module_obj)},
};
STATIC MP_DEFINE_CONST_DICT(uimp_module_globals, uimp_module_globals_table);

const mp_obj_module_t mp_module_uimp = {
        .base = {&mp_type_module},
        .globals = (mp_obj_dict_t *) &uimp_module_globals,
};
