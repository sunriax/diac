
#include "adc.h"

//  +---------------------------------------------------------------+
//  |                   ADC initialization                          |
//  +---------------------------------------------------------------+
void adc_init(void)
{
    ADC0.CTRLC = (ADC_CAPACITANCE<<ADC_SAMPCAP_bp) | ADC_REFERENCE | ADC_PRESCALER;
    ADC0.CTRLD = ADC_SAMPLE_DELAY_VARIATION | ADC_INITDLY_DLY0_gc | (ADC_SAMPLE_DELAY<<ADC_SAMPDLY_gp);
    ADC0.SAMPCTRL = ADC_SAMPLE_LENGTH;
    ADC0.CTRLA = ADC_RESOLUTION | ADC_ENABLE_bm;
    
    // Check if ADC interrupt handler is active
    #ifdef ADC_ADIE
        ADC0.INTCTRL = ADC_RESRDY_bm;   // Enable ADC interrupt
    #endif
}

//  +---------------------------------------------------------------+
//  |                   ADC disable function                        |
//  +---------------------------------------------------------------+
void adc_disable(void)
{
    ADC0.CTRLA &= ~ADC_ENABLE_bm;
}

//  +---------------------------------------------------------------+
//  |                   ADC channel selection                       |
//  +---------------------------------------------------------------+
void adc_channel(ADC_Channel channel)
{
    ADC0.MUXPOS = ((0x1F & channel)<<ADC_MUXPOS_gp);   // Select ADC Channel
}

void adc_accumulation(ADC_Accumulation samples)
{
    ADC0.CTRLB = samples;
}

#ifndef ADC_ADIE

    //  +---------------------------------------------------------------+
    //  |                   ADC read value                              |
    //  +-------------------------------+-------------------------------+
    //  |             RESH              |             RESL              |
    //  +---+---+---+---+---+---+---+---+---+---+---+---+---+---+---+---+
    //  | - | - | - | - | - | - | ? | ? | ? | ? | ? | ? | ? | ? | ? | ? |
    //  +---+---+---+---+---+---+---+---+---+---+---+---+---+---+---+---+
    unsigned int adc_read(void)
    {
        ADC0.COMMAND = ADC_STARTEI_bm;

        while(ADC0.COMMAND & ADC_STARTEI_bm)
        {
            ;
        }
        
        return ADC0.RES;
    }

    //  +---------------------------------------------------------------+
    //  |                   ADC read average from values                |
    //  +---------------------------------------------------------------+
    //  | Parameter:    size    ->  quantity of conversions             |
    //  |                                                               |
    //  |    Return:    0x????  ->  Arithmetic average                  |
    //  |                           of converted data                   |
    //  +---------------------------------------------------------------+
    unsigned int adc_average(unsigned char samples)
    {
        unsigned long average = 0;
        
        for(unsigned char i=0; i < samples; i++)
        {
                average += adc_read();
        }
        
        average /= samples;
        
        return (unsigned int)(average);
    }
#endif