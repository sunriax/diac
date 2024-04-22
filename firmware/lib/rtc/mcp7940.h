
#ifndef MCP7940_H_
#define MCP7940_H_

    #ifndef F_CPU
        #define F_CPU 20000000UL
    #endif

    #ifndef MCP7940_ADDRESS
        #define MCP7940_ADDRESS 0x6F
    #endif

    #ifndef MCP7940_RTCSEC
        #define MCP7940_RTCSEC 0x00

        #define RTC_ST_bm 0x80
        #define RTC_SECTEN_bm 0x70
        #define RTC_SECTEN_bp 4
    #endif

    #ifndef MCP7940_RTCMIN
        #define MCP7940_RTCMIN 0x01
        
        #define RTC_MINTEN_bm 0x70
        #define RTC_MINTEN_bp 4
    #endif

    #ifndef MCP7940_RTCHOUR
        #define MCP7940_RTCHOUR 0x02

        #define RTC_FORMAT_bm 0x40
        #define RTC_HRTEN_bm 0x30
        #define RTC_HRTEN_bp 4
    #endif

    #ifndef MCP7940_RTCWKDAY
        #define MCP7940_RTCWKDAY 0x03

        #define RTC_OSCRUN_bm 0x20
        #define RTC_PWRFAIL_bm 0x10
        #define RTC_VBATEN_bm 0x08
        #define RTC_WKDAY_bp 0
    #endif

    #ifndef MCP7940_RTCDATE
        #define MCP7940_RTCDATE 0x04

        #define RTC_DATETEN_bm 0x30
        #define RTC_DATETEN_bp 4
    #endif

    #ifndef MCP7940_RTCMTH
        #define MCP7940_RTCMTH 0x05
        
        #define RTC_LPYR_bp 5
        #define RTC_LPYR_bm 0x20
        #define RTC_MTHTEN_bm 0x10
        #define RTC_MTHTEN_bp 4
    #endif

    #ifndef MCP7940_RTCYEAR
        #define MCP7940_RTCYEAR 0x06

        #define RTC_YRTEN_bm 0xF0
        #define RTC_YRTEN_bp 4
    #endif

    #ifndef MCP7940_CONTROL
        #define MCP7940_CONTROL 0x07

        #define RTC_OUT_bm 0x80
        #define RTC_SQWEN_bm 0x40
        #define RTC_ALM1EN_bm 0x20
        #define RTC_ALM0EN_bm 0x10
        #define RTC_EXTOSC_bm 0x08
        #define RTC_CSTRIM_bm 0x04
        #define RTC_SQWFS1_bm 0x02
        #define RTC_SQWFS0_bm 0x01

        #define RTC_SQWFS_32768HZ (RTC_SQWFS1_bm | RTC_SQWFS0_bm)
        #define RTC_SQWFS_8192HZ RTC_SQWFS1_bm
        #define RTC_SQWFS_4096HZ RTC_SQWFS0_bm
        #define RTC_SQWFS_1HZ 0x00
    #endif

    #ifndef MCP7940_OSCTRIM
        #define MCP7940_OSCTRIM 0x08

        #define RTC_SIGN_bm 0x80
    #endif

    // #############################################

    #if !defined(MCP7940_ALM0SEC) && !defined(MCP7940_ALM1SEC)
        #define MCP7940_ALM0SEC 0x0A
        #define MCP7940_ALM1SEC 0x11

        #define ALM_SECTEN_bp 4
        #define ALM_SECONE_bp 0
    #endif

    #if !defined(MCP7940_ALM0MIN) && !defined(MCP7940_ALM1MIN)
        #define MCP7940_ALM0MIN 0x0B
        #define MCP7940_ALM1MIN 0x12

        #define ALM_MINTEN_bp 4
        #define ALM_MINONE_bp 0
    #endif

    #if !defined(MCP7940_ALM0HOUR) && !defined(MCP7940_ALM1HOUR)
        #define MCP7940_ALM0HOUR 0x0C
        #define MCP7940_ALM1HOUR 0x13

        #define ALM_FORMAT_bm 0x40
        #define ALM_HRTEN_bp 4
        #define ALM_HRONE_bp 0
    #endif

    #if !defined(MCP7940_ALM0WKDAY) && !defined(MCP7940_ALM1WKDAY)
        #define MCP7940_ALM0WKDAY 0x0D
        #define MCP7940_ALM1WKDAY 0x14

        #define ALM_ALMPOL_bm 0x80
        #define ALM_ALMMSK2_bm 0x40
        #define ALM_ALMMSK1_bm 0x20
        #define ALM_ALMMSK0_bm 0x10
        #define ALM_ALMIF_bm 0x08
        #define ALM_WKDAY_bp 0

        #define ALM_SECOND_MATCH 0x00
        #define ALM_MINUTE_MATCH ALM_ALMMSK0_bm
        #define ALM_HOUR_MATCH ALM_ALMMSK1_bm
        #define ALM_DAY_MATCH (ALM_ALMMSK1_bm | ALM_ALMMSK1_bm)
        #define ALM_DATE_MATCH ALM_ALMMSK2_bm
        #define ALM_FULL_MATCH (ALM_ALMMSK2_bm | ALM_ALMMSK1_bm | ALM_ALMMSK0_bm)
    #endif

    #if !defined(MCP7940_ALM0DATE) && !defined(MCP7940_ALM1DATE)
        #define MCP7940_ALM0DATE 0x0E
        #define MCP7940_ALM1DATE 0x15

        #define ALM_DATETEN_bp 4
        #define ALM_DATEONE_bp 0
    #endif

    #if !defined(MCP7940_ALM0MTH) && !defined(MCP7940_ALM1MTH)
        #define MCP7940_ALM0MTH 0x0F
        #define MCP7940_ALM1MTH 0x16

        #define ALM_MTHTEN_bp 4
        #define ALM_MTHONE_bp 0
    #endif

    // #############################################

    #if !defined(MCP7940_PWRDNMIN) && !defined(MCP7940_PWRUPMIN)
        #define MCP7940_PWRDNMIN 0x18
        #define MCP7940_PWRUPMIN 0x1C
    #endif

    #if !defined(MCP7940_PWRDNHOUR) && !defined(MCP7940_PWRUPHOUR)
        #define MCP7940_PWRDNHOUR 0x19
        #define MCP7940_PWRUPHOUR 0x1D
    #endif

    #if !defined(MCP7940_PWRDNDATE) && !defined(MCP7940_PWRUPDATE)
        #define MCP7940_PWRDNDATE 0x1A
        #define MCP7940_PWRUPDATE 0x1E
    #endif

    #if !defined(MCP7940_PWRDNMTH) && !defined(MCP7940_PWRUPMTH)
        #define MCP7940_PWRDNMTH 0x1B
        #define MCP7940_PWRUPMTH 0x1F

        #define MCP7940_PWRWEEKDAY_bp 5
    #endif

    // #############################################

    #ifndef MCP7940_MFP_PORT
        #define MCP7940_MFP_PORT RTC_SQWEN_bm
    #endif

    #include <avr/io.h>
    #include <avr/eeprom.h>
    #include <util/delay.h>

    #include "../twi/twi.h"
    #include "../types/types.h"

    enum RTC_Status_t
    {
        RTC_Disabled=0,
        RTC_Running,
        RTC_Fail,
        RTC_Power_Fail,
        RTC_Valid
    };
    typedef enum RTC_Status_t RTC_Status;

    enum RTC_Mode_t
    {
        RTC_Disable=0,
        RTC_Enable
    };
    typedef enum RTC_Mode_t RTC_Mode;

    enum RTC_Data_t
    {
        RTC_Current=0,
        RTC_Power_Down,
        RTC_Power_Up
    };
    typedef enum RTC_Data_t RTC_Data;

             void mcp7940_init(RTC_Mode battery);
             void mcp7940_battery(RTC_Mode mode);
             void mcp7940_oscillator(RTC_Mode mode);

    unsigned char mcp7940_oscillator_value(void);
             void mcp7940_oscillator_copy(Copy_Mode mode);

       RTC_Status mcp7940_powerfail_status(void);
       RTC_Status mcp7940_oscillator_status(void);

      const char* mcp7940_weekday_string(unsigned char day);
    
    unsigned char mcp7940_hour(RTC_Data data);
    unsigned char mcp7940_minute(RTC_Data data);
    unsigned char mcp7940_second(void);

    unsigned char mcp7940_weekday(RTC_Data data);
    unsigned char mcp7940_day(RTC_Data data);
    unsigned char mcp7940_month(RTC_Data data);
    unsigned char mcp7940_year(void);
    unsigned char mcp7940_leapyear(void);

       RTC_Status mcp7940_setweekday(unsigned char weekday);
       RTC_Status mcp7940_settime(unsigned char hour, unsigned char minute, unsigned char second);
       RTC_Status mcp7940_setdate(unsigned char day, unsigned char month, unsigned char year);
       RTC_Status mcp7940_setoscillator(unsigned char trim);

#endif /* MCP7940_H_ */