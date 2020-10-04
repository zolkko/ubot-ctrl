use cortex_m::asm;
use cortex_m_rt::entry;
use rtt_target::rprintln;


fn exit() -> ! {
    loop { asm::bkpt() }
}


#[entry]
fn main() -> ! {
    // omitted: rtt initialization
    rprintln!("Hello, world!");
    exit();
}

