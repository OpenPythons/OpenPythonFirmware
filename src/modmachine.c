#include <stdio.h>

#include "py/obj.h"
#include "mphalport.h"
#include "extmod/machine_mem.h"

STATIC const mp_rom_map_elem_t machine_module_globals_table[] = {
        {MP_ROM_QSTR(MP_QSTR___name__), MP_ROM_QSTR(MP_QSTR_machine)},

        {MP_ROM_QSTR(MP_QSTR_mem8),     MP_ROM_PTR(&machine_mem8_obj)},
        {MP_ROM_QSTR(MP_QSTR_mem16),    MP_ROM_PTR(&machine_mem16_obj)},
        {MP_ROM_QSTR(MP_QSTR_mem32),    MP_ROM_PTR(&machine_mem32_obj)},
};
STATIC MP_DEFINE_CONST_DICT(machine_module_globals, machine_module_globals_table);

const mp_obj_module_t mp_module_machine = {
        .base = {&mp_type_module},
        .globals = (mp_obj_dict_t *) &machine_module_globals,
};
