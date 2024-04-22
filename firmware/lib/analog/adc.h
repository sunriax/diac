/* -----------------------------------------
 * G.Raf^engineering
 * www.sunriax.at
 * -----------------------------------------
 *    Platform: Megacard
 *    Hardware: ATmega16
 * -----------------------------------------
 *     Version: 1.0 Release
 *      Author: G.Raf
 * Description:
 *   Header file for adc library
 * -----------------------------------------
 */

#ifndef ADC_H_
#define ADC_H_


    #ifndef ADC_RESOLUTION
        // ADC_RESSEL_10BIT_gc -> 10-Bit
        // ADC_RESSEL_8BIT_gc  -> 8-Bit
        #define ADC_RESOLUTION ADC_RESSEL_8BIT_gc
    #endif

    #ifndef ADC_CAPACITANCE
        // 0 -> Recommended for reference voltage values below 1V
        // 1 -> Recommended for higher reference voltages
        #define ADC_CAPACITANCE 1
    #endif

    #ifndef ADC_REFERENCE
        // ADC_REFSEL_INTREF_gc -> Internal reference
        // ADC_REFSEL_VDDREF_gc -> VDD reference
        #define ADC_REFERENCE ADC_REFSEL_VDDREF_gc 
    #endif

    #ifndef ADC_PRESCALER
        // !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
        // !!! The prescaler settings should be setup   !!!
        // !!! to a value that the ADC clock is between !!!
        // !!! 50 - 1500 kHz (for fully 10 bit mode)    !!!
        // !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!

        // +-----------------------------------+
        // |    Prescaler                      |
        // +---------------------+-------------+
        // | ADC_PRESC_DIV2_gc   | F_PER / 2   |
        // | ADC_PRESC_DIV4_gc   | F_PER / 4   |
        // | ADC_PRESC_DIV8_gc   | F_PER / 8   |
        // | ADC_PRESC_DIV16_gc  | F_PER / 16  |
        // | ADC_PRESC_DIV32_gc  | F_PER / 32  |
        // | ADC_PRESC_DIV64_gc  | F_PER / 64  |
        // | ADC_PRESC_DIV128_gc | F_PER / 128 |
        // | ADC_PRESC_DIV256_gc | F_PER / 256 |
        // +---------------------+-------------+
        #define ADC_PRESCALER ADC_PRESC_DIV64_gc
    
    #endif

    #ifndef ADC_INIT_DELAY
        // +-----------------------------+
        // | ADC CLK delay before conv.  |
        // +-----------------------+-----+
        // | ADC_INITDLY_DLY0_gc   |   0 |
        // | ADC_INITDLY_DLY16_gc  |  16 |
        // | ADC_INITDLY_DLY32_gc  |  32 |
        // | ADC_INITDLY_DLY64_gc  |  64 |
        // | ADC_INITDLY_DLY128_gc | 128 |
        // | ADC_INITDLY_DLY256_gc | 256 |
        // +-----------------------+-----+
        #define ADC_INIT_DELAY ADC_INITDLY_DLY0_gc
    #endif

    #ifndef ADC_SAMPLE_DELAY_VARIATION
        // ADC_ASDV_ASVOFF_gc -> OFF
        // ADC_ASDV_ASVON_gc  -> ON
        #define ADC_SAMPLE_DELAY_VARIATION ADC_ASDV_ASVOFF_gc
    #endif

    #ifndef ADC_SAMPLE_DELAY
        // Value -> 0 - 15
        #define ADC_SAMPLE_DELAY 8
    #endif

    #ifndef ADC_SAMPLE_LENGTH
        // Value -> 0 - 31
        #define ADC_SAMPLE_LENGTH 8
    #endif

    // Definition of ADC processing (Enabled = Interrupt/Disabled = Polling)

    // !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
    // !!! Interrupts are not included in this library  !!!
    // !!! The interrupt vector should be placed in the !!!
    // !!! main.c file                                  !!!
    // !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!

    // #ifndef ADC_ADIE
    //  #define ADC_ADIE
    // #endif

    #include <avr/io.h>

    enum ADC_Accumulation_t
    {
        ADC_Sample_None=ADC_SAMPNUM_ACC1_gc,
        ADC_Sample_2=ADC_SAMPNUM_ACC2_gc,
        ADC_Sample_4=ADC_SAMPNUM_ACC4_gc,
        ADC_Sample_8=ADC_SAMPNUM_ACC8_gc,
        ADC_Sample_16=ADC_SAMPNUM_ACC16_gc,
        ADC_Sample_32=ADC_SAMPNUM_ACC32_gc,
        ADC_Sample_64=ADC_SAMPNUM_ACC64_gc
    };
    typedef enum ADC_Accumulation_t ADC_Accumulation;

    enum ADC_Channel_t
    {
        ADC_CH0=ADC_MUXPOS_AIN0_gc,
        ADC_CH1=ADC_MUXPOS_AIN1_gc,
        ADC_CH2=ADC_MUXPOS_AIN2_gc,
        ADC_CH3=ADC_MUXPOS_AIN3_gc,
        ADC_CH4=ADC_MUXPOS_AIN4_gc,
        ADC_CH5=ADC_MUXPOS_AIN5_gc,
        ADC_CH6=ADC_MUXPOS_AIN6_gc,
        ADC_CH7=ADC_MUXPOS_AIN7_gc,
        ADC_CH8=ADC_MUXPOS_AIN8_gc,
        ADC_CH9=ADC_MUXPOS_AIN9_gc,
        ADC_CH10=ADC_MUXPOS_AIN10_gc,
        ADC_CH11=ADC_MUXPOS_AIN11_gc,
        ADC_INTREF=ADC_MUXPOS_INTREF_gc,
        ADC_GND=ADC_MUXPOS_GND_gc
    };
    typedef enum ADC_Channel_t ADC_Channel;

                void adc_init(void);
                void adc_channel(ADC_Channel channel);
                void adc_accumulation(ADC_Accumulation samples);
                void adc_disable(void);
            
    #ifndef ADC_ADIE
        unsigned int adc_read(void);
        unsigned int adc_average(unsigned char samples);
    #endif

#endif /* ADC_H_ */