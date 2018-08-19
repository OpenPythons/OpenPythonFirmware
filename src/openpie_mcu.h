#ifndef OPENPIE_MCU
#define OPENPIE_MCU

typedef struct _ucp_uart_t {
    volatile uint32_t TXR;
    volatile uint32_t RXR;
} ucp_uart_t;

#define UART0 ((ucp_uart_t*)0x40000000)


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
    volatile uint32_t TICKS_US;
} rtc_t;

#define RTC ((rtc_t*)0x40000300)




#endif
