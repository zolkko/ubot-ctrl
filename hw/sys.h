
#ifndef __sys_h__
#define __sys_h__

#ifndef __HEAP_SIZE
#define __HEAP_SIZE  (1024 * 32)
#endif

#ifndef __STACK_SIZE
#define __STACK_SIZE (1024 * 4)
#endif

#define WEAK_DEFAULT        __attribute__((weak, alias("default_isr_handler")))
#define WEAK_NAKED_DEFAULT  __attribute__((weak, alias("default_isr_handler"), naked))
#define ISR_HANDLER_SECTION __attribute__((section(".isr_handler")))
#define HEAP_SECTION        __attribute__((section(".heap.space")))
#define STACK_SECTION       __attribute__((section(".stack.space")))

#endif

