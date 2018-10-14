#include "py/obj.h"
#include "py/runtime.h"
#include "msgpack.h"

mp_obj_t umsgpack_dumps(mp_obj_t obj) {
    msgpack_result_t result = msgpack_dumps(obj);
    return mp_obj_new_bytes(result.data, result.size);
}

MP_DEFINE_CONST_FUN_OBJ_1(umsgpack_dumps_obj, umsgpack_dumps);

mp_obj_t umsgpack_loads(mp_obj_t obj) {
    size_t len = 0;
    const char *data = mp_obj_str_get_data(obj, &len);
    return msgpack_loads(data, len);
}

MP_DEFINE_CONST_FUN_OBJ_1(umsgpack_loads_obj, umsgpack_loads);


STATIC const mp_rom_map_elem_t umsgpack_module_globals_table[] = {
        {MP_ROM_QSTR(MP_QSTR___name__), MP_ROM_QSTR(MP_QSTR_umsgpack)},

        {MP_ROM_QSTR(MP_QSTR_dumps),    MP_ROM_PTR(&umsgpack_dumps_obj)},
        {MP_ROM_QSTR(MP_QSTR_loads),    MP_ROM_PTR(&umsgpack_loads_obj)},
};
STATIC MP_DEFINE_CONST_DICT(umsgpack_module_globals, umsgpack_module_globals_table);


const mp_obj_module_t mp_module_umsgpack = {
        .base = {&mp_type_module},
        .globals = (mp_obj_dict_t *) &umsgpack_module_globals,
};
