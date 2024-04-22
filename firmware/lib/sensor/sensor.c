
#include "sensor.h"

void sensor_init(void)
{
    SENSOR_PORT.DIRCLR = SENSOR_PIN;

    adc_init();
    adc_channel(SENSOR_CHANNEL);
}

int sensor_value(void)
{
    return SENSOR_CALCULATION(adc_read());
}