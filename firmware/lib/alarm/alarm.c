
#include "alarm.h"

unsigned long EEMEM ee_alarm_table[ALARM_TABLE_MAX_LENGTH];
unsigned long alarm_table[sizeof(ee_alarm_table)/sizeof(ee_alarm_table[0])];

unsigned long current_alarm;
unsigned char current_alarm_snooze;

void alarm_init()
{
    alarm_copy(ToBUFFER);
}

static void alarm_status_string(unsigned char data, char *text, unsigned char length)
{
    for (unsigned char i=0; i < length; i++)
    {
        if((0x01 & (data>>i)) == 1)
        {
            text[i] = '+';
            continue;
        }
        text[i] = '-';
    }
    text[length] = '\0';
}

unsigned char alarm_hour(unsigned long alarm)
{
    return (alarm>>ALARM_HOUR_bp);
}

unsigned char alarm_minute(unsigned long alarm)
{
    return (alarm>>ALARM_MINUTE_bp);
}

unsigned char alarm_snooze_minutes(unsigned long alarm)
{
    return (0x0F & (alarm>>ALARM_SNOOZE_MINUTES_bp));
}

static ALARM_Snooze_Mode alarm_snooze_mode(unsigned long alarm)
{
    return (0x0F & (alarm>>ALARM_SNOOZE_MODE_bp));
}

void alarm_snooze_mode_string(unsigned long alarm, char* data)
{
    alarm_status_string(alarm_snooze_mode(alarm), data, ALARM_SNOOZE_MODE_FIELD_LENGTH);
}

unsigned char alarm_snooze_mode_data(char data)
{
    switch (data)
    {
        case 'E':
            return (1<<ALARM_SNOOZE_MODE_E_bp);
        case '5':
            return (1<<ALARM_SNOOZE_MODE_5_bp);
        case '3':
            return (1<<ALARM_SNOOZE_MODE_3_bp);
        case '1':
            return (1<<ALARM_SNOOZE_MODE_1_bp);
        default:
            return 0;
    }
}

static unsigned char alarm_weekday(unsigned long alarm)
{
    return (alarm>>ALARM_WEEKDAY_bp);
}

void alarm_weekday_string(unsigned long alarm, char* data)
{
    alarm_status_string(alarm_weekday(alarm), data, ALARM_WEEKDAY_FIELD_LENGTH);
}

unsigned char alarm_weekday_data(const char* data)
{
    unsigned char temp = 0;

    for (unsigned char i=0; i < 7; i++)
    {
        if(*(data++) == '+')
        {
            temp |= (1<<i);
        }
    }
    return temp;
}


static ALARM_Status alarm_scan_table(unsigned char hour, unsigned char minute, unsigned char weekday)
{
    for (unsigned char i=0; i < (sizeof(alarm_table)/sizeof(alarm_table[0])); i++)
    {
        if((alarm_hour(alarm_table[i]) == hour) && (alarm_minute(alarm_table[i]) == minute) && (alarm_weekday(alarm_table[i]) & (1<<(weekday - 1))))
        {
            current_alarm = alarm_table[i];
            return ALARM_Enabled;
        }
    }
    return ALARM_Disabled;
}

// Should be triggerd every minute once!!!
ALARM_Status alarm_scan(unsigned char hour, unsigned char minute, unsigned char weekday)
{
    if(current_alarm > 0)
    {
        unsigned char snooze_iteration = 0;

        switch(alarm_snooze_mode(current_alarm))
        {
            case ALARM_Snooze_Mode_1:
                snooze_iteration = 1;
            break;
            case ALARM_Snooze_Mode_3:
                snooze_iteration = 3;
            break;
            case ALARM_Snooze_Mode_5:
                snooze_iteration = 5;
            break;
            case ALARM_Snooze_Mode_Infinity:
                snooze_iteration = 255;
            break;
            default:
                alarm_reset();
            return ALARM_Disabled;
        }

        if(snooze_iteration >= current_alarm_snooze)
        {    
            unsigned char timespan_hour = alarm_hour(current_alarm);
            unsigned char timespan_minute = alarm_minute(current_alarm) + current_alarm_snooze * alarm_snooze_minutes(current_alarm);

            do 
            {
                if(timespan_minute > 59)
                {
                    timespan_hour++;
                    timespan_minute -= 60;
                }
                if(timespan_hour > 23)
                {
                    timespan_hour -= 24;
                }
            } while (timespan_minute > 59);

            if((timespan_hour >= hour) && (timespan_minute >= minute))
            {
                return ALARM_Enabled;
            }
            return ALARM_Disabled;
        }
        else
        {
            alarm_reset();
            return ALARM_Disabled;
        }
        return ALARM_Enabled;
    }
    return alarm_scan_table(hour, minute, weekday);
}

void alarm_snooze(void)
{
    current_alarm_snooze++;
}

void alarm_reset(void)
{
    current_alarm = 0UL;
    current_alarm_snooze = 0;
}

ALARM_Error alarm_setalarm(
    unsigned char position,
    unsigned char hour,
    unsigned char minute,
    unsigned char snooze_minutes,
    ALARM_Snooze_Mode snooze_mode,
    ALARM_Weekdays weekdays)
{
    if(position >= (sizeof(alarm_table)/sizeof(alarm_table[0])))
    {
        return ALARM_Out_Of_Range;
    }

    alarm_table[position] = (
        ((unsigned long)(0x0F & snooze_minutes)<<ALARM_SNOOZE_MINUTES_bp) | 
        ((unsigned long)(0x0F & snooze_mode)<<ALARM_SNOOZE_MODE_bp) |
        ((unsigned long)weekdays<<ALARM_WEEKDAY_bp) |
        ((unsigned int)hour<<ALARM_HOUR_bp) |
        (minute<<ALARM_MINUTE_bp)
        );

    return ALARM_None;
}

unsigned long alarm_list_select(unsigned char position)
{
    return alarm_table[position];
}

unsigned long* alarm_list_all()
{
    return alarm_table;
}

void alarm_copy(Copy_Mode mode)
{
    switch (mode)
    {
        case ToEEPROM:
            eeprom_write_block(alarm_table, ee_alarm_table, sizeof(ee_alarm_table));
        break;
        default:
            eeprom_read_block(alarm_table, ee_alarm_table, sizeof(ee_alarm_table));
        break;
    }
}

ALARM_Error alarm_clear(unsigned char position)
{
    if(position >= (sizeof(alarm_table)/sizeof(alarm_table[0])))
    {
        return ALARM_Out_Of_Range;
    }
    alarm_table[position] = 0UL;
    
    return ALARM_None;
}

void alarm_clear_all(void)
{
    for (unsigned char i=0; i < sizeof(alarm_table)/sizeof(alarm_table[0]); i++)
    {
        alarm_table[i] = 0UL;
    }
}