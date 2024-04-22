
#ifndef CONSOLE_H_
#define CONSOLE_H_

    #ifndef CONSOLE_BUFFER_LENGTH
        #define CONSOLE_BUFFER_LENGTH 100
    #endif

    #ifndef CONSOLE_LINE_LENGTH
        #define CONSOLE_LINE_LENGTH 30
    #endif
    
    #ifndef CONSOLE_LINE_CHARACTER
        #define CONSOLE_LINE_CHARACTER '#'
    #endif

    #ifndef CONSOLE_TABLE_CORNER_CHARACTER
        #define CONSOLE_TABLE_CORNER_CHARACTER '+'
    #endif

    #ifndef CONSOLE_TABLE_LINE_CHARACTER
        #define CONSOLE_TABLE_LINE_CHARACTER '-'
    #endif

    #ifndef CONSOLE_TABLE_COLUMN_CHARACTER
        #define CONSOLE_TABLE_COLUMN_CHARACTER '|'
    #endif

    #include <avr/io.h>
    #include <avr/eeprom.h>

    #include "../types/types.h"
    #include "../uart/uart.h"
    #include "../alarm/alarm.h"

    enum CONSOLE_Border_t
    {
        CONSOLE_Alarm_None=0x00,
        CONSOLE_Alarm_Header=0x01,
        CONSOLE_Alarm_Footer=0x02
    };
    typedef enum CONSOLE_Border_t CONSOLE_Border;

    void console_init(void);
    void console_clear(void);
    void console_newline(void);
    void console_line(unsigned char length);

    void console_alarm(unsigned char id, unsigned long alarm, CONSOLE_Border border);
    void console_alarm_list(unsigned long *alarm, unsigned char length);

    void console_table_border(unsigned char length);
    void console_table_column(const char *text, unsigned char length);
    void console_table_header(const char *text, unsigned char length);

    char console_readkey(const char *input);
    void console_readline(const char *input, char *buffer, unsigned char length);

#endif /* CONSOLE_H_ */