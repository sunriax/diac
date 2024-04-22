
#ifndef SETUP_H_
#define SETUP_H_

    #ifndef SETUP_LIMIT_CYCLES
        #define SETUP_LIMIT_CYCLES 100
    #endif

    #ifndef SETUP_TABLE_WIDTH
        #define SETUP_TABLE_WIDTH 50
    #endif
    
    #include <stdlib.h>
    #include <string.h>
    #include <avr/io.h>
    
    #include "../types/types.h"
    #include "../uart/uart.h"
    #include "../input/input.h"
    #include "../clock/clock.h"
    #include "../rtc/mcp7940.h"
    #include "../sound/sound.h"
    #include "../console/console.h"

    enum SETUP_Mode_t
    {
        SETUP_Mode_None=0x00,
        SETUP_Mode_Manual=0x02,
        SETUP_Mode_Console=0x04,
        SETUP_Mode_Done=0xFF
    };
    typedef enum SETUP_Mode_t SETUP_Mode;

    enum SETUP_Update_t
    {
        SETUP_Update_None=0x00,
        SETUP_Update_Alarm=0x01,
        SETUP_Update_Alarm_Tone=0x02,
        SETUP_Update_Intensity=0x04,
        SETUP_Update_Oscillator=0x08
    };
    typedef enum SETUP_Update_t SETUP_Update;


    SETUP_Mode setup_init(void);
    void setup_menu(SETUP_Mode *mode, SETUP_Update *update);
    void setup_save(SETUP_Update update);

#endif /* SETUP_H_ */