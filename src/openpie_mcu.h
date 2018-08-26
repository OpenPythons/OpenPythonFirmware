#ifndef OPENPIE_MCU
#define OPENPIE_MCU

typedef struct _openpie_io_t {
    volatile unsigned char TXR;
    volatile unsigned char RXR;
} openpie_io_t;

#define OPENPIE_IO ((openpie_io_t*)0x40000000)


typedef struct _openpie_controller_t {
    volatile uint32_t PENDING;
    volatile uint32_t EXCEPTION;
    volatile uint32_t INTR_CHAR;
    volatile uint32_t RAM_SIZE;
    volatile uint32_t IDLE;
    volatile uint32_t INSNS;
} openpie_controller_t;

#define OPENPIE_CONTROLLER ((openpie_controller_t*)0x40000100)


typedef struct _rtc_t {
    volatile uint32_t TICKS_MS;
} rtc_t;

#define RTC ((rtc_t*)0x40000200)


#endif
