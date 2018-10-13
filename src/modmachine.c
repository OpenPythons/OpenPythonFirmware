#include <stdio.h>

#include "py/obj.h"
#include "py/objstr.h"
#include "py/runtime.h"
#include "py/mphal.h"
#include "syscall.h"
#include "machine.h"
#include "extmod/machine_mem.h"
#include "lib/mpack/mpack.h"

#include <stdio.h>
#include <string.h>

mp_obj_t signal_hook_obj = mp_const_none;
mp_obj_t stdout_hook_obj = mp_const_none;
mp_obj_t stdin_hook_obj = mp_const_none;


mp_obj_t machine_debug(mp_obj_t obj) {
    size_t len = 0;
    const char *buf = mp_obj_str_get_data(obj, &len);

    (void) __syscall2(SYS_DEBUG, (int) buf, (int) len);
    return mp_const_none;
}

MP_DEFINE_CONST_FUN_OBJ_1(machine_debug_obj, machine_debug);


STATIC mp_obj_t machine_hook_signal(mp_obj_t hook_obj) {
    signal_hook_obj = hook_obj;
    return hook_obj;
}

MP_DEFINE_CONST_FUN_OBJ_1(machine_hook_signal_obj, machine_hook_signal);


STATIC mp_obj_t machine_hook_stdout(mp_obj_t hook_obj) {
    stdout_hook_obj = hook_obj;
    return hook_obj;
}

MP_DEFINE_CONST_FUN_OBJ_1(machine_hook_stdout_obj, machine_hook_stdout);


STATIC mp_obj_t machine_hook_stdin(mp_obj_t hook_obj) {
    stdin_hook_obj = hook_obj;
    return hook_obj;
}

MP_DEFINE_CONST_FUN_OBJ_1(machine_hook_stdin_obj, machine_hook_stdin);


STATIC const mp_rom_map_elem_t machine_module_globals_table[] = {
        {MP_ROM_QSTR(MP_QSTR___name__), MP_ROM_QSTR(MP_QSTR_machine)},

        // debug
        {MP_ROM_QSTR(MP_QSTR_debug),                MP_ROM_PTR(&machine_debug_obj)},

        // hook
        {MP_ROM_QSTR(MP_QSTR_hook_signal),          MP_ROM_PTR(&machine_hook_signal_obj)},
        {MP_ROM_QSTR(MP_QSTR_hook_stdout),          MP_ROM_PTR(&machine_hook_stdout_obj)},
        {MP_ROM_QSTR(MP_QSTR_hook_stdin),           MP_ROM_PTR(&machine_hook_stdin_obj)},
};

STATIC MP_DEFINE_CONST_DICT(machine_module_globals, machine_module_globals_table);

const mp_obj_module_t mp_module_machine = {
        .base = {&mp_type_module},
        .globals = (mp_obj_dict_t *) &machine_module_globals,
};
