#include <stdint.h>
#include <cmsis.h>
#include "itm.h"


inline void ITM_DEBUG(uint8_t number, uint8_t data) {
    if ((ITM_TCR & ITM_TCR_ITMENA) == 0) {
        return;
    }

    if ((ITM_TER & (1 << number)) == 0) {
        return;
    }

    do {
        while((__LDREXB(ITM_STIM8(number)) & ITM_STIM_FIFOREADY) == 0) {
            __CLREX();
        }
    } while(__STREXB(data, ITM_STIM8(number)));
}

