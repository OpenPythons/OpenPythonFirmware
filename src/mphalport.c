#include <stdint.h>
#include <stdio.h>
#include <string.h>

#include "py/runtime.h"
#include "py/objstr.h"
#include "mphalport.h"
#include "syscall.h"

extern mp_obj_t signal_hook_obj;
extern mp_obj_t input_hook_obj;
extern mp_obj_t print_hook_obj;

int mp_hal_stdin_rx_chr(void) {
    if (input_hook_obj == mp_const_none) {
        return 0;
    } else {
        mp_obj_t obj = mp_call_function_0(input_hook_obj);
        return (int) mp_obj_get_int(obj);
    }
}

void mp_hal_stdout_tx_str(const char *str) {
    mp_hal_stdout_tx_strn(str, strlen(str));
}

void mp_hal_stdout_tx_strn(const char *str, mp_uint_t len) {
    if (print_hook_obj == mp_const_none) {
        __syscall2(SYS_DEBUG, (int)str, (int)len);
    } else {
        mp_obj_t str_obj = mp_obj_new_str(str, len);
        mp_call_function_1(print_hook_obj, str_obj);
    }
}

void mp_hal_stdout_tx_strn_cooked(const char *str, size_t len) {
    mp_hal_stdout_tx_strn(str, len);
}

void mp_hal_delay_ms(mp_uint_t ms) {
    uint32_t start = mp_hal_ticks_ms();
    uint32_t elapsed = 0;
    while (elapsed < ms) {
        mp_handle_pending();
        int32_t ticks = (ms - elapsed) / (1000 / 20);
        mp_obj_t tick_obj = mp_obj_new_int(ticks < 0? 0 : ticks);
        mp_call_function_1(signal_hook_obj, tick_obj);
        elapsed = mp_hal_ticks_ms() - start;
    }
}

void mp_hal_delay_us(mp_uint_t us) {
    uint32_t start = mp_hal_ticks_us();
    while (mp_hal_ticks_us() - start < us) {
        mp_handle_pending();
    }
}

mp_uint_t mp_hal_ticks_ms(void) {
    return (mp_uint_t) __syscall0(SYS_TIMER_TICKS_MS);
}

mp_uint_t mp_hal_ticks_us(void) {
    return (mp_uint_t) __syscall0(SYS_TIMER_TICKS_US);
}

mp_uint_t mp_hal_ticks_cpu(void) {
    // return OPENPIE_CONTROLLER->INSNS;
    return 0; // return (mp_uint_t) __syscall0(SYS_TIMER_TICKS_CPU);
}
