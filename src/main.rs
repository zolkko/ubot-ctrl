#![no_main]
#![no_std]

extern crate stm32f4xx_hal;

use defmt_rtt as _; // global logger
use stm32f4xx_hal as _; // memory layout

use cortex_m::asm;
use cortex_m_rt::entry;

#[panic_handler]
fn panic_handler(_info: &core::panic::PanicInfo) -> ! {
    defmt::error!("panicked");
    exit();
}

fn exit() -> ! {
    loop {
        asm::bkpt();
    }
}

#[entry]
fn main() -> ! {
    defmt::info!("Hello, World!");
    exit();
}
