
#include "uart.h"

#if UART_STDMODE > 0
    // Initialize FILE stream
    #if !defined(UART_TXCIE) && !defined(UART_UDRIE) && !defined(UART_RXCIE) && UART_STDMODE == 1
        static FILE std_uart = FDEV_SETUP_STREAM(uart_printf, uart_scanf, _FDEV_SETUP_RW);
    #elif !defined(UART_TXCIE) && !defined(UART_UDRIE) && UART_STDMODE == 2
        static FILE std_uart = FDEV_SETUP_STREAM(uart_printf, NULL, _FDEV_SETUP_WRITE);
    #elif !defined(UART_RXCIE) && UART_STDMODE == 3
        static FILE std_uart = FDEV_SETUP_STREAM(NULL, uart_scanf, _FDEV_SETUP_READ);
    #endif
#endif

#if !defined(UART_RXCIE) && !defined(UART_TXCIE) && !defined(UART_UDRIE)
    #if UART_HANDSHAKE > 0
        static UART_Handshake uart_handshake_sending = UART_Ready;
    #endif
#endif

//  +---------------------------------------------------------------+
//  |                       UART initialization                     |
//  +---------------------------------------------------------------+
void uart_init(void)
{
    // UART PORT settings
    #ifdef UART_ALTERNATE_PORT
        PORTMUX.CTRLB = PORTMUX_USART0_ALTERNATE_gc;
    #endif
    
    UART_PORT.DIRSET = UART_TX_PIN;
    UART_PORT.DIRCLR = UART_RX_PIN;
    
    // Check if hardware flow control is enabled
    #if UART_HANDSHAKE == 2
        // Setup RTS (output)/CTS (input)
        UART_HANDSHAKE_PORT.DIRSET = UART_HANDSHAKE_RTS;
        UART_HANDSHAKE_PORT.DIRCLR = UART_HANDSHAKE_CTS;
    #endif
    
    // Check which bit sampling mode should be activated
    #if UART_SAMPLE == 1
        USART0.CTRLB |= (UART_SAMPLE<<1);	// Setup 8 samples/bit
	#else
		USART0.CTRLB &= ~(UART_SAMPLE<<1);	// Setup 16 samples/bit
    #endif

    USART0.BAUD = (unsigned int)UART_BAUD_RATE(BAUDRATE);
    USART0.CTRLC = (0x03 & (UART_DATASIZE - 5));    // Setup data size
    
    #if UART_PARITY > 0
        USART0.CTRLC |= (0x30 & ((UART_PARITY + 1)<<4));  // UART_Parity Mode
    #endif
    
    #if UART_STOPBITS > 1
        USART0.CTRLC |= (0x08 & (UART_STOPBITS<<3));      // Setup stop bits
    #endif

    // Interrupt control
    USART0.CTRLA = 0x00;
    
    // Enable transmitter/receiver
    USART0.CTRLB |= USART_TXEN_bm | USART_RXEN_bm;

    // Receiver interrupt setup
    #ifdef UART_RXCIE
        USART0.CTRLA |= USART_RXCIE_bm;
    #endif

    // Transmitter interrupt setup
    #if defined(UART_TXCIE) && !defined(UART_UDRIE)
        USART0.CTRLA |= USART_TXCIE_bm;
    #endif

    // Transmitter interrupt setup
    #if !defined(UART_TXCIE) && defined(UART_UDRIE)
        USART0.CTRLA |= USART_DREIE_bm;
    #endif

    #if !defined(UART_TXCIE) && !defined(UART_UDRIE) && (UART_STDMODE == 1 || UART_STDMODE == 2)
        stdout = &std_uart;
    #endif
    
    #if !defined(UART_RXCIE) && UART_STDMODE == 1 || UART_STDMODE == 3
        stdin = &std_uart;
    #endif
}

void uart_disable(void)
{
    USART0.CTRLB &= ~(USART_TXEN_bm | USART_RXEN_bm);
    USART0.CTRLA &= ~(USART_RXCIE_bm | USART_TXCIE_bm | USART_DREIE_bm);
}

