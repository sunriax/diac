
#ifndef ALARM_H_
#define ALARM_H_

// Alarm definition:
//  31                            24:23                           16:15                            8:7                              0
//  +---+---+---+---+---+---+---+---+---+---+---+---+---+---+---+---+---+---+---+---+---+---+---+---+---+---+---+---+---+---+---+---+
//  | ? | ? | ? | ? | ? | ? | ? | ? | ? | ? | ? | ? | ? | ? | ? | ? | ? | ? | ? | ? | ? | ? | ? | ? | ? | ? | ? | ? | ? | ? | ? | ? |
//  +---+---+---+---+---+---+---+---+---+---+---+---+---+---+---+---+---+---+---+---+---+---+---+---+---+---+---+---+---+---+---+---+
//  | MINUTES(1-16) |    MODE       |     S   S   F   T   W   T   M |            BINARY             |            BINARY             |
//  |            SNOOZE             |              WEEKDAY          |             HOUR              |            MINUTE             |
//  +-------------------------------+-------------------------------+-------------------------------+-------------------------------+

    #ifndef ALARM_SNOOZE_MODE_bp
        #define ALARM_SNOOZE_MODE_bp 24
    #endif

    #ifndef ALARM_SNOOZE_MODE_FIELD_LENGTH
        #define ALARM_SNOOZE_MODE_FIELD_LENGTH 4
    #endif

    #ifndef ALARM_SNOOZE_MODE_E_bp
        #define ALARM_SNOOZE_MODE_E_bp 3
    #endif

    #ifndef ALARM_SNOOZE_MODE_5_bp
        #define ALARM_SNOOZE_MODE_5_bp 2
    #endif
    
    #ifndef ALARM_SNOOZE_MODE_3_bp
        #define ALARM_SNOOZE_MODE_3_bp 1
    #endif

    #ifndef ALARM_SNOOZE_MODE_1_bp
        #define ALARM_SNOOZE_MODE_1_bp 0
    #endif

    #ifndef ALARM_SNOOZE_MINUTES_bp
        #define ALARM_SNOOZE_MINUTES_bp 28
    #endif

    #ifndef ALARM_WEEKDAY_bp
        #define ALARM_WEEKDAY_bp 16
    #endif

    #ifndef ALARM_WEEKDAY_FIELD_LENGTH
        #define ALARM_WEEKDAY_FIELD_LENGTH 7
    #endif

    #ifndef ALARM_HOUR_bp
        #define ALARM_HOUR_bp 8
    #endif

    #ifndef ALARM_MINUTE_bp
        #define ALARM_MINUTE_bp 0
    #endif

    #ifndef ALARM_TABLE_MAX_LENGTH
        #define ALARM_TABLE_MAX_LENGTH 8
    #endif

    #include <avr/io.h>
    #include <avr/eeprom.h>

    #include "../types/types.h"

    enum ALARM_Status_t
    {
        ALARM_Disabled=0,
        ALARM_Enabled
    };
    typedef enum ALARM_Status_t ALARM_Status;

    enum ALARM_Error_t
    {
        ALARM_None=0,
        ALARM_Out_Of_Range
    };
    typedef enum ALARM_Error_t ALARM_Error;

    enum ALARM_Snooze_Mode_t
    {
        ALARM_Snooze_Mode_None=0x00,
        ALARM_Snooze_Mode_1=0x01,
        ALARM_Snooze_Mode_3=0x02,
        ALARM_Snooze_Mode_5=0x04,
        ALARM_Snooze_Mode_Infinity=0x08
    };
    typedef enum ALARM_Snooze_Mode_t ALARM_Snooze_Mode;

    enum ALARM_Weekdays_t
    {
        ALARM_Weekdays_Monday=0x00,
        ALARM_Weekdays_Tuesday=0x01,
        ALARM_Weekdays_Wednesday=0x02,
        ALARM_Weekdays_Thursday=0x04,
        ALARM_Weekdays_Friday=0x08,
        ALARM_Weekdays_Saturday=0x16,
        ALARM_Weekdays_Sunday=0x32,
    };
    typedef enum ALARM_Weekdays_t ALARM_Weekdays;

             void alarm_init();

     ALARM_Status alarm_scan(unsigned char hour, unsigned char minute, unsigned char weekday);
             void alarm_snooze(void);
             void alarm_reset(void);

      ALARM_Error alarm_setalarm(unsigned char position, unsigned char hour, unsigned char minute, unsigned char snooze_minutes, ALARM_Snooze_Mode snooze_mode, ALARM_Weekdays weekdays);

    unsigned char alarm_hour(unsigned long alarm);
    unsigned char alarm_minute(unsigned long alarm);
    unsigned char alarm_snooze_minutes(unsigned long alarm);
             void alarm_snooze_mode_string(unsigned long alarm, char* data);
    unsigned char alarm_snooze_mode_data(char data);
             void alarm_weekday_string(unsigned long alarm, char* data);
    unsigned char alarm_weekday_data(const char* data);

    unsigned long alarm_list_select(unsigned char position);
   unsigned long* alarm_list_all(void);

             void alarm_copy(Copy_Mode mode);
      ALARM_Error alarm_clear(unsigned char position);
             void alarm_clear_all(void);

#endif /* ALARM_H_ */