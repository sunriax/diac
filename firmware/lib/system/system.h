
#ifndef SYSTEM_H_
#define SYSTEM_H_

    #ifndef SYSTEM_CLOCK
    // +-----------------------------+--------------------+------+--------+
    // |         SYSTEM_CLOCK        |  SYSTEM_CLOCK_BIT  | Type | Freq.  |
    // +-----------------------------+--------------------+------+--------+
    // | CLKCTRL_CLKSEL_OSC20M_gc    | CLKCTRL_OSC20MS_bm | Int. | 20 MHz |
    // | CLKCTRL_CLKSEL_OSCULP32K_gc | CLKCTRL_OSC32KS_bm | Int. | 32 kHz |
    // | CLKCTRL_CLKSEL_EXTCLK_gc    | CLKCTRL_EXTS_bm    |Ext. | ??     |
    // +-----------------------------+--------------------+------+--------+
      #define SYSTEM_CLOCK CLKCTRL_CLKSEL_OSC20M_gc
      #define SYSTEM_CLOCK_BIT CLKCTRL_OSC20MS_bm
    #endif

    #ifndef SYSTEM_PER_CLOCK_PRESCALER
        // +------------------------------+------+
        // |      Possible definitions    |      |
        // +------------------------------+------+
        // | // undefine         -> F_CPU | / 1  |
        // | CLKCTRL_PDIV_2X_gc  -> F_CPU | / 2  |
        // | CLKCTRL_PDIV_4X_gc  -> F_CPU | / 4  |
        // | CLKCTRL_PDIV_8X_gc  -> F_CPU | / 8  |
        // | CLKCTRL_PDIV_16X_gc -> F_CPU | / 16 |
        // | CLKCTRL_PDIV_32X_gc -> F_CPU | / 32 |
        // | CLKCTRL_PDIV_64X_gc -> F_CPU | / 64 |
        // | CLKCTRL_PDIV_6X_gc  -> F_CPU | / 6  |
        // | CLKCTRL_PDIV_10X_gc -> F_CPU | / 10 |
        // | CLKCTRL_PDIV_24X_gc -> F_CPU | / 24 |
        // | CLKCTRL_PDIV_48X_gc -> F_CPU | / 48 |
        // +------------------------------+------+
        #define SYSTEM_PER_CLOCK_PRESCALER CLKCTRL_PDIV_2X_gc
    #endif

    #include <avr/io.h>

    void system_init(void);

#endif /* SYSTEM_H_ */