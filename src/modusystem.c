#include <stdio.h>

#include "py/obj.h"
#include "mphalport.h"

#define svc(code) __asm volatile ("svc %[immediate]"::[immediate] "I" (code))

__attribute__ ((noinline)) void svc_0_test(int a, int b, int c, int d)
{
    printf("before-svc #0) a=%d; b=%d; c=%d; d=%d;\n", a, b, c, d);
    svc(0);
    printf("after-svc #0) a=%d; b=%d; c=%d; d=%d;\n", a, b, c, d);
}

STATIC mp_obj_t usystem_example(mp_obj_t obj) {
    size_t len = 0;
    const char *buf = mp_obj_str_get_data(obj, &len);

    svc_0_test((mp_uint_t)buf, len, 0, 0);

    return MP_OBJ_NEW_SMALL_INT(0);
}
MP_DEFINE_CONST_FUN_OBJ_1(usystem_example_obj, usystem_example);


STATIC const mp_rom_map_elem_t usystem_module_globals_table[] = {
    { MP_ROM_QSTR(MP_QSTR___name__), MP_ROM_QSTR(MP_QSTR_usystem) },

    { MP_ROM_QSTR(MP_QSTR_example), MP_ROM_PTR(&usystem_example_obj) },
};
STATIC MP_DEFINE_CONST_DICT(usystem_module_globals, usystem_module_globals_table);


const mp_obj_module_t mp_module_usystem = {
    .base = { &mp_type_module },
    .globals = (mp_obj_dict_t*)&usystem_module_globals,
};
