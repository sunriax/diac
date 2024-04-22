/* -----------------------------------------
 * G.Raf^engineering
 * www.sunriax.at
 * -----------------------------------------
 *    Platform: CLOCK
 *    Hardware: ATTiny406/1606/...
 * -----------------------------------------
 *     Version: 1.0 Release
 *      Author: G.Raf
 * Description:
 *   Function file for spi library
 * -----------------------------------------
 */

#include "spi.h"

void spi_init(SPI_Direction direction, SPI_Polarity setup, SPI_Phase sample)
{
    #ifdef SPI_ALTERNATE_PORT
        PORTMUX.CTRLB = PORTMUX_SPI0_ALTERNATE_gc;
    #endif

	// !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
	// !!!      SPECIAL FUNCTION        !!!
	// !!! If during initialisation SS  !!!
	// !!! pin is LOW, SPI controller   !!!
	// !!! will be configured as slave. !!!
	// !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
	PORTA.DIRCLR = SPI_MOSI | SPI_MISO | SPI_SCK | SPI_SS;
	PORTA.SPI_MISO_PINCTRL = PORT_PULLUPEN_bm;
	PORTA.SPI_SS_PINCTRL = PORT_PULLUPEN_bm;
	
	SPI0.CTRLA = SPI_MASTER_bm
	#ifdef SPI2X
		| SPI_CLK2X_bm
	#endif
	
	#ifdef SPI_CLOCK
		| SPI_CLOCK
	#endif
		| ((0x01 & direction)<<6);
	
	SPI0.CTRLB |= ((0x01 & setup)<<1) | ((0x01 & sample)<<0);

	#ifdef SPI_SPIE
		SPI0.INTCTRL |= SPI_IE_bm;
	#endif
	
	SPI0.CTRLA |= SPI_ENABLE_bm;
	
	// Setup SCK, MOSI and SS as output
	// PORT configuration gets overwritten from SPI controller
	PORTA.DIR |= SPI_MOSI | SPI_MISO | SPI_SCK | SPI_SS;
}

#ifndef SPI_SPIE

    void spi_select(SPI_Select mode)
    {
	    switch(mode)
	    {
		    case SPI_Enable : PORTA.OUTCLR = SPI_SS;	break;
		    default         : PORTA.OUTSET = SPI_SS;	break;
	    }
    }

    unsigned char spi_transfer(unsigned char data)
    {
	    SPI0.DATA = data;

	    while (!(SPI0.INTFLAGS & SPI_IF_bm))
        {
            ;
        }
	    return SPI0.DATA;
    }

#endif

void spi_disable(void)
{
    // Disable SPI
    SPI0.CTRLA &= ~SPI_ENABLE_bm;
    PORTA.DIRCLR = PIN4_bm | PIN3_bm | PIN2_bm | PIN1_bm;
    
    #ifdef SPI_SPIE
        SPI0.INTCTRL &= ~SPI_IE_bm;
    #endif
}