#if !defined(UART_TXCIE) && !defined(UART_UDRIE)
    char uart_putchar(char data)
    {
        // Wait until last transmission completed
        while (!(USART0.STATUS & USART_DREIF_bm))
        {
            ;
        }
        USART0.TXDATAL = data; // Write data to transmission register
    
        return 0;   // Return that there was no fault
    }

    #if (UART_STDMODE == 1 || UART_STDMODE == 2)
        int uart_printf(char data, FILE *stream)
        {
            return uart_putchar(data);
        }
    #endif

#endif

#if !defined(UART_RXCIE)
    UART_Data uart_scanchar(char *data)
    {
        // If data has been received
        if(USART0.STATUS & USART_RXCIF_bm)
        {
            // Check if an error during receiving data occurred
            if(uart_error_flags() != UART_None)
            {
                *data = 0;
                return UART_Fault;
            }
        
            #if UART_HANDSHAKE == 1
                if (*data == UART_HANDSHAKE_XON)
                {
                    uart_handshake_sending = UART_Ready;
                    return UART_Empty;
                }
                else if (*data == UART_HANDSHAKE_XOFF)
                {
                    uart_handshake_sending = UART_Pause;
                    return UART_Empty;
                }
            #endif
        
            *data = USART0.RXDATAL;
        
            #if defined(UART_RXC_ECHO) && !defined(UART_TXCIE) && !defined(UART_UDRIE)
                // Send echo of received data to UART
                uart_putchar(*data);
            #endif
        
            return UART_Received;
        }
        return UART_Empty;
    }

    char uart_getchar(UART_Data *status)
    {
        char data;
    
        // Wait until data has been received or an error occurred
        do
        {
            *status = uart_scanchar(&data);
        } while (*status == UART_Empty);
		
        return data;
    }

    #if (UART_STDMODE == 1 || UART_STDMODE == 3)
        int uart_scanf(FILE *stream)
        {
            UART_Data temp;
    
            return (int)uart_getchar(&temp);
        }

        void uart_clear(void)
        {
            clearerr(stdin);    // Clear error on stream
            getchar();          // Remove character from stream
        }

    UART_Error uart_error_flags(void)
    {
        // UART_Frame error
        if(USART0.RXDATAH & USART_FERR_bm)
        {
            USART0.RXDATAL;			// Clear UART data register
            return UART_Frame;		// Return NUL
        }
        // Data UART_Buffer overflow error
        else if(USART0.RXDATAH & USART_BUFOVF_bm)
        {
            USART0.RXDATAL;			// Clear UART data register
            return UART_Overrun;	// Return NUL
        }
        // UART_Parity error
        else if(USART0.RXDATAH & USART_PERR_bm)
        {
            USART0.RXDATAL;         // Clear UART data register
            return UART_Parity;		// Return NUL
        }
        return UART_None;
    }
    #endif
#endif

#if !defined(UART_RXCIE) && !defined(UART_TXCIE) && !defined(UART_UDRIE)
    #if UART_HANDSHAKE > 0
        UART_Handshake uart_handshake(UART_Handshake status)
        {
            if(status == UART_Ready)
            {
                #if UART_HANDSHAKE == 1
                    uart_putchar(UART_HANDSHAKE_XON);
                #elif UART_HANDSHAKE == 2
                    UART_HANDSHAKE_PORT &= ~(1<<UART_HANDSHAKE_RTS);
                #endif
            }
            else if(status == UART_Pause)
            {
                #if UART_HANDSHAKE == 1
                    uart_putchar(UART_HANDSHAKE_XOFF);
                #elif UART_HANDSHAKE == 2
                    UART_HANDSHAKE_PORT |= (1<<UART_HANDSHAKE_RTS);
                #endif
            }
            else
            {
                #if UART_HANDSHAKE == 1
                    return uart_handshake_sending;
                #elif UART_HANDSHAKE == 2
                    if (!(UART_HANDSHAKE_PORT.IN & (1<<UART_HANDSHAKE_CTS)))
                    {
                        return UART_Ready;
                    }
                    return UART_Pause;
                #endif
            }
            return UART_Status;
        }
    #endif
#endif