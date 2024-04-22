
#include "input.h"

void input_init(void)
{
    INPUT_PORT.DIRCLR = INPUT_PIN_S3 | INPUT_PIN_S1 | INPUT_PIN_S2 | INPUT_PIN_S0;
    INPUT_PORT.INPUT_PIN_S0_PINCTRL = INPUT_PIN_S0_SETUP;
    INPUT_PORT.INPUT_PIN_S1_PINCTRL = INPUT_PIN_S1_SETUP;
    INPUT_PORT.INPUT_PIN_S2_PINCTRL = INPUT_PIN_S2_SETUP;
    INPUT_PORT.INPUT_PIN_S3_PINCTRL = INPUT_PIN_S3_SETUP;
}

INPUT_Status input_status(INPUT_Name name)
{
    if(!(PORTC.IN & name))
    {
        _delay_ms(INPUT_DEBOUNCE_TIME);
        return ON;
    }
    return OFF;
}