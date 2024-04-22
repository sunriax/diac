
#include "main.h"

ISR(TCA0_OVF_vect)
{
    clock_output(CLOCK_Disable);
    TCA0.SINGLE.INTFLAGS = TCA_SINGLE_OVF_bm;
}

ISR(TCA0_CMP0_vect)
{
    clock_output(CLOCK_Enable);
    TCA0.SINGLE.INTFLAGS = TCA_SINGLE_CMP0_bm;
}

ISR(TCB0_INT_vect)
{
    sound_toggle_pin();
    TCB0.INTFLAGS = TCB_CAPT_bm;
}

volatile unsigned char day, month, year, weekday, hour, min, sec;

ISR(PORTA_PORT_vect)
{
    if((++sec) > 59)
    {
        sec = 0;
        
        if((++min) > 59)
        {
            min = 0;
            
            if((++hour) > 23)
            {
                hour = 0;
            }
        }
    }
    EXTERNAL_INTERRUPT_TRIGGER_PORT.INTFLAGS = EXTERNAL_INTERRUPT_TRIGGER_PIN;
}

static void update_datetime(void)
{
    hour = mcp7940_hour(RTC_Current);
    min = mcp7940_minute(RTC_Current);
    sec = mcp7940_second();
    day = mcp7940_day(RTC_Current);
    month = mcp7940_month(RTC_Current);
    year = mcp7940_year();
    weekday = mcp7940_weekday(RTC_Current);
}

static const char string_init[] = "\n\rInitializing";
static const char string_done[] = "Done";

int main(void)
{
    EXTERNAL_INTERRUPT_TRIGGER_PORT.DIRCLR = EXTERNAL_INTERRUPT_TRIGGER_PIN;
    EXTERNAL_INTERRUPT_TRIGGER_PORT.EXTERNAL_INTERRUPT_TRIGGER_PINCTRL = EXTERNAL_INTERRUPT_TRIGGER_PIN_SETUP;

    system_init();
    console_init();

    sei();

    printf("%s Input\t", string_init);
    input_init();
    printf("%s", string_done);
    printf("%s RTC\t", string_init);
    mcp7940_init(RTC_Enable);
    printf("%s", string_done);
    printf("%s Clock\t", string_init);
    clock_init();
    printf("%s", string_done);
    printf("%s Sound\t", string_init);
    sound_init();
    printf("%s", string_done);
    printf("%s Alarm\t", string_init);
    alarm_init();
    printf("%s", string_done);
    printf("%s Sensor\t", string_init);
    sensor_init();
    printf("%s", string_done);
    console_alarm_list(alarm_list_all(), ALARM_TABLE_MAX_LENGTH);

    SETUP_Update setup_update = SETUP_Update_None;
    SETUP_Mode setup_mode = setup_init();

    while((setup_mode != SETUP_Mode_None) && (setup_mode != SETUP_Mode_Done))
    {
		setup_menu(&setup_mode, &setup_update);
    }
    setup_save(setup_update);
    
    unsigned char last_sec = 0;
    unsigned char last_min = 0;
    unsigned char alarm_run = 0;
    unsigned char status = 0;

	update_datetime();
	console_newline();

    while(1)
    {
        if(input_status(INPUT_SW1))
        {
            clock_setdate(day, month, CLOCK_STATUS_LED_BLUE2);
        }
        else if(last_sec != sec)
        {
            if((hour == 0) && (min == 0) && (sec == 0))
            {
                update_datetime();
            }

            status ^= CLOCK_STATUS_LED_GREEN;

            clock_settime(hour, min, sec, status);
            printf("\rCurrent Timestamp %2u:%2u:%2u | %3s(%u) %2u-%2u-%2u", hour, min, sec, mcp7940_weekday_string(weekday), weekday, day, month, year);

            last_sec = sec;
        }

        if(last_min != min)
        {
            // Daylight saving Time
            if((hour == 2) && (min == 0))
            {
                if((month == 4) && (weekday == 7) && ((day + 7) > 31))
                {
                    mcp7940_settime((++hour), min, (++sec));
                }
                else if((month == 10) && (weekday == 7) && ((day + 7) > 31))
                {
                    mcp7940_settime((--hour), min, (++sec));
                }
            }

            if(alarm_scan(hour, min, weekday) == ALARM_Enabled)
            {
                alarm_run = 1;
            }
            last_min = min;
        }

        if(alarm_run)
        {
            clock_setdata(0x0F0F0F0F, 0x0F0F0F0F0F0F0F0F, status);
            sound_play(SOUND_ON);

            if(input_status(INPUT_SW3))
            {
                sound_play(SOUND_OFF);
                alarm_reset();

                alarm_run = 0;
            }
            else if(input_status(INPUT_SW4))
            {
                sound_play(SOUND_OFF);
                alarm_snooze();

                alarm_run = 0;
            }
        }
    }
}