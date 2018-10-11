#include <stdio.h>

#include "py/obj.h"
#include "py/objstr.h"
#include "py/runtime.h"
#include "py/mphal.h"
#include "syscall.h"
#include "msgpack.h"
#include "machine.h"
#include "lib/mpack/mpack.h"

#include <stdio.h>
#include <string.h>


typedef struct _syscall_result_t {
    int status;
    byte *buf;
    size_t len;
} syscall_result_t;

#define _debug(s) __syscall2(SYS_DEBUG, (int)s, (int)strlen(s));
mp_obj_t ucomputer_debug(mp_obj_t);


STATIC mp_obj_t wrap_result(int retcode) {
    void *ret = (void *) retcode;
    if (ret == NULL) {
        return mp_const_none;
    } else {
        syscall_result_t *result = (syscall_result_t *) ret;
        int status = result->status;
        switch (status) {
            case 0: // ERROR
            {
                mp_obj_t args_obj = msgpack_loads((const char *)result->buf, result->len);

                size_t count = 0;
                mp_obj_t *items = NULL;
                mp_obj_get_array(args_obj, &count, &items);

                mp_obj_t exc = mp_obj_new_exception_args(&mp_type_SystemError, count, items);
                mp_obj_exception_add_traceback(exc, MP_QSTR__lt_ucomputer_gt_, 0, MP_QSTR__lt_syscall_gt_);
                nlr_raise(exc);
            }
            case 1: // OK (msgpack)
                return msgpack_loads((const char *)result->buf, result->len);
            case 2: // OK (int)
                // return msgpack_loads((const char *)result->buf, result->len);
                return mp_const_none; // NIY
            default:
            {
                mp_obj_t exc = mp_obj_new_exception_arg1(&mp_type_SystemError,
                    MP_OBJ_NEW_QSTR(MP_QSTR_invalid_code));
                nlr_raise(exc);
            }
        }
    }
}


mp_obj_t ucomputer_shutdown() {
    __syscall0(SYS_CONTROL_SHUTDOWN);
    __fatal_error("shutdown failure");
}

MP_DEFINE_CONST_FUN_OBJ_0(ucomputer_shutdown_obj, ucomputer_shutdown);


mp_obj_t ucomputer_reboot() {
    __syscall0(SYS_CONTROL_REBOOT);
    __fatal_error("reboot failure");
}

MP_DEFINE_CONST_FUN_OBJ_0(ucomputer_reboot_obj, ucomputer_reboot);


mp_obj_t ucomputer_crash(mp_obj_t message_obj) {
    size_t len = 0;
    const char *message = mp_obj_str_get_data(message_obj, &len);
    __syscall2(SYS_CONTROL_CRASH, (int) message, (int) len);
    __fatal_error("crash failure");
}

MP_DEFINE_CONST_FUN_OBJ_1(ucomputer_crash_obj, ucomputer_crash);


STATIC mp_obj_t ucomputer_push_signal(size_t n_args, const mp_obj_t *args) {
    byte *data = NULL;
    size_t size = 0;

    mpack_writer_t *writer = msgpack_dump_new(&data, &size);
    mpack_start_array(writer, n_args);
    for (int i = 0; i < n_args; i++)
        msgpack_dump(writer, args[i]);
    mpack_finish_array(writer);
    msgpack_dump_close(writer);

    return wrap_result(__syscall2(SYS_SIGNAL_PUSH, (int) data, (int) size));
}

MP_DEFINE_CONST_FUN_OBJ_VAR(ucomputer_push_signal_obj, 1, ucomputer_push_signal);


STATIC mp_obj_t ucomputer_pop_signal(mp_obj_t ticks_obj) {
    mp_int_t ticks = mp_obj_get_int(ticks_obj);
    return wrap_result(__syscall1(SYS_SIGNAL_REQUEST, (int) ticks));
}

MP_DEFINE_CONST_FUN_OBJ_1(ucomputer_pop_signal_obj, ucomputer_pop_signal);


STATIC mp_obj_t ucomputer_components(size_t n_args, const mp_obj_t *args) {
    if (n_args == 0) {
        return wrap_result(__syscall2(SYS_COMPUTER_COMPONENTS, 0, 0));
    } else {
        size_t len = 0;
        const char *buf = mp_obj_str_get_data(args[0], &len);
        return wrap_result(__syscall2(SYS_COMPUTER_COMPONENTS, (int) buf, (int) len));
    }
}

MP_DEFINE_CONST_FUN_OBJ_VAR_BETWEEN(ucomputer_components_obj, 0, 1, ucomputer_components);


STATIC mp_obj_t ucomputer_component_count() {
    return wrap_result(__syscall0(SYS_COMPUTER_COMPONENT_COUNT));
}

MP_DEFINE_CONST_FUN_OBJ_0(ucomputer_component_count_obj, ucomputer_component_count);


