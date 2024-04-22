
#include "clock.h"

unsigned int EEMEM ee_clock_intensity = CLOCK_INTENSITY_STANDARD;
unsigned int clock_current_intensity;

void clock_init(void)
{
    CLOCK_REGISTER_PORT.DIRSET = CLOCK_REGISTER_OUTPUT_PIN | CLOCK_REGISTER_CLEAR_PIN;
    CLOCK_REGISTER_PORT.OUTSET = CLOCK_REGISTER_CLEAR_PIN;
    
    spi_init(SPI_MSB, SPI_Rising, SPI_Rising);

    clock_copy_intensity(ToBUFFER);

    TCA0.SINGLE.PER = CLOCK_TIMER_MAX_VALUE;
    TCA0.SINGLE.CMP0 = clock_current_intensity;
    TCA0.SINGLE.INTCTRL = TCA_SINGLE_OVF_bm | TCA_SINGLE_CMP0_bm;
    TCA0.SINGLE.CTRLB = TCA_SINGLE_WGMODE_SINGLESLOPE_gc;
    TCA0.SINGLE.CTRLA = CLOCK_TIMER_PRESCALER | TCA_SINGLE_ENABLE_bm;
    
    for (unsigned int i=0; i < 128; i++)
    {
        clock_settime((i%24), i%60, i%60, (i%4));
        _delay_ms(CLOCK_INIT_DELAY);
    }
    clock_setdata(0UL, 0UL, 0);
}

void clock_output(CLOCK_Status status)
{
    if(status == CLOCK_Enable)
    {
        CLOCK_REGISTER_PORT.OUTSET = CLOCK_REGISTER_OUTPUT_PIN;
        return;
    }
    CLOCK_REGISTER_PORT.OUTCLR = CLOCK_REGISTER_OUTPUT_PIN;
}

void clock_clear(void)
{
    CLOCK_REGISTER_PORT.OUTCLR = CLOCK_REGISTER_CLEAR_PIN;
    _delay_ms(CLOCK_CLEAR_DELAY);
    CLOCK_REGISTER_PORT.OUTSET = CLOCK_REGISTER_CLEAR_PIN;
}

unsigned int clock_intensity(void)
{
    return clock_current_intensity;
}

CLOCK_Error clock_setintensity(unsigned int intensity)
{
    if(intensity <= CLOCK_INTENSITY_MIN || intensity >= CLOCK_INTENSITY_MAX)
    {
        return CLOCK_Limit_Reached;
    }

    clock_current_intensity = intensity;
    TCA0.SINGLE.CMP0 = intensity;

    return CLOCK_None;
}

void clock_copy_intensity(Copy_Mode mode)
{
    switch (mode)
    {
        case ToEEPROM:
            eeprom_write_word(&ee_clock_intensity, clock_current_intensity);
        break;
        default:
            clock_current_intensity = eeprom_read_word(&ee_clock_intensity);
        break; 
    }
}

CLOCK_Error clock_settime(unsigned char hour, unsigned char minute, unsigned char second, CLOCK_Status_LED status)
{
    if((hour > 23) || (minute > 59) || (second > 59))
    {
        return CLOCK_Out_Of_Range;
    }
    
    unsigned long data24 = (1UL<<hour);
    unsigned long long data60 = (1ULL<<minute) | (1ULL<<second);

    clock_setdata(data24, data60, status);

    return CLOCK_None;
}

CLOCK_Error clock_setdate(unsigned char day, unsigned char month, CLOCK_Status_LED status)
{
    if((day < 1) || (day > 31) || (month < 1) || (month > 12))
    {
        return CLOCK_Out_Of_Range;
    }
    
    unsigned long data24 = 0UL;

    for (unsigned char i=0; i < month; i++)
    {
         data24 |= (1UL<<i);
    }

    unsigned long long data60 = 0ULL;

    for (unsigned char i=0; i < day; i++)
    {
        data60 |= (1UL<<i);
    }

    clock_setdata(data24, data60, status);

    return CLOCK_None;
}

void clock_setdata(unsigned long data24, unsigned long long data60, CLOCK_Status_LED status)
{
    spi_select(SPI_Enable);
    spi_transfer((status<<4) | (0x0F & (data60>>56)));
    
    for (signed char i=6; i >= 0; i--)
    {
        spi_transfer((data60>>(i * 8UL)));
    }

    for (signed char i=2; i >= 0; i--)
    {
        spi_transfer((data24>>(i * 8UL)));
    }
    spi_select(SPI_Disable);
}