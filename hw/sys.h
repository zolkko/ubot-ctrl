
#ifndef __sys_h__
#define __sys_h__


#define WEAK_DEFAULT __attribute__((weak, alias("default_isr_handler")))
#define WEAK_NAKED_DEFAULT __attribute__((weak, alias("default_isr_handler"), naked))

#define ISR_HANDLER_SECTION __attribute__ ((section(".isr_handler")))

void sys_init(void);

#endif
