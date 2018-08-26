#include <stdint.h>
#include <stdio.h>

#include "py/runtime.h"
#include "py/objstr.h"
#include "openpie_mcu.h"
#include "mphalport.h"

extern mp_obj_t print_hook;

int mp_hal_stdin_rx_chr(void) {
    unsigned char c = 0;
    mp_handle_pending();
    c = OPENPIE_IO->RXR;
    return c;
}

void mp_hal_stdout_tx_strn(const char *str, mp_uint_t len) {
    mp_obj_t signal_buf = mp_obj_new_str(str, len);
    mp_obj_t handler = mp_obj_dict_get(
        MP_OBJ_FROM_PTR(&MP_STATE_VM(dict_main)),
        MP_OBJ_NEW_QSTR(MP_QSTR_print_handler)
    );

    mp_call_function_1_protected(handler, signal_buf);
}

void mp_hal_delay_ms(mp_uint_t ms) {
    uint32_t start = mp_hal_ticks_ms();
    extern void mp_handle_pending(void);
    while (mp_hal_ticks_ms() - start < ms) {
        mp_handle_pending();
        OPENPIE_CONTROLLER->IDLE = 1;
    }
}

void mp_hal_delay_us(mp_uint_t us) {
    uint32_t start = mp_hal_ticks_us();
    while (mp_hal_ticks_us() - start < us) {
        mp_handle_pending();
    }
}

mp_uint_t mp_hal_ticks_us(void) {
    return mp_hal_ticks_ms() * 1000;
}

mp_uint_t mp_hal_ticks_ms(void) {
    return RTC->TICKS_MS;
}

mp_uint_t mp_hal_ticks_cpu(void) {
    return OPENPIE_CONTROLLER->INSNS;
}