STATIC mp_obj_t ucomputer_max_components() {
    return wrap_result(__syscall0(SYS_COMPUTER_MAX_COMPONENT));
}

MP_DEFINE_CONST_FUN_OBJ_0(ucomputer_max_components_obj, ucomputer_max_components);


STATIC mp_obj_t ucomputer_methods(mp_obj_t address_obj) {
    byte *data = NULL;
    size_t size = 0;

    mpack_writer_t *writer = msgpack_dump_new(&data, &size);
    mpack_start_array(writer, 1);
        msgpack_dump(writer, address_obj);
    mpack_finish_array(writer);
    msgpack_dump_close(writer);

    return wrap_result(__syscall2(SYS_COMPUTER_METHODS, (int) data, (int) size));
}

MP_DEFINE_CONST_FUN_OBJ_1(ucomputer_methods_obj, ucomputer_methods);


STATIC mp_obj_t ucomputer_annotations(mp_obj_t address_obj, mp_obj_t method_obj) {
    byte *data = NULL;
    size_t size = 0;

    mpack_writer_t *writer = msgpack_dump_new(&data, &size);
    mpack_start_array(writer, 2);
        msgpack_dump(writer, address_obj);
        msgpack_dump(writer, method_obj);
    mpack_finish_array(writer);
    msgpack_dump_close(writer);

    return wrap_result(__syscall2(SYS_COMPUTER_ANNOTATIONS, (int) data, (int) size));
}

MP_DEFINE_CONST_FUN_OBJ_2(ucomputer_annotations_obj, ucomputer_annotations);


STATIC mp_obj_t ucomputer_invoke(size_t n_args, const mp_obj_t *args) {
    byte *data = NULL;
    size_t size = 0;

    mpack_writer_t *writer = msgpack_dump_new(&data, &size);
    mpack_start_array(writer, n_args);
    for (int i = 0; i < n_args; i++)
        msgpack_dump(writer, args[i]);
    mpack_finish_array(writer);
    msgpack_dump_close(writer);

    mp_obj_t values = wrap_result(__syscall2(SYS_INVOKE, (int) data, (int) size));
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

MP_DEFINE_CONST_FUN_OBJ_VAR(ucomputer_invoke_obj, 2, ucomputer_invoke);


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


STATIC mp_obj_t ucomputer_world_time() {
    return wrap_result(__syscall0(SYS_TIMER_WORLD_TIME));
}

MP_DEFINE_CONST_FUN_OBJ_0(ucomputer_world_time_obj, ucomputer_world_time);


STATIC mp_obj_t ucomputer_up_time() {
    return wrap_result(__syscall0(SYS_TIMER_UP_TIME));
}

MP_DEFINE_CONST_FUN_OBJ_0(ucomputer_up_time_obj, ucomputer_up_time);


STATIC mp_obj_t ucomputer_cpu_time() {
    return wrap_result(__syscall0(SYS_TIMER_CPU_TIME));
}

MP_DEFINE_CONST_FUN_OBJ_0(ucomputer_cpu_time_obj, ucomputer_cpu_time);


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

        // components
        {MP_ROM_QSTR(MP_QSTR_components),           MP_ROM_PTR(&ucomputer_components_obj)},
        {MP_ROM_QSTR(MP_QSTR_component_count),      MP_ROM_PTR(&ucomputer_component_count_obj)},
        {MP_ROM_QSTR(MP_QSTR_max_components),       MP_ROM_PTR(&ucomputer_max_components_obj)},
        {MP_ROM_QSTR(MP_QSTR_methods),              MP_ROM_PTR(&ucomputer_methods_obj)},
        {MP_ROM_QSTR(MP_QSTR_annotations),          MP_ROM_PTR(&ucomputer_annotations_obj)},

        // invoke
        {MP_ROM_QSTR(MP_QSTR_invoke),               MP_ROM_PTR(&ucomputer_invoke_obj)},

        // machine
        {MP_ROM_QSTR(MP_QSTR_get_cost_per_tick),    MP_ROM_PTR(&ucomputer_get_cost_per_tick_obj)},
        {MP_ROM_QSTR(MP_QSTR_last_error),           MP_ROM_PTR(&ucomputer_last_error_obj)},

        // beep
        {MP_ROM_QSTR(MP_QSTR_beep),                 MP_ROM_PTR(&ucomputer_beep_obj)},

        // time
        {MP_ROM_QSTR(MP_QSTR_world_time),           MP_ROM_PTR(&ucomputer_world_time_obj)},
        {MP_ROM_QSTR(MP_QSTR_up_time),              MP_ROM_PTR(&ucomputer_up_time_obj)},
        {MP_ROM_QSTR(MP_QSTR_cpu_time),             MP_ROM_PTR(&ucomputer_cpu_time_obj)},

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
