
#include <stdint.h>
#include <stdlib.h>
#include <cmsis.h>
#include "sys.h"


/* Address defined in LD script */
extern uint32_t __StackTop;

extern uint32_t __data_load__;
extern uint32_t __data_start__;
extern uint32_t __data_end__;
extern uint32_t __bss_start__;
extern uint32_t __bss_end__;

/* Defined in newlibc-nano */
extern void _mainCRTStartup(void);

/* Forward declaration of the default fault handlers. */
static void default_isr_handler(void);

void reset_isr_handler(void);

void nmi_isr_handler(void) WEAK_DEFAULT;
void hard_fault_isr_handler(void) WEAK_DEFAULT;
void mpu_fault_isr_handler(void) WEAK_DEFAULT;
void bus_fault_isr_handler(void) WEAK_DEFAULT;
void usage_fault_isr_handler(void) WEAK_DEFAULT;
void debug_monitor_isr_handler(void) WEAK_DEFAULT;

void SysTick_Handler(void) WEAK_DEFAULT;
void SVC_Handler(void) WEAK_NAKED_DEFAULT;
void PendSV_Handler(void) WEAK_NAKED_DEFAULT;

void ssi1_isr_handler(void) WEAK_DEFAULT;
void ssi2_isr_handler(void) WEAK_DEFAULT;
void ssi3_isr_handler(void) WEAK_DEFAULT;


