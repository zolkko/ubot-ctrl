
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

HEAP_SECTION
uint8_t heap_space[__HEAP_SIZE];

STACK_SECTION
uint8_t stack_space[__STACK_SIZE];

/* Defined in newlibc-nano. the alias of _start */
extern void _mainCRTStartup(void);

/* Forward declaration of the default fault handlers. */
static void default_isr_handler(void);

/* weak declarations of ISR handlers. */
void reset_isr_handler(void);

void nmi_isr_handler(void) WEAK_DEFAULT;
void hard_fault_isr_handler(void) WEAK_DEFAULT;
void mpu_fault_isr_handler(void) WEAK_DEFAULT;
void bus_fault_isr_handler(void) WEAK_DEFAULT;
void usage_fault_isr_handler(void) WEAK_DEFAULT;
void debug_monitor_isr_handler(void) WEAK_DEFAULT;

void SVC_Handler(void) WEAK_NAKED_DEFAULT;
void PendSV_Handler(void) WEAK_NAKED_DEFAULT;
void SysTick_Handler(void) WEAK_DEFAULT;

void spi1_isr_handler(void) WEAK_DEFAULT;
void spi2_isr_handler(void) WEAK_DEFAULT;
void spi3_isr_handler(void) WEAK_DEFAULT;


/* The vector table. */
__attribute__ ((section(".isr_vector")))
void (* const isr_vector_table[])(void) = {
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

    /* stm32f4 specific interrupts */

    default_isr_handler,            // Window WatchDog
	default_isr_handler,            // PVD through EXTI Line detection
	default_isr_handler,            // Tamper and TimeStamps through the EXTI line
	default_isr_handler,            // RTC Wakeup through the EXTI line
	default_isr_handler,            // FLASH
	default_isr_handler,            // RCC
	default_isr_handler,            // EXTI Line0
	default_isr_handler,            // EXTI Line1
	default_isr_handler,            // EXTI Line2
	default_isr_handler,            // EXTI Line3
	default_isr_handler,            // EXTI Line4
	default_isr_handler,            // DMA1 Stream 0
	default_isr_handler,            // DMA1 Stream 1
	default_isr_handler,            // DMA1 Stream 2
	default_isr_handler,            // DMA1 Stream 3
	default_isr_handler,            // DMA1 Stream 4
	default_isr_handler,            // DMA1 Stream 5
	default_isr_handler,            // DMA1 Stream 6
	default_isr_handler,            // ADC1, ADC2 and ADC3s
	default_isr_handler,            // CAN1 TX
	default_isr_handler,            // CAN1 RX0
	default_isr_handler,            // CAN1 RX1
	default_isr_handler,            // CAN1 SCE
	default_isr_handler,            // External Line[9:5]s
	default_isr_handler,            // TIM1 Break and TIM9
	default_isr_handler,            // TIM1 Update and TIM10
	default_isr_handler,            // TIM1 Trigger and Commutation and TIM11
	default_isr_handler,            // TIM1 Capture Compare
	default_isr_handler,            // TIM2
	default_isr_handler,            // TIM3
	default_isr_handler,            // TIM4
	default_isr_handler,            // I2C1 Event
	default_isr_handler,            // I2C1 Error
	default_isr_handler,            // I2C2 Event
	default_isr_handler,            // I2C2 Error
	spi1_isr_handler,               // SPI1
	spi2_isr_handler,               // SPI2
	default_isr_handler,            // USART1
	default_isr_handler,            // USART2
	default_isr_handler,            // USART3
	default_isr_handler,            // External Line[15:10]s
	default_isr_handler,            // RTC Alarm (A and B) through EXTI Line
	default_isr_handler,            // USB OTG FS Wakeup through EXTI line
	default_isr_handler,            // TIM8 Break and TIM12
	default_isr_handler,            // TIM8 Update and TIM13
	default_isr_handler,            // TIM8 Trigger and Commutation and TIM14
	default_isr_handler,            // TIM8 Capture Compare
	default_isr_handler,            // DMA1 Stream7
	default_isr_handler,            // FSMC
	default_isr_handler,            // SDIO
	default_isr_handler,            // TIM5
	spi3_isr_handler,               // SPI3
	default_isr_handler,            // UART4
	default_isr_handler,            // UART5
	default_isr_handler,            // TIM6 and DAC1&2 underrun errors
	default_isr_handler,            // TIM7
	default_isr_handler,            // DMA2 Stream 0
	default_isr_handler,            // DMA2 Stream 1
	default_isr_handler,            // DMA2 Stream 2
	default_isr_handler,            // DMA2 Stream 3
	default_isr_handler,            // DMA2 Stream 4
	default_isr_handler,            // Ethernet
	default_isr_handler,            // Ethernet Wakeup through EXTI line
	default_isr_handler,            // CAN2 TX
	default_isr_handler,            // CAN2 RX0
	default_isr_handler,            // CAN2 RX1
	default_isr_handler,            // CAN2 SCE
	default_isr_handler,            // USB OTG FS
	default_isr_handler,            // DMA2 Stream 5
	default_isr_handler,            // DMA2 Stream 6
	default_isr_handler,            // DMA2 Stream 7
	default_isr_handler,            // USART6
	default_isr_handler,            // I2C3 event
	default_isr_handler,            // I2C3 error
	default_isr_handler,            // USB OTG HS End Point 1 Out
	default_isr_handler,            // USB OTG HS End Point 1 In
	default_isr_handler,            // USB OTG HS Wakeup through EXTI
	default_isr_handler,            // USB OTG HS
	default_isr_handler,            // DCMI
	default_isr_handler,            // CRYP crypto
	default_isr_handler,            // Hash and Rng
	default_isr_handler,            // FPU
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

    /* Call the clock system intitialization function.
     * Provided by cmsis sdk. (system_stm32f4xx.c)
     *
     * NOTE: In order to maintain SystemCoreClock variable in sync
     * the SystemCoreClockUpdate function must be called each time
     * the core clock is changed.
     */
    SystemInit();

    /* Calling the crt0 'cold-start' entry point. There __libc_init_array is called
     * and when existing hardware_init_hook() and software_init_hook() before
     * starting main(). software_init_hook() is available and has to be called due
     * to initializsation when using rtos.
     */
    _mainCRTStartup();
}


ISR_HANDLER_SECTION
static void default_isr_handler(void)
{
    while (1) {
    }
}

