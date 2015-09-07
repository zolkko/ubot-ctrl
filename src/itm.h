#ifndef __itm_h__
#define __itm_h__

#ifdef __cplusplus
extern "C" {
#endif

#define ITM_HWREG(x)                    (*((volatile uint32_t *)(x)))
#define ITM_HWREGH(x)                   (*((volatile uint16_t *)(x)))
#define ITM_HWREGB(x)                   (*((volatile uint8_t *)(x)))


/* 
 * ITM_STIM
 *
 * Bits 31:0 - Write to port FIFO for forwarding as software event packet
 * Bits 31:1 - RAZ
 */
#define ITM_STIM8(n)                    ((volatile uint8_t  *)(ITM_BASE + (n * 4)))
#define ITM_STIM16(n)                   ((volatile uint16_t *)(ITM_BASE + (n * 4)))
#define ITM_STIM32(n)                   ((volatile uint32_t *)(ITM_BASE + (n * 4)))
#define ITM_STIM_FIFOREADY              (1 << 0)

/*
 * Trace Enable ports (ITM_TER[x])
 *
 * Bits 31:0 - Stimulus port #N is enabled with STIMENA[N] is set
 */
#define ITM_TER                         (ITM_HWREG(ITM_BASE + 0xE00))

/* 
 * Trace Control (ITM_TCR)
 *
 * Bits 31:0 - Bit [N] of PRIVMASK controls stimulus ports 8N to 8N+7
 * 0: User access allowed to stimulus ports
 * 1: Privileged access only to stimulus ports
 */
#define ITM_TCR                         ITM_HWREG(ITM_BASE + 0xE80)

/*
 * Trace Privilege (ITM_TPR)
 *
 * Bits 31:24 - Reserved
 */
#define ITM_TPR                         ITM_HWREG(ITM_BASE + 0xE40)

#define ITM_TCR_BUSY                    (1 << 23)
#define ITM_TCR_TRACE_BUS_ID_MASK       (0x3f << 16)

/* Bits 15:10 - Reserved */
#define ITM_TCR_TSPRESCALE_NONE         (0 << 8)
#define ITM_TCR_TSPRESCALE_DIV4         (1 << 8)
#define ITM_TCR_TSPRESCALE_DIV16        (2 << 8)
#define ITM_TCR_TSPRESCALE_DIV64        (3 << 8)
#define ITM_TCR_TSPRESCALE_MASK         (3 << 8)

/* Bits 7:5 - Reserved */
#define ITM_TCR_SWOENA                  (1 << 4)
#define ITM_TCR_TXENA                   (1 << 3)
#define ITM_TCR_SYNCENA                 (1 << 2)
#define ITM_TCR_TSENA                   (1 << 1)
#define ITM_TCR_ITMENA                  (1 << 0)

/*
 * CoreSight Lock Status Register for this peripheral
 */
#define ITM_LSR                         ITM_HWREG(ITM_BASE + 0xFB4)
#define ITM_LSR_LOCKED                  (1 << 1)
#define ITM_LSR_IMP                     (1 << 0)

/*
 * CoreSight Lock Access Register for this peripheral
 *
 * A write of 0xC5ACCE55 enables further write access to this component.
 */
#define ITM_LAR                         ITM_HWREG(ITM_BASE + 0xFB0)
#define ITM_LAR_EN                      (0xC5ACCE55)

/*
 * CoreSight Device ID register.
 */
#define ITM_DEVID                       ITM_HWREG(ITM_BASE + 0xFC8)
#define ITM_DEVID_UART_NRZ              (1 << 11)
#define ITM_DEVID_MANCH                 (1 << 10)
#define ITM_DEVID_SYNCH                 (1 << 9)
#define ITM_DEVID_FIFO_SIZE             ((1 << 8) | (1 << 7) | (1 << 6))
#define ITM_DEVID_FIFO_SIZE_bm          (3)
#define ITM_DEVID_FIFO_SIZE_bp          (6)


void ITM_DEBUG(uint8_t n, uint8_t c);


#ifdef __cplusplus
}
#endif

#endif

