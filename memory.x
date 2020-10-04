MEMORY
{ 
    FLASH (rx)  : ORIGIN = 0x08000000, LENGTH = 1024K
    CCM   (rwx) : ORIGIN = 0x10000000, LENGTH = 64K
    /* Reserve SRAM for interrupt vectors */
    VRAM  (rwx) : ORIGIN = 0x20000000, LENGTH = 0x188
    RAM   (rwx) : ORIGIN = 0x20000188, LENGTH = 128k - 0x188
}

