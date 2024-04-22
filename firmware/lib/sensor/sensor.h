
#ifndef SENSOR_H_
#define SENSOR_H_

    #ifndef SENSOR_PORT
        #define SENSOR_PORT PORTB
    #endif

    #ifndef SENSOR_PIN
        #define SENSOR_PIN PIN5_bm
    #endif

    #ifndef SENSOR_CHANNEL
        #define SENSOR_CHANNEL 0x08
    #endif

    #ifndef SENSOR_CALCULATION
        #define SENSOR_CALCULATION(DATA) ((100UL * DATA - 15500) / 869)
    #endif

    #include <avr/io.h>
   
    #include "../analog/adc.h"
   
    void sensor_init(void);
    int sensor_value(void);

#endif /* SENSOR_H_ */