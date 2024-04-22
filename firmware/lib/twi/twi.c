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
*   Function file for twi library
* -----------------------------------------
*/

#include "twi.h"

void twi_init(void)
{
    TWI0.MBAUD =  (unsigned char)TWI_BAUD_RATE(F_TWI);
    TWI0.MCTRLA = (1<<TWI_ENABLE_bp)
    #ifdef TWI_TWIE
        | (1<<TWI_RIEN_bp)
        | (1<<TWI_WIEN_bp)
    #endif
        | TWI_TIMEOUT_DISABLED_gc;
        
    TWI0.MSTATUS = TWI_BUSSTATE_IDLE_gc;
}

void twi_disable(void)
{
    TWI0.MCTRLA &= ~(TWI_ENABLE_bm | TWI_RIEN_bp | TWI_WIEN_bp);
}

#ifndef TWI_TWIE

    void twi_start(void)
    {
    }

    void twi_stop(void)
    {
        TWI0.MCTRLB |= TWI_MCMD_STOP_gc;
    }

    TWI_Status twi_status(void)
    {
        if(TWI0.MSTATUS & (TWI_WIF_bm | TWI_RIF_bm))
        {
            if(!(TWI0.MSTATUS & TWI_RXACK_bm))
            {
                return TWI_Ready;
            }
            else
            {
                return TWI_NAck;
            }
        }
        else if(TWI0.MSTATUS & TWI_ARBLOST_bm)
        {
            return TWI_Arbitration;
        }
        else if(TWI0.MSTATUS & TWI_BUSERR_bm)
        {
            return TWI_Error;
        }
        else
        {
            return TWI_None;
        }
    }

    TWI_Status twi_address(unsigned char address, TWI_Operation operation)
    {
        TWI0.MADDR = (address<<1) | operation;

        TWI_Status temp;

        do 
        {
            temp = twi_status();
        } while (twi_status() == TWI_None);

        return temp;
    }

    TWI_Status twi_set(unsigned char data)
    {
        TWI0.MDATA = data;
        
        TWI_Status temp;

        do
        {
            temp = twi_status();
        } while (twi_status() == TWI_None);

        return temp;
    }

    TWI_Status twi_get(unsigned char *data, TWI_Acknowledge ack)
    {
        TWI_Status temp;

        do
        {
            temp = twi_status();
        } while (twi_status() == TWI_None);

        if(temp == TWI_Ready)
        {
            *data = TWI0.MDATA;
        
            if (ack == TWI_NACK)
            {
                TWI0.MCTRLB = TWI_ACKACT_bm;
                twi_stop();
            }
            else
            {
                TWI0.MCTRLB = TWI_MCMD_RECVTRANS_gc;
            }
        }
        return temp;
    }
#endif