/* The vector table. */
__attribute__ ((section(".isr_vector")))
void (* const g_pfnVectors[])(void) = {
    (void (*)(void))(&__StackTop),  // The initial stack pointer
    reset_isr_handler,              // The reset handler
    nmi_isr_handler,                // The NMI handler
    hard_fault_isr_handler,         // The hard fault handler
    mpu_fault_isr_handler,          // The MPU fault handler
    bus_fault_isr_handler,          // The bus fault handler
    usage_fault_isr_handler,        // The usage fault handler
    0,                              // Reserved
    0,                              // Reserved
    0,                              // Reserved
    0,                              // Reserved
    SVC_Handler,                    // SVCall handler
    debug_monitor_isr_handler,      // Debug monitor handler
    0,                              // Reserved
    PendSV_Handler,                 // The PendSV handler
    SysTick_Handler,                // The SysTick handler
    default_isr_handler,            // GPIO Port A
    default_isr_handler,            // GPIO Port B
    default_isr_handler,            // GPIO Port C
    default_isr_handler,            // GPIO Port D
    default_isr_handler,            // GPIO Port E
    default_isr_handler,            // UART0 Rx and Tx
    default_isr_handler,            // UART1 Rx and Tx
    default_isr_handler,            // SSI0 Rx and Tx
    default_isr_handler,            // I2C0 Master and Slave
    default_isr_handler,            // PWM Fault
    default_isr_handler,            // PWM Generator 0
    default_isr_handler,            // PWM Generator 1
    default_isr_handler,            // PWM Generator 2
    default_isr_handler,            // Quadrature Encoder 0
    default_isr_handler,            // ADC Sequence 0
    default_isr_handler,            // ADC Sequence 1
    default_isr_handler,            // ADC Sequence 2
    default_isr_handler,            // ADC Sequence 3
    default_isr_handler,            // Watchdog timer
    default_isr_handler,            // Timer 0 subtimer A
    default_isr_handler,            // Timer 0 subtimer B
    default_isr_handler,            // Timer 1 subtimer A
    default_isr_handler,            // Timer 1 subtimer B
    default_isr_handler,            // Timer 2 subtimer A
    default_isr_handler,            // Timer 2 subtimer B
    default_isr_handler,            // Analog Comparator 0
    default_isr_handler,            // Analog Comparator 1
    default_isr_handler,            // Analog Comparator 2
    default_isr_handler,            // System Control (PLL, OSC, BO)
    default_isr_handler,            // FLASH Control
    default_isr_handler,            // GPIO Port F
    default_isr_handler,            // GPIO Port G
    default_isr_handler,            // GPIO Port H
    default_isr_handler,            // UART2 Rx and Tx
    ssi1_isr_handler,               // SSI1 Rx and Tx
    default_isr_handler,            // Timer 3 subtimer A
    default_isr_handler,            // Timer 3 subtimer B
    default_isr_handler,            // I2C1 Master and Slave
    default_isr_handler,            // Quadrature Encoder 1
    default_isr_handler,            // CAN0
    default_isr_handler,            // CAN1
    0,                              // Reserved
    0,                              // Reserved
    default_isr_handler,            // Hibernate
    default_isr_handler,            // USB0
    default_isr_handler,            // PWM Generator 3
    default_isr_handler,            // uDMA Software Transfer
    default_isr_handler,            // uDMA Error
    default_isr_handler,            // ADC1 Sequence 0
    default_isr_handler,            // ADC1 Sequence 1
    default_isr_handler,            // ADC1 Sequence 2
    default_isr_handler,            // ADC1 Sequence 3
    0,                              // Reserved
    0,                              // Reserved
    default_isr_handler,            // GPIO Port J
    default_isr_handler,            // GPIO Port K
    default_isr_handler,            // GPIO Port L
    ssi2_isr_handler,               // SSI2 Rx and Tx
    ssi3_isr_handler,               // SSI3 Rx and Tx
    default_isr_handler,            // UART3 Rx and Tx
    default_isr_handler,            // UART4 Rx and Tx
    default_isr_handler,            // UART5 Rx and Tx
    default_isr_handler,            // UART6 Rx and Tx
    default_isr_handler,            // UART7 Rx and Tx
    0,                              // Reserved
    0,                              // Reserved
    0,                              // Reserved
    0,                              // Reserved
    default_isr_handler,            // I2C2 Master and Slave
    default_isr_handler,            // I2C3 Master and Slave
    default_isr_handler,            // Timer 4 subtimer A
    default_isr_handler,            // Timer 4 subtimer B
    0,                              // Reserved
    0,                              // Reserved
    0,                              // Reserved
    0,                              // Reserved
    0,                              // Reserved
    0,                              // Reserved
    0,                              // Reserved
    0,                              // Reserved
    0,                              // Reserved
    0,                              // Reserved
    0,                              // Reserved
    0,                              // Reserved
    0,                              // Reserved
    0,                              // Reserved
    0,                              // Reserved
    0,                              // Reserved
    0,                              // Reserved
    0,                              // Reserved
    0,                              // Reserved
    0,                              // Reserved
    default_isr_handler,            // Timer 5 subtimer A
    default_isr_handler,            // Timer 5 subtimer B
    default_isr_handler,            // Wide Timer 0 subtimer A
    default_isr_handler,            // Wide Timer 0 subtimer B
    default_isr_handler,            // Wide Timer 1 subtimer A
    default_isr_handler,            // Wide Timer 1 subtimer B
    default_isr_handler,            // Wide Timer 2 subtimer A
    default_isr_handler,            // Wide Timer 2 subtimer B
    default_isr_handler,            // Wide Timer 3 subtimer A
    default_isr_handler,            // Wide Timer 3 subtimer B
    default_isr_handler,            // Wide Timer 4 subtimer A
    default_isr_handler,            // Wide Timer 4 subtimer B
    default_isr_handler,            // Wide Timer 5 subtimer A
    default_isr_handler,            // Wide Timer 5 subtimer B
    default_isr_handler,            // FPU
    0,                              // Reserved
    0,                              // Reserved
    default_isr_handler,            // I2C4 Master and Slave
    default_isr_handler,            // I2C5 Master and Slave
    default_isr_handler,            // GPIO Port M
    default_isr_handler,            // GPIO Port N
    default_isr_handler,            // Quadrature Encoder 2
    0,                              // Reserved
    0,                              // Reserved
    default_isr_handler,            // GPIO Port P (Summary or P0)
    default_isr_handler,            // GPIO Port P1
    default_isr_handler,            // GPIO Port P2
    default_isr_handler,            // GPIO Port P3
    default_isr_handler,            // GPIO Port P4
    default_isr_handler,            // GPIO Port P5
    default_isr_handler,            // GPIO Port P6
    default_isr_handler,            // GPIO Port P7
    default_isr_handler,            // GPIO Port Q (Summary or Q0)
    default_isr_handler,            // GPIO Port Q1
    default_isr_handler,            // GPIO Port Q2
    default_isr_handler,            // GPIO Port Q3
    default_isr_handler,            // GPIO Port Q4
    default_isr_handler,            // GPIO Port Q5
    default_isr_handler,            // GPIO Port Q6
    default_isr_handler,            // GPIO Port Q7
    default_isr_handler,            // GPIO Port R
    default_isr_handler,            // GPIO Port S
    default_isr_handler,            // PWM 1 Generator 0
    default_isr_handler,            // PWM 1 Generator 1
    default_isr_handler,            // PWM 1 Generator 2
    default_isr_handler,            // PWM 1 Generator 3
    default_isr_handler             // PWM 1 Fault
};


ISR_HANDLER_SECTION
void reset_isr_handler(void)
{
    /* Copy the data segment initializers from FLASH to SRAM. */
    volatile uint32_t * data_load_addr = &__data_load__;
    volatile uint32_t * data_sram_addr = &__data_start__;

    for ( ; data_sram_addr < &__data_end__; ) {
        *data_sram_addr++ = *data_load_addr++;
    }

    /* Zero fill the bss segment. */
    __asm("    ldr     r0, =__bss_start__\n"
          "    ldr     r1, =__bss_end__\n"
          "    mov     r2, #0\n"
          "    .thumb_func\n"
          "zero_loop:\n"
          "    cmp     r0, r1\n"
          "    it      lt\n"
          "    strlt   r2, [r0], #4\n"
          "    blt     zero_loop");

    SystemInit();

    _mainCRTStartup();
}


ISR_HANDLER_SECTION
static void default_isr_handler(void)
{
    while (1) {
    }
}

