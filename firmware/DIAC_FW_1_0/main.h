
#ifndef MAIN_H_
#define MAIN_H_

    #ifndef EXTERNAL_INTERRUPT_TRIGGER_PORT
        #define EXTERNAL_INTERRUPT_TRIGGER_PORT PORTA
    #endif

    #ifndef EXTERNAL_INTERRUPT_TRIGGER_PIN
        #define EXTERNAL_INTERRUPT_TRIGGER_PIN       PIN7_bm
        #define EXTERNAL_INTERRUPT_TRIGGER_PINCTRL   PIN7CTRL
        #define EXTERNAL_INTERRUPT_TRIGGER_PIN_SETUP PORT_ISC_FALLING_gc | PORT_PULLUPEN_bm
    #endif

    #include <avr/io.h>
    #include <avr/interrupt.h>

    #include "../lib/system/system.h"
    #include "../lib/input/input.h"
    #include "../lib/uart/uart.h"
    #include "../lib/alarm/alarm.h"
    #include "../lib/sound/sound.h"
    #include "../lib/clock/clock.h"
    #include "../lib/rtc/mcp7940.h"
    #include "../lib/sensor/sensor.h"
    #include "../lib/console/console.h"
    #include "../lib/setup/setup.h"

#endif /* MAIN_H_ */