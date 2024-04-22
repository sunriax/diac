
#include "sound.h"

unsigned int EEMEM ee_sound_tone[SOUND_FREQUENCIES_MAX_LENGTH] = {
    SOUND_HERTZ2VALUE(900),
    SOUND_HERTZ2VALUE(2500),
    SOUND_HERTZ2VALUE(450),
    SOUND_HERTZ2VALUE(12000),
    SOUND_HERTZ2VALUE(500),
    SOUND_HERTZ2VALUE(2500),
    SOUND_HERTZ2VALUE(250),
    SOUND_HERTZ2VALUE(5500),
    SOUND_HERTZ2VALUE(1050),
    SOUND_HERTZ2VALUE(2450),
    SOUND_HERTZ2VALUE(230),
    SOUND_HERTZ2VALUE(7000),
    SOUND_HERTZ2VALUE(900),
    SOUND_HERTZ2VALUE(2500),
    SOUND_HERTZ2VALUE(450),
    SOUND_HERTZ2VALUE(14000),
    SOUND_HERTZ2VALUE(1200),
    SOUND_HERTZ2VALUE(120),
    SOUND_HERTZ2VALUE(1150),
    SOUND_HERTZ2VALUE(8500),
    SOUND_HERTZ2VALUE(5050),
    SOUND_HERTZ2VALUE(2450),
    SOUND_HERTZ2VALUE(3430),
    SOUND_HERTZ2VALUE(600),
    SOUND_HERTZ2VALUE(1100),
    SOUND_HERTZ2VALUE(200),
    SOUND_HERTZ2VALUE(2550),
    SOUND_HERTZ2VALUE(14000),
    SOUND_HERTZ2VALUE(1000),
    SOUND_HERTZ2VALUE(150)
};
unsigned char EEMEM ee_sound_length = sizeof(ee_sound_tone)/sizeof(ee_sound_tone[0]);

unsigned int sound_tone[sizeof(ee_sound_tone)/sizeof(ee_sound_tone[0])];
unsigned char sound_length;

static unsigned int sound_hz2value(unsigned int hertz)
{
    return (F_PER/(hertz<<1));
}

void sound_init()
{
    SOUND_PORT.DIRSET = SOUND_PIN;
    SOUND_PORT.OUTCLR = SOUND_PIN;
    
    sound_length = eeprom_read_byte(&ee_sound_length);
    sound_copy(ToBUFFER);
	
	TCB0.CCMP = sound_tone[0];
	TCB0.INTCTRL = TCB_CAPT_bm;
	TCB0.CTRLB = TCB_CNTMODE_INT_gc;
	TCB0.CTRLA = TCB_CLKSEL_CLKDIV1_gc;
}

SOUND_Error sound_copy(Copy_Mode mode)
{
    if(sound_length > sizeof(ee_sound_tone)/sizeof(ee_sound_tone[0]))
    {
        return SOUND_Range;
    }

    switch (mode)
    {
        case ToEEPROM:

            eeprom_write_block(sound_tone, ee_sound_tone, sound_length * sizeof(sound_tone[0]));
            eeprom_write_byte(&ee_sound_length, sound_length);

        return SOUND_None;
        default:
		
            eeprom_read_block(sound_tone, ee_sound_tone, sound_length * sizeof(sound_tone[0]));
			
        return SOUND_None;  
    }
}

SOUND_Error sound_create(unsigned char position, unsigned char hertz)
{
    if(position >= (sizeof(ee_sound_tone)/sizeof(ee_sound_tone[0])))
    {
        return SOUND_Range;
    }

    sound_tone[position] = sound_hz2value(hertz);
    sound_length = position;

    return SOUND_None;
}

void sound_play(SOUND_Status status)
{
    if(status == SOUND_ON)
    {
        TCB0.CTRLA |= TCB_ENABLE_bm;
        for (unsigned char i=0; i < sound_length; i++)
        {
            TCB0.CCMP = sound_tone[i];
            _delay_ms(SOUND_DELAY_TIME);
        }
        return;
    }
    TCB0.CTRLA &= ~TCB_ENABLE_bm;
    SOUND_PORT.OUTCLR = SOUND_PIN;
}

void sound_toggle_pin(void)
{
    SOUND_PORT.OUTTGL = SOUND_PIN;
}