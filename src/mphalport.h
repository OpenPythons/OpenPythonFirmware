#include "py/obj.h"
#include "modmachine.h"

void mp_hal_delay_ms(mp_uint_t ms);

void mp_hal_delay_us(mp_uint_t us);

mp_uint_t mp_hal_ticks_ms(void);

mp_uint_t mp_hal_ticks_us(void);

mp_uint_t mp_hal_ticks_cpu(void);

void mp_hal_set_interrupt_char(int c);

static inline void mp_hal_delay_us_fast(uint32_t us) { return; }
