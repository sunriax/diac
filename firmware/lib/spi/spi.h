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
 *   Header file for spi library
 * -----------------------------------------
 */

#ifndef SPI_H_
#define SPI_H_

    // !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
    // !!! When device is configured as  !!!
    // !!! slave the system clock has to !!!
    // !!! be 4 times higher then the    !!!
    // !!! SPI clock f_CPU > 4 * f_SPI   !!! 
    // !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! 

    //#ifndef SPI2X       // Double speed mode
        //#define SPI2X
    //#endif

    #ifndef SPI_CLOCK
        // +------------------------------+-------+------+
        // |         SPI2X                |   0   |   1  |
        // +------------------------------+-------+------+
        // | SPI_PRESC_DIV4_gc   -> F_PER | / 4   | / 2  |
        // | SPI_PRESC_DIV16_gc  -> F_PER | / 16  | / 8  |
        // | SPI_PRESC_DIV64_gc  -> F_PER | / 64  | / 32 |
        // | SPI_PRESC_DIV128_gc -> F_PER | / 128 | / 64 |
        // +------------------------------+-------+------+
        #define SPI_CLOCK SPI_PRESC_DIV4_gc
	
    #endif

    //#ifndef SPI_ALTERNATE_PORT
        //#define SPI_ALTERNATE_PORT
    //#endif

    // !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
    // !!! When using the alternate port !!!
    // !!! pins have to be modified!     !!!
    // !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
    #ifndef SPI_PORT
        #define SPI_PORT PORTA
    #endif

    #ifndef SPI_MOSI
        #define SPI_MOSI PIN1_bm
    #endif
    
    #ifndef SPI_MISO
        #define SPI_MISO PIN2_bm
        #define SPI_MISO_PINCTRL PIN2CTRL
    #endif

    #ifndef SPI_SCK
        #define SPI_SCK PIN3_bm
    #endif

    #ifndef SPI_SS
        #define SPI_SS PIN4_bm
        #define SPI_SS_PINCTRL PIN4CTRL
    #endif

    // Definition of SPI processing (Enabled = Interrupt/Disabled = Polling)

    // !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
    // !!! Interrupts are not included in this library !!!
    // !!! If interrupts are used, disable the polling !!!
    // !!! by uncommenting the next lines              !!!
    // !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!

    //#ifndef SPI_SPIE  // SPI Interrupt based processing
    //  #define SPI_SPIE
    //#endif

    #include <avr/io.h>

    enum SPI_Direction_t
    {
        SPI_MSB=0,
        SPI_LSB
    };
    typedef enum SPI_Direction_t SPI_Direction;

    enum SPI_Edge_t
    {
        SPI_Rising=0,
        SPI_Falling
    };
    typedef enum SPI_Edge_t SPI_Polarity;
    typedef enum SPI_Edge_t SPI_Phase;

    enum SPI_Select_t
    {
        SPI_Disable=0,
        SPI_Enable
    };
    typedef enum SPI_Select_t SPI_Select;

    enum SPI_Status_t
    {
	    SPI_Received=0,
	    SPI_Collision,
	    SPI_None
    };
    typedef enum SPI_Status_t SPI_Status;

    void spi_init(SPI_Direction direction, SPI_Polarity setup, SPI_Phase sample);
    void spi_disable(void);
    void spi_select(SPI_Select mode);

    #ifndef SPI_SPIE
	    unsigned char spi_transfer(unsigned char data);
    #endif

#endif /* SPI_H_ */