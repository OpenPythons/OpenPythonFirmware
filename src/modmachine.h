#ifndef MICROPY_INCLUDED_MODMACHINE_H
#define MICROPY_INCLUDED_MODMACHINE_H

#include "umport_mcu.h"

typedef struct _machine_pin_obj_t {
    mp_obj_base_t base;
    qstr name;
    gpio_t *port;
    mp_uint_t pin;
} machine_pin_obj_t;

extern const mp_obj_type_t machine_pin_type;

machine_pin_obj_t *machine_pin_get(mp_obj_t *obj_in);
void pin_set(mp_obj_t self_in, int value);

extern const mp_obj_type_t machine_hard_i2c_type;

#endif
