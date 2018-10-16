#include <string.h>

#include "py/nlr.h"
#include "py/runtime.h"
#include "py/objexcept.h"
#include "lib/utils/interrupt_char.h"
#include "gccollect.h"
#include "machine.h"
#include "syscall.h"

extern mp_obj_t signal_hook_obj;

MP_DEFINE_EXCEPTION(SystemError, Exception)

void _start(void) {
    int status = main(0, NULL);
    _exit(status);
    for (;;);
}

void _exit(int status) {
    for (;;);
    // __syscall0(SYS_CONTROL_SHUTDOWN);
}

void Reset_Handler(void) __attribute__((naked));

void Reset_Handler(void) {
    // set stack pointer
    __asm volatile ("ldr r0, =_svec");
    __asm volatile ("ldr r1, [r0]");
    __asm volatile ("mov sp, r1");

    // copy .data section from flash to RAM
    if (__syscall3(SYS_CONTROL_INIT_COPY, (int) &_sidata, (int) &_sdata, (int) &_edata) == 0) {
        for (uint32_t *src = &_sidata, *dest = &_sdata; dest < &_edata;) {
            *dest++ = *src++;
        }
    }

    // zero out .bss section
    if (__syscall2(SYS_CONTROL_INIT_ZERO, (int) &_sbss, (int) &_ebss) == 0) {
        for (uint32_t *dest = &_sbss; dest < &_ebss;) {
            *dest++ = 0;
        }
    }

    // OPENPIE_CONTROLLER->PENDING = (uint32_t) &MP_STATE_VM(mp_pending_exception);
    // OPENPIE_CONTROLLER->EXCEPTION = (uint32_t) &MP_STATE_VM(mp_kbd_exception);
    // OPENPIE_CONTROLLER->INTR_CHAR = (uint32_t) &mp_interrupt_char;

    // jump to board initialisation
    _start();
}


int Call_Handler(void *handler, int n_args, int *args) {
    nlr_buf_t nlr;
    if (nlr_push(&nlr) == 0) {
        int result = 0;
        switch (n_args) {
            case 0:
                result = ((int (*)())handler)();
                break;
            case 1:
                result = ((int (*)(int))handler)(args[0]);
                break;
            case 2:
                result = ((int (*)(int, int))handler)(args[0], args[1]);
                break;
            case 3:
                result = ((int (*)(int, int, int))handler)(args[0], args[1], args[2]);
                break;
            case 4:
                result = ((int (*)(int, int, int, int))handler)(args[0], args[1], args[2], args[3]);
                break;
            default:
                __fatal_error("invalid n_args");
        }

        nlr_pop();
        __syscall1(SYS_CONTROL_RETURN, result);
    } else {
        __syscall1(SYS_CONTROL_RETURN, 0);
    }

    __fatal_error("invalid control");
}


const uint32_t startup_vector[] __attribute__((section(".startup"))) = {
        (uint32_t) &_estack,
        (uint32_t) &Reset_Handler,
        (uint32_t) &Call_Handler,
        // TODO: add custom handler for detect failure (or just turn off computer?)
};

void nlr_jump_fail(void *val) {
    __fatal_error("nlr_jump_fail");
}

void NORETURN __fatal_error(const char *msg) {
    __syscall2(SYS_CONTROL_CRASH, (int) msg, (int) strlen(msg));
    for (;;);
}

#ifndef NDEBUG

void MP_WEAK __assert_func(const char *file, int line, const char *func, const char *expr) {
    printf("Assertion '%s' failed, at file %s:%d\n", expr, file, line);
    __fatal_error("Assertion failed");
}

#endif
