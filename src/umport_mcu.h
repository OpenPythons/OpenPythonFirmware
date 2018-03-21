#ifndef UMPORT_MCU
#define UMPORT_MCU

typedef struct _umport_controller_t {
    volatile uint32_t PENDING;
    volatile uint32_t EXCEPTION;
    volatile uint32_t INTR_CHAR;
    volatile uint32_t RAM_SIZE;
    volatile uint32_t STACK_SIZE;
    volatile uint32_t IDLE;
    volatile uint32_t INSNS;
} umport_controller_t;

#define UMPORT_CONTROLLER ((umport_controller_t*)0x40000100)

typedef struct _gpio_t {
    volatile uint32_t ODR;
    volatile uint32_t IDR;
} gpio_t;

#define GPIO ((gpio_t*)0x40000200)
#define GPIO_X ((gpio_t*)0x40000208)
#define GPIO_Y ((gpio_t*)0x40000210)

typedef struct _rtc_t {
    volatile uint32_t TICKS_MS;
    volatile uint32_t TICKS_US;
} rtc_t;

#define RTC ((rtc_t*)0x40000300)

#endif
