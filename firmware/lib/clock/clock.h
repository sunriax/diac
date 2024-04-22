

#ifndef CLOCK_H_
#define CLOCK_H_

// +--------+---------------------------------------------------------------------------------------+-----------------------------+
// | 3    0 | 60                                                                                  0 | 24                        0 |
// +--------+---------------------------------------------------------------------------------------+-----------------------------+
// | status |                                         data60                                        |           data24            |
// +-STATUS-+-MIN59-56-+-MIN55-48-+-MIN47-40-+-MIN39-32-+-MIN31-24-+-MIN23-16-+-MIN15-08-+-MIN07-00-+-HR23-16-+-HR15-08-+-HR08-00-+
// |   0xF  |    0xF   |   0xFF   |   0xFF   |   0xFF   |   0xFF   |   0xFF   |   0xFF   |   0xFF   |   0xFF  |   0xFF  |   0xFF  |
// +--------+----------+----------+----------+----------+----------+----------+----------+----------+---------+---------+---------+

    #ifndef F_CPU
        #define F_CPU 20000000UL
    #endif
    
    #ifndef CLOCK_INIT_DELAY
        #define CLOCK_INIT_DELAY 25
    #endif

    #ifndef CLOCK_CLEAR_DELAY
        #define CLOCK_CLEAR_DELAY 10
    #endif

    #ifndef CLOCK_INTENSITY_MIN
        #define CLOCK_INTENSITY_MIN 0x0FFF
    #endif

    #ifndef CLOCK_INTENSITY_MAX
        #define CLOCK_INTENSITY_MAX 0xFFFE
    #endif
	
	#ifndef CLOCK_INTENSITY_STANDARD
		#define CLOCK_INTENSITY_STANDARD 0x2222
	#endif

    #ifndef CLOCK_TIMER_MAX_VALUE
        #define CLOCK_TIMER_MAX_VALUE 0xFFFF
    #endif

    #ifndef CLOCK_TIMER_PRESCALER
        // +---------------------------------------+--------+
        // |               Possible definitions             |
        // +---------------------------------------+--------+
        // | TCA_SINGLE_CLKSEL_DIV1_gc    -> F_PER | / 1    |
        // | TCA_SINGLE_CLKSEL_DIV2_gc    -> F_PER | / 2    |
        // | TCA_SINGLE_CLKSEL_DIV4_gc    -> F_PER | / 4    |
        // | TCA_SINGLE_CLKSEL_DIV8_gc    -> F_PER | / 8    |
        // | TCA_SINGLE_CLKSEL_DIV16_gc   -> F_PER | / 16   |
        // | TCA_SINGLE_CLKSEL_DIV64_gc   -> F_PER | / 64   |
        // | TCA_SINGLE_CLKSEL_DIV256_gc  -> F_PER | / 256  |
        // | TCA_SINGLE_CLKSEL_DIV1024_gc -> F_PER | / 1024 |
        // +---------------------------------------+--------+
        #define CLOCK_TIMER_PRESCALER TCA_SINGLE_CLKSEL_DIV1_gc
    #endif

    #ifndef CLOCK_REGISTER_PORT
        #define CLOCK_REGISTER_PORT PORTA
    #endif

    #ifndef CLOCK_REGISTER_OUTPUT_PIN
        #define CLOCK_REGISTER_OUTPUT_PIN PIN6_bm
    #endif

    #ifndef CLOCK_REGISTER_CLEAR_PIN
        #define CLOCK_REGISTER_CLEAR_PIN PIN5_bm
    #endif

    #ifndef CLOCK_STATUS_LED_BLUE1
        #define CLOCK_STATUS_LED_BLUE1 0x01
    #endif
    
    #ifndef CLOCK_STATUS_LED_RED
        #define CLOCK_STATUS_LED_RED 0x02
    #endif

    #ifndef CLOCK_STATUS_LED_GREEN
        #define CLOCK_STATUS_LED_GREEN 0x04
    #endif
    
    #ifndef CLOCK_STATUS_LED_BLUE2
        #define CLOCK_STATUS_LED_BLUE2 0x08
    #endif

    #include <avr/io.h>
    #include <avr/eeprom.h>
    #include <util/delay.h>

    #include "../spi/spi.h"
    #include "../types/types.h"

    enum CLOCK_Error_t
    {
        CLOCK_None=0,
        CLOCK_Limit_Reached,
        CLOCK_Out_Of_Range
    };
    typedef enum CLOCK_Error_t CLOCK_Error;

    enum CLOCK_Status_t
    {
        CLOCK_Disable=0,
        CLOCK_Enable
    };
    typedef enum CLOCK_Status_t CLOCK_Status;

    enum CLOCK_Status_LED_t
    {
        CLOCK_LED_None=0,
        CLOCK_LED_Blue1=CLOCK_STATUS_LED_BLUE1,
        CLOCK_LED_Red=CLOCK_STATUS_LED_RED,
        CLOCK_LED_Green=CLOCK_STATUS_LED_GREEN,
        CLOCK_LED_Blue2=CLOCK_STATUS_LED_BLUE2
    };
    typedef enum CLOCK_Status_LED_t CLOCK_Status_LED;

           void clock_init(void);
           void clock_output(CLOCK_Status status);
           void clock_clear(void);
   unsigned int clock_intensity(void);
    CLOCK_Error clock_setintensity(unsigned int intensity);
           void clock_copy_intensity(Copy_Mode mode);
    CLOCK_Error clock_settime(unsigned char hour, unsigned char minute, unsigned char second, CLOCK_Status_LED status);
    CLOCK_Error clock_setdate(unsigned char day, unsigned char month, CLOCK_Status_LED status);
           void clock_setdata(unsigned long data24, unsigned long long data60, CLOCK_Status_LED status);

    void clock_saveintensity(void);

#endif /* CLOCK_H_ */