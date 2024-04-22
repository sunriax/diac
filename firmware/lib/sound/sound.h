

#ifndef SOUND_H_
#define SOUND_H_

    #ifndef F_CPU
        #define F_CPU 20000000UL
    #endif

    #ifndef F_PER
        #define F_PER 10000000UL
    #endif
	
    #ifndef SOUND_DELAY_TIME
        #define SOUND_DELAY_TIME 50
    #endif

    #ifndef SOUND_MIN_FREQUENCY
        #define SOUND_MIN_FREQUENCY 100
    #endif

    #ifndef SOUND_MAX_FREQUENCY
        #define SOUND_MAX_FREQUENCY 15000U
    #endif

    #ifndef SOUND_FREQUENCIES_MAX_LENGTH
        #define SOUND_FREQUENCIES_MAX_LENGTH 30
    #endif

    #ifndef SOUND_PORT
        #define SOUND_PORT PORTB
    #endif

    #ifndef SOUND_PIN
        #define SOUND_PIN PIN4_bm
    #endif

    #define SOUND_HERTZ2VALUE(VALUE) ((unsigned int)(F_PER/(float)(2UL * VALUE)))

    #include <avr/io.h>
    #include <avr/eeprom.h>
    #include <util/delay.h>

    #include "../types/types.h"

    enum SOUND_Error_t
    {
        SOUND_None=0,
        SOUND_Range
    };
    typedef enum SOUND_Error_t SOUND_Error;

    enum SOUND_Status_t
    {
        SOUND_OFF=0,
        SOUND_ON
    };
    typedef enum SOUND_Status_t SOUND_Status;

           void sound_init();
           void sound_play(SOUND_Status status);
           void sound_toggle_pin(void);

    SOUND_Error sound_copy(Copy_Mode mode);
    SOUND_Error sound_create(unsigned char position, unsigned char hertz);
           void sound_save(void);

#endif /* SOUND_H_ */