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
 *   Header file for twi library
 * -----------------------------------------
 */

#ifndef TWI_H_
#define TWI_H_

	#ifndef F_PER               // System clock
		#define F_PER 10000000UL
	#endif

	// Definition of TWI parameters
	#ifndef F_TWI               // TWI clock
		#define F_TWI 400000UL
	#endif  

	#ifndef T_RISE              // TWI Signal rise time
		#define T_RISE 1/(float)10000000
	#endif

	#ifndef TWI_BAUD_RATE
		#define TWI_BAUD_RATE(MBAUD_RATE) (((float)F_PER)/(float)((MBAUD_RATE<<1)) - 5UL - ((unsigned int)(F_PER * (T_RISE))>>1))
	#endif

	#ifndef TWI_PRESCALE        // TWI TWSR register prescaler (0-3)
		#define TWI_PRESCALE 0
	#endif

	#ifndef TWI_WRITE           // TWI write command
		#define TWI_WRITE 0x00
	#endif

	#ifndef TWI_READ            // TWI read command
		#define TWI_READ 0x01
	#endif

    // Definition of TWI processing (Enabled = Interrupt/Disabled = Polling)

    //  !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
    //  !!! Interrupts are not included in this library  !!!
    //  !!! If interrupts are used, polling will be      !!!
    //  !!! disabled                                     !!!
    //  !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!

    //#ifndef TWI_TWIE  // TWI Interrupt based processing
    //  #define TWI_TWIE
    //#endif

    #include <avr/io.h>

    enum TWI_Operation_t
    {
        TWI_Write=TWI_WRITE,
        TWI_Read=TWI_READ
    };
    typedef enum TWI_Operation_t TWI_Operation;

    enum TWI_Acknowledge_t
    {
	    TWI_NACK=0,
	    TWI_ACK
    };
    typedef enum TWI_Acknowledge_t TWI_Acknowledge;

    enum TWI_Status_t
    {
        TWI_None=0,
        TWI_NAck,
	    TWI_Ready,
        TWI_Arbitration,
        TWI_Error
    };
    typedef enum TWI_Status_t TWI_Status;

	      void twi_init(void);
          void twi_disable(void);
    TWI_Status twi_status(void);

    #ifndef TWI_TWIE
              void twi_start(void);
              void twi_stop(void);
        TWI_Status twi_address(unsigned char address, TWI_Operation operation);
        TWI_Status twi_set(unsigned char data);
        TWI_Status twi_get(unsigned char *data, TWI_Acknowledge ack);
    #endif


#endif /* TWI_H_ */