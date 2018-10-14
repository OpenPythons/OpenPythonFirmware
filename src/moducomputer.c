#include "py/obj.h"
#include "machine.h"
#include "msgpack.h"
#include "syscall.h"

mp_obj_t wrap_result(int code);



STATIC mp_obj_t ucomputer_shutdown() {
    __syscall0(SYS_CONTROL_SHUTDOWN);
    __fatal_error("shutdown failure");
}

MP_DEFINE_CONST_FUN_OBJ_0(ucomputer_shutdown_obj, ucomputer_shutdown);


STATIC mp_obj_t ucomputer_reboot() {
    __syscall0(SYS_CONTROL_REBOOT);
    __fatal_error("reboot failure");
}

MP_DEFINE_CONST_FUN_OBJ_0(ucomputer_reboot_obj, ucomputer_reboot);


STATIC mp_obj_t ucomputer_crash(mp_obj_t message_obj) {
    size_t len = 0;
    const char *message = mp_obj_str_get_data(message_obj, &len);
    __syscall2(SYS_CONTROL_CRASH, (int) message, (int) len);
    __fatal_error("crash failure");
}

MP_DEFINE_CONST_FUN_OBJ_1(ucomputer_crash_obj, ucomputer_crash);


STATIC mp_obj_t ucomputer_push_signal(size_t n_args, const mp_obj_t *args) {
    msgpack_result_t result = msgpack_args_dumps(n_args, args);
    return wrap_result(__syscall2(SYS_SIGNAL_PUSH, (int)result.data, (int)result.size));
}

MP_DEFINE_CONST_FUN_OBJ_VAR(ucomputer_push_signal_obj, 1, ucomputer_push_signal);


STATIC mp_obj_t ucomputer_pop_signal(mp_obj_t ticks_obj) {
    mp_int_t ticks = mp_obj_get_int(ticks_obj);
    return wrap_result(__syscall1(SYS_SIGNAL_REQUEST, (int) ticks));
}

MP_DEFINE_CONST_FUN_OBJ_1(ucomputer_pop_signal_obj, ucomputer_pop_signal);


STATIC mp_obj_t ucomputer_get_cost_per_tick() {
    return wrap_result(__syscall0(SYS_COMPUTER_GET_COST_PER_TICK));
}

MP_DEFINE_CONST_FUN_OBJ_0(ucomputer_get_cost_per_tick_obj, ucomputer_get_cost_per_tick);


STATIC mp_obj_t ucomputer_last_error() {
    return wrap_result(__syscall0(SYS_COMPUTER_LAST_ERROR));
}

MP_DEFINE_CONST_FUN_OBJ_0(ucomputer_last_error_obj, ucomputer_last_error);


STATIC mp_obj_t ucomputer_beep(size_t n_args, const mp_obj_t *args) {
    switch (n_args) {
        case 1:
        {
            size_t len = 0;
            const char *buf = mp_obj_str_get_data(args[0], &len);

            return wrap_result(__syscall2(SYS_COMPUTER_BEEP_1, (int) buf, (int) len));
        }
        case 2:
        {
            int freq = mp_obj_get_int(args[0]);
            int dura = mp_obj_get_int(args[1]);

            return wrap_result(__syscall2(SYS_COMPUTER_BEEP_2, freq, dura));
        }
    }

    return mp_const_none;
}

MP_DEFINE_CONST_FUN_OBJ_VAR_BETWEEN(ucomputer_beep_obj, 1, 2, ucomputer_beep);


STATIC mp_obj_t ucomputer_users() {
    return wrap_result(__syscall0(SYS_COMPUTER_USERS));
}

MP_DEFINE_CONST_FUN_OBJ_0(ucomputer_users_obj, ucomputer_users);


STATIC mp_obj_t ucomputer_add_user(mp_obj_t user_obj) {
    size_t len = 0;
    const char *buf = mp_obj_str_get_data(user_obj, &len);

    return wrap_result(__syscall2(SYS_COMPUTER_ADD_USER, (int) buf, (int) len));
}

MP_DEFINE_CONST_FUN_OBJ_1(ucomputer_add_user_obj, ucomputer_add_user);


STATIC mp_obj_t ucomputer_remove_user(mp_obj_t user_obj) {
    size_t len = 0;
    const char *buf = mp_obj_str_get_data(user_obj, &len);

    return wrap_result(__syscall2(SYS_COMPUTER_REMOVE_USER, (int) buf, (int) len));
}

MP_DEFINE_CONST_FUN_OBJ_1(ucomputer_remove_user_obj, ucomputer_remove_user);


STATIC mp_obj_t ucomputer_get_computer_address() {
    return wrap_result(__syscall0(SYS_COMPUTER_COMPUTER_ADDRESS));
}

MP_DEFINE_CONST_FUN_OBJ_0(ucomputer_get_computer_address_obj, ucomputer_get_computer_address);


STATIC mp_obj_t ucomputer_get_tmp_address() {
    return wrap_result(__syscall0(SYS_COMPUTER_TMP_ADDRESS));
}

MP_DEFINE_CONST_FUN_OBJ_0(ucomputer_get_tmp_address_obj, ucomputer_get_tmp_address);


STATIC const mp_rom_map_elem_t ucomputer_module_globals_table[] = {
        {MP_ROM_QSTR(MP_QSTR___name__), MP_ROM_QSTR(MP_QSTR_ucomputer)},

        // control
        {MP_ROM_QSTR(MP_QSTR_shutdown),             MP_ROM_PTR(&ucomputer_shutdown_obj)},
        {MP_ROM_QSTR(MP_QSTR_reboot),               MP_ROM_PTR(&ucomputer_reboot_obj)},
        {MP_ROM_QSTR(MP_QSTR_crash),                MP_ROM_PTR(&ucomputer_crash_obj)},

        // signal
        {MP_ROM_QSTR(MP_QSTR_push_signal),          MP_ROM_PTR(&ucomputer_push_signal_obj)},
        {MP_ROM_QSTR(MP_QSTR_pop_signal),           MP_ROM_PTR(&ucomputer_pop_signal_obj)},

        // machine
        {MP_ROM_QSTR(MP_QSTR_get_cost_per_tick),    MP_ROM_PTR(&ucomputer_get_cost_per_tick_obj)},
        {MP_ROM_QSTR(MP_QSTR_last_error),           MP_ROM_PTR(&ucomputer_last_error_obj)},

        // beep
        {MP_ROM_QSTR(MP_QSTR_beep),                 MP_ROM_PTR(&ucomputer_beep_obj)},

        // users
        {MP_ROM_QSTR(MP_QSTR_users),                MP_ROM_PTR(&ucomputer_users_obj)},
        {MP_ROM_QSTR(MP_QSTR_add_user),             MP_ROM_PTR(&ucomputer_add_user_obj)},
        {MP_ROM_QSTR(MP_QSTR_remove_user),          MP_ROM_PTR(&ucomputer_remove_user_obj)},

        // address
        {MP_ROM_QSTR(MP_QSTR_get_computer_address), MP_ROM_PTR(&ucomputer_get_computer_address_obj)},
        {MP_ROM_QSTR(MP_QSTR_get_tmp_address),      MP_ROM_PTR(&ucomputer_get_tmp_address_obj)},
};

STATIC MP_DEFINE_CONST_DICT(ucomputer_module_globals, ucomputer_module_globals_table);

const mp_obj_module_t mp_module_ucomputer = {
        .base = {&mp_type_module},
        .globals = (mp_obj_dict_t *) &ucomputer_module_globals,
};
