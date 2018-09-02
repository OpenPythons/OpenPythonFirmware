#include <stdio.h>
#include <string.h>

#include "py/obj.h"
#include "py/objstr.h"
#include "umsgpack.h"
#include "lib/mpack/mpack.h"


mp_obj_t umsgpack_dumps(mp_obj_t obj) {
    // const char *umsgpack_dumps(mp_obj_t obj);
    mpack_writer_t *writer = umsgpack_dump(obj);
    if (writer == NULL)
        return mp_const_none;

    mp_obj_new_str(writer->data)


    return mp_const_true;
}

MP_DEFINE_CONST_FUN_OBJ_1(umsgpack_dumps_obj, umsgpack_dumps);


mp_obj_t umsgpack_loads(mp_obj_t obj) {
    // mp_obj_t umsgpack_loads(const char *buf, size_t len);

    return mp_const_none;
}

MP_DEFINE_CONST_FUN_OBJ_1(umsgpack_loads_obj, umsgpack_loads);


STATIC const mp_rom_map_elem_t umsgpack_module_globals_table[] = {
        {MP_ROM_QSTR(MP_QSTR___name__), MP_ROM_QSTR(MP_QSTR_umsgpack)},

        {MP_ROM_QSTR(MP_QSTR_debug),    MP_ROM_PTR(&umsgpack_debug_obj)},
};
STATIC MP_DEFINE_CONST_DICT(umsgpack_module_globals, umsgpack_module_globals_table);


const mp_obj_module_t mp_module_umsgpack = {
        .base = {&mp_type_module},
        .globals = (mp_obj_dict_t *) &umsgpack_module_globals,
};
