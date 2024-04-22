
#ifndef INPUT_H_
#define INPUT_H_

    #ifndef F_CPU
        #define F_CPU 20000000UL
    #endif

    #ifndef INPUT_DEBOUNCE_TIME
        #define INPUT_DEBOUNCE_TIME 10
    #endif

    #ifndef INPUT_PORT
        #define INPUT_PORT PORTC
    #endif

    #ifndef INPUT_PIN_S0
        #define INPUT_PIN_S0         PIN0_bm
        #define INPUT_PIN_S0_PINCTRL PIN0CTRL
        #define INPUT_PIN_S0_SETUP   PORT_PULLUPEN_bm
    #endif
    
    #ifndef INPUT_PIN_S1
        #define INPUT_PIN_S1         PIN1_bm
        #define INPUT_PIN_S1_PINCTRL PIN1CTRL
        #define INPUT_PIN_S1_SETUP   PORT_PULLUPEN_bm
    #endif

    #ifndef INPUT_PIN_S2
        #define INPUT_PIN_S2         PIN2_bm
        #define INPUT_PIN_S2_PINCTRL PIN2CTRL
        #define INPUT_PIN_S2_SETUP   PORT_PULLUPEN_bm
    #endif

    #ifndef INPUT_PIN_S3
        #define INPUT_PIN_S3         PIN3_bm
        #define INPUT_PIN_S3_PINCTRL PIN3CTRL
        #define INPUT_PIN_S3_SETUP   PORT_PULLUPEN_bm
    #endif

    #include <avr/io.h>
    #include <util/delay.h>

    enum INPUT_Name_t
    {
        INPUT_SW1=INPUT_PIN_S0,
        INPUT_SW2=INPUT_PIN_S1,
        INPUT_SW3=INPUT_PIN_S2,
        INPUT_SW4=INPUT_PIN_S3
    };
    typedef enum INPUT_Name_t INPUT_Name;

    enum INPUT_Status_t
    {
        OFF=0,
        ON
    };
    typedef enum INPUT_Status_t INPUT_Status;

    void input_init(void);
    INPUT_Status input_status(INPUT_Name name);

#endif /* INPUT_H_ */