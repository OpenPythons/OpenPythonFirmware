#include <stdio.h>
#include <string.h>

#include "py/nlr.h"
#include "py/smallint.h"
#include "py/obj.h"
#include "lib/timeutils/timeutils.h"
#include "extmod/utime_mphal.h"
#include "syscall.h"

mp_obj_t wrap_result(int retcode);


STATIC mp_obj_t utime_time_world() {
    return wrap_result(__syscall0(SYS_TIMER_WORLD_TIME));
}

MP_DEFINE_CONST_FUN_OBJ_0(utime_time_world_obj, utime_time_world);


STATIC mp_obj_t utime_time_up() {
    return wrap_result(__syscall0(SYS_TIMER_UP_TIME));
}

MP_DEFINE_CONST_FUN_OBJ_0(utime_time_up_obj, utime_time_up);


STATIC mp_obj_t utime_time_cpu() {
    return wrap_result(__syscall0(SYS_TIMER_CPU_TIME));
}

MP_DEFINE_CONST_FUN_OBJ_0(utime_time_cpu_obj, utime_time_cpu);


STATIC const mp_rom_map_elem_t time_module_globals_table[] = {
        {MP_ROM_QSTR(MP_QSTR___name__),   MP_ROM_QSTR(MP_QSTR_utime)},

        // MicroPython
        {MP_ROM_QSTR(MP_QSTR_sleep),      MP_ROM_PTR(&mp_utime_sleep_obj)},
        {MP_ROM_QSTR(MP_QSTR_sleep_ms),   MP_ROM_PTR(&mp_utime_sleep_ms_obj)},
        {MP_ROM_QSTR(MP_QSTR_sleep_us),   MP_ROM_PTR(&mp_utime_sleep_us_obj)},
        {MP_ROM_QSTR(MP_QSTR_ticks_ms),   MP_ROM_PTR(&mp_utime_ticks_ms_obj)},
        {MP_ROM_QSTR(MP_QSTR_ticks_us),   MP_ROM_PTR(&mp_utime_ticks_us_obj)},
        {MP_ROM_QSTR(MP_QSTR_ticks_cpu),  MP_ROM_PTR(&mp_utime_ticks_cpu_obj)},
        {MP_ROM_QSTR(MP_QSTR_ticks_add),  MP_ROM_PTR(&mp_utime_ticks_add_obj)},
        {MP_ROM_QSTR(MP_QSTR_ticks_diff), MP_ROM_PTR(&mp_utime_ticks_diff_obj)},
        
        // OpenComputers
        {MP_ROM_QSTR(MP_QSTR_time_world), MP_ROM_PTR(&utime_time_world_obj)},
        {MP_ROM_QSTR(MP_QSTR_time_up),    MP_ROM_PTR(&utime_time_up_obj)},
        {MP_ROM_QSTR(MP_QSTR_time_cpu),   MP_ROM_PTR(&utime_time_cpu_obj)},
};

STATIC MP_DEFINE_CONST_DICT(time_module_globals, time_module_globals_table);

const mp_obj_module_t mp_module_utime = {
        .base = {&mp_type_module},
        .globals = (mp_obj_dict_t *) &time_module_globals,
};
