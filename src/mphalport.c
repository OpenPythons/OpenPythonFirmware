#include <stdint.h>
#include <stdio.h>

#include "py/runtime.h"
#include "umport_mcu.h"
#include "mphalport.h"

int mp_hal_stdin_rx_chr(void) {
    unsigned char c = 0;
    c = UART0->RXR;
    return c;
}

void mp_hal_stdout_tx_strn(const char *str, mp_uint_t len) {
    while (len--) {
        UART0->TXR = *str++;
    }
}

void mp_hal_delay_ms(mp_uint_t ms) {
    uint32_t start = mp_hal_ticks_ms();
    extern void mp_handle_pending(void);
    while (mp_hal_ticks_ms() - start < ms) {
        mp_handle_pending();
        UMPORT_CONTROLLER->IDLE = 1;
    }
}

void mp_hal_delay_us(mp_uint_t us) {
    uint32_t start = mp_hal_ticks_us();
    while (mp_hal_ticks_us() - start < us) {
        mp_handle_pending();
    }
}

mp_uint_t mp_hal_ticks_us(void) {
    return RTC->TICKS_US;
}

mp_uint_t mp_hal_ticks_ms(void) {
    return RTC->TICKS_MS;
}

mp_uint_t mp_hal_ticks_cpu(void) {
    return UMPORT_CONTROLLER->INSNS;
}
