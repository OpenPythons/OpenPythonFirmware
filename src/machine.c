#include "py/nlr.h"
#include "py/runtime.h"
#include "py/objexcept.h"
#include "lib/utils/interrupt_char.h"
#include "gccollect.h"
#include "openpie_mcu.h"
#include "machine.h"
#include "syscall.h"

MP_DEFINE_EXCEPTION(SystemError, Exception)

void _start(void) {
    int status = main(0, NULL);
    _exit(status);
    for (;;);
}

void _exit(int status) {
    __syscall1(SYS_CONTROL, SYS_CONTROL_SHUTDOWN);
}

void Reset_Handler(void) __attribute__((naked));

void Reset_Handler(void) {
    // set stack pointer
    __asm volatile ("ldr r0, =_svec");
    __asm volatile ("ldr r1, [r0]");
    __asm volatile ("mov sp, r1");

    // copy .data section from flash to RAM
    for (uint32_t *src = &_sidata, *dest = &_sdata; dest < &_edata;) {
        *dest++ = *src++;
    }

    // zero out .bss section
    for (uint32_t *dest = &_sbss; dest < &_ebss;) {
        *dest++ = 0;
    }

    OPENPIE_CONTROLLER->PENDING = (uint32_t) &MP_STATE_VM(mp_pending_exception);
    OPENPIE_CONTROLLER->EXCEPTION = (uint32_t) &MP_STATE_VM(mp_kbd_exception);
    OPENPIE_CONTROLLER->INTR_CHAR = (uint32_t) &mp_interrupt_char;

    // jump to board initialisation
    _start();
}

void Signal_Handler() {
    mp_obj_t handler = mp_obj_dict_get(
            MP_OBJ_FROM_PTR(&MP_STATE_VM(dict_main)),
            MP_OBJ_NEW_QSTR(MP_QSTR_signal_handler)
    ); // TODO: optimize
    mp_sched_schedule(handler, mp_const_none);
    __syscall1(SYS_CONTROL, SYS_CONTROL_RETURN);
}

typedef int (*Handler)(int, int, int);

void Call_Handler(Handler handler, int a1, int a2, int a3) {
    __fatal_error("call handler called");
    nlr_buf_t nlr;
    if (nlr_push(&nlr) == 0) {
        int value = handler(a1, a2, a3);
        __syscall2(SYS_CONTROL, SYS_CONTROL_RETURN, value);
    } else {
        // TODO: handle exception
        int value = -1;
        __syscall2(SYS_CONTROL, SYS_CONTROL_RETURN, value);
    }
}

const uint32_t startup_vector[] __attribute__((section(".startup"))) = {
        (uint32_t) &_estack,
        (uint32_t) &Reset_Handler,
        (uint32_t) &Signal_Handler,
        (uint32_t) &Call_Handler,
        // TODO: increase memory size
        // TODO: add custom handler for detect failure (or just turn off computer?)
};

void nlr_jump_fail(void *val) {
    __fatal_error("nlr_jump_fail");
}

void NORETURN __fatal_error(const char *msg) {
    __syscall2(SYS_CONTROL, SYS_CONTROL_CRASH, (int) msg);
    for (;;);
}

#ifndef NDEBUG

void MP_WEAK __assert_func(const char *file, int line, const char *func, const char *expr) {
    printf("Assertion '%s' failed, at file %s:%d\n", expr, file, line);
    __fatal_error("Assertion failed");
}

#endif
