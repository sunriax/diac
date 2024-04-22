
#include "setup.h"

const char string_system_setup[]        = "System Setup";
const char string_system_update[]       = "Updating configuration";
const char string_system_updating[]     = "Updating";
const char string_start_config[]        = "Starting configuration!";
const char string_press_any_key[]       = "Press any key to start";
const char string_press_q_key[]         = "Press q to quit";

const char string_system_config[]       = "System Configuration";
const char string_time_and_date[]       = "Time and Date";
const char string_alarm[]               = "Alarm";
const char string_alarm_tone[]          = "Alarm Tone";
const char string_intensity[]           = "LED Intensity";
const char string_oscillator[]          = "Oscillator";

const char string_change_time[]         = "Change Time";

const char string_input_time[]          = "[hh:mm:ss]";
const char string_input_date[]          = "[dd-mm-yy]";
const char string_input_weekday[]       = "[1=MON-7]";

const char string_select_alarm[]        = "Change alarm (?=Number)";
const char string_clear_alarm[]         = "Clear alarm";

const char string_input_alarm_weekdays[]= "[MTWTFSS]";
const char string_input_snooze_mode[]   = "[E/5/3/1/N]";
const char string_input_snooze_time[]   = "[0-16]";

const char string_sound_play[]          = "Play";
const char string_sound_edit[]          = "Edit";
const char string_sound_frequency[]     = "Frequency";

const char string_intensity_increase[]  = "Increase";
const char string_intensity_decrease[]  = "Decrease";

const char string_oscillator_modify[]   = "Modify";
const char string_oscillator_trim[]     = "([-]NNN)";

const char string_notice_done[]         = "Done";
const char string_notice_input[]        = "Input";
const char string_notice_error[]        = "Error";
const char string_notice_notfound[]     = "Not found";
const char string_notice_range[]        = "Out of range";
const char string_notice_notanumber[]   = "Not a number";

const char string_ask_save[]            = "Save changes";
const char string_ask_discharge[]       = "Discharge changes";
const char string_ask_exit[]            = "Exit configuration";

SETUP_Mode setup_init(void)
{
    printf("\n\n\r%s\n\r", string_system_setup);
    console_line(CONSOLE_LINE_LENGTH);

    if(input_status(INPUT_SW1))
    {
        printf("%s", string_start_config);
        
        while(input_status(INPUT_SW1));
        return SETUP_Mode_Manual;
    }
    else
    {
        printf("\n\r%s\n\r", string_press_any_key);
        
        for (unsigned char i=0; i < SETUP_LIMIT_CYCLES; i++)
        {
            if((i%10) == 0)
            {
                uart_putchar('+');
            }
            
            char temp;
            
            if(uart_scanchar(&temp) == UART_Received)
            {
                printf("%s", string_start_config);
                return SETUP_Mode_Console;
            }
            _delay_ms(25);
        }
    }
    return SETUP_Mode_None;
}

static void setup_menu_list(Menu_Item *menu, unsigned char length)
{
    char buffer[100];

    for (unsigned char y=0; y < length; y++)
    {
        if(y == 0)
        {
            console_table_header(menu[y].str, SETUP_TABLE_WIDTH);
        }
        else
        {
            buffer[0] = '[';

            if(menu[y].acronym == '0')
            {
                buffer[1] = ('0' + y);
            }
            else
            {
                buffer[1] = menu[y].acronym;
            }

            buffer[2] = ']';
            buffer[3] = SPACE_CHARACTER;
            buffer[4] = '\0';

            strcat(buffer, menu[y].str);
            console_table_column(buffer, SETUP_TABLE_WIDTH);
        }
    }
    console_table_border(SETUP_TABLE_WIDTH);
}

Menu_Item save_dialog_menu[] = {
    { 'H', string_ask_exit },
    { 's', string_ask_save },
    { 'd', string_ask_discharge },
};

static Menu_Dialog setup_menu_save_dialog(void)
{
    setup_menu_list(save_dialog_menu, sizeof(save_dialog_menu)/sizeof(save_dialog_menu[0]));

    do
    {
        switch (console_readkey(string_notice_input))
        {
            case 's':
                return Dialog_Result_Ok;
            case 'd':
                return Dialog_Result_Abort;
        }
    } while (1);
}

static void setup_menu_time(SETUP_Mode mode, unsigned char *hour, unsigned char *minute, unsigned char *second)
{
    if(mode == SETUP_Mode_Console)
    {
        do
        {
			printf("\n\r(%2u:%2u:%2u) -> %s: ", *hour, *minute, *second, string_input_time);
			
			if(scanf("%2hhu:%2hhu:%2hhu", hour, minute, second) == 3)
			{
				break;
			}
			uart_clear();
        } while (1);
    }
}

static SETUP_Update setup_menu_datetime(SETUP_Mode mode)
{
    unsigned char hour = mcp7940_hour(RTC_Current);
    unsigned char min = mcp7940_minute(RTC_Current);
    unsigned char sec = mcp7940_second();
    
    // Time
    setup_menu_time(mode, &hour, &min, &sec);

    unsigned char day = mcp7940_day(RTC_Current);
    unsigned char month = mcp7940_month(RTC_Current);
    unsigned char year = mcp7940_year();

    unsigned char weekday = 0;
    
    if(mode == SETUP_Mode_Console)
    {
        // Date
		do
		{
			printf("\n\r(%2u-%2u-%2u) -> %s: ", day, month, year, string_input_date);
			
			if(scanf("%2hhu-%2hhu-%2hhu", &day, &month, &year) == 3)
			{
				break;
			}
			uart_clear();
		} while (1);

        // Weekday
        do
        {
	        printf("\n\r%3s(%1u) -> %s: ", mcp7940_weekday_string(mcp7940_weekday(RTC_Current)), (mcp7940_weekday(RTC_Current)), string_input_weekday);
	        
	        if(scanf("%1hhu", &weekday) == 1)
	        {
		        break;
	        }
	        uart_clear();
        } while (1);

    }

    if(setup_menu_save_dialog() == Dialog_Result_Ok)
    {
        if(mcp7940_settime(hour, min, sec) == RTC_Fail ||
           mcp7940_setdate(day, month, year) == RTC_Fail ||
           mcp7940_setweekday(--weekday) == RTC_Fail)
        {
            printf("\n\r%s", string_notice_error);
        }
    }
    return SETUP_Update_None;
}

static void setup_menu_alarm(SETUP_Mode mode, unsigned char id)
{
    if(id >= ALARM_TABLE_MAX_LENGTH)
    {
        printf("\n\r%s", string_notice_range);
        return;
    }

    unsigned long selected_alarm = alarm_list_select(id);
    char input = '\0';
    char temp[8];

    unsigned char hour = alarm_hour(selected_alarm);
    unsigned char minute = alarm_minute(selected_alarm);
    unsigned char second = 0;
    unsigned char weekday = 0;
    unsigned char snooze_mode = 0;
    unsigned char snooze_minute = 0;

    if(mode == SETUP_Mode_Console)
    {
        setup_menu_time(mode, &hour, &minute, &second);

        printf("\n\r(+/-) -> %s: ", string_input_alarm_weekdays);
        
        if(scanf("%7s", temp) != 1)
        {
            uart_clear();
        }
        weekday = alarm_weekday_data(temp);

        
        printf("\n\r%s: ", string_input_snooze_mode);
        
        if(scanf("%1c", &input) != 1)
        {
            uart_clear();
        }

        snooze_mode = alarm_snooze_mode_data(input);

        printf("\n\r%s: ", string_input_snooze_time);
        
        if(scanf("%2hhu", &snooze_minute) != 1)
        {
            uart_clear();
        }

        snooze_minute = (0x0F & snooze_minute);
    }

    if(setup_menu_save_dialog() == Dialog_Result_Ok)
    {
        alarm_setalarm(id, hour, minute, snooze_minute, snooze_mode, weekday);
    }
}

Menu_Item alarms_menu[] = {
    { 'H', string_alarm },
    { '?', string_select_alarm },
    { 'c', string_clear_alarm },
    { 'q', string_ask_exit },
};

static SETUP_Update setup_menu_alarms(SETUP_Mode mode)
{
    char input[3];

    do
    {
        console_alarm_list(alarm_list_all(), ALARM_TABLE_MAX_LENGTH);
        setup_menu_list(alarms_menu, sizeof(alarms_menu)/sizeof(alarms_menu[0]));

        if(mode == SETUP_Mode_Console)
        {
            console_readline(string_notice_input, input, sizeof(input)/sizeof(input[0]));
        }

        switch (input[0])
        {
            case 'c':
                alarm_clear_all();
            break;
            default:
            {
                if(types_check_isnumeric(input))
                {
                    setup_menu_alarm(mode, atoi(input));
                }
            }
        }
    } while (input[0] != 'q');

    
    if(setup_menu_save_dialog() == Dialog_Result_Ok)
    {
        return SETUP_Update_Alarm;
    }
    return SETUP_Update_None;
}

Menu_Item sound_menu[] = {
    { 'H', string_alarm_tone },
    { 'p', string_sound_play },
    { 'e', string_sound_edit },
    { 'q', string_ask_exit },
};

static SETUP_Update setup_menu_sound(SETUP_Mode mode)
{
    char input = '\0';

    do
    {
        setup_menu_list(sound_menu, sizeof(sound_menu)/sizeof(sound_menu[0]));

        if(mode == SETUP_Mode_Console)
        {
            input = console_readkey(string_notice_input);
        }

        switch (input)
        {
            case 'p':
                sound_play(SOUND_ON);
                sound_play(SOUND_OFF);
            break;
            case 'e':
                printf("\n\r%s", string_press_q_key);

                for(unsigned char i=0; i < SOUND_FREQUENCIES_MAX_LENGTH; i++)
                {
                    printf("\n\r[%2u] %s (%u - %u): ", i, string_sound_frequency, SOUND_MIN_FREQUENCY, SOUND_MAX_FREQUENCY);
                    char buffer[6];

                    if(scanf("%5s", buffer) != 1)
                    {
                        uart_clear();
                        i--;
                        continue;
                    }

                    if(buffer[0] == 'q')
                    {
	                    uart_clear();
                        break;
                    }

                    if(!types_check_isnumeric(buffer))
                    {
                        uart_clear();
                        i--;
                        continue;
                    }

                    if(sound_create(i, atoi(buffer)) == SOUND_Range)
                    {
                        printf("\n\r%s", string_notice_range);

                        uart_clear();
                        i--;
                        continue;
                    }
                }
            break;
        }
    } while (input != 'q');
    
    if(setup_menu_save_dialog() == Dialog_Result_Ok)
    {
        return SETUP_Update_Alarm_Tone;
    }
    return SETUP_Update_None;
}

Menu_Item intensity_menu[] = {
    { 'H', string_intensity },
    { 'w', string_intensity_increase },
    { 's', string_intensity_decrease },
    { 'q', string_ask_exit },
};

static SETUP_Update setup_menu_intensity(SETUP_Mode mode)
{
    char input = '\0';
    unsigned int intensity = clock_intensity();

    setup_menu_list(intensity_menu, sizeof(intensity_menu)/sizeof(intensity_menu[0]));
    printf("\n\r%s (0x%04X): ", string_notice_input, intensity);

	clock_setdata(0x00000000, 0x0F0F0F0F0F0F0F0F, 0x0F);

    do
    {
        if(mode == SETUP_Mode_Console)
        {
            if(uart_scanchar(&input) == UART_Received)
            {
                printf("\r%s (0x%04X): ", string_notice_input, intensity);

                if(input == 'w')
                {
                    if(intensity <= CLOCK_INTENSITY_MAX)
                    {
                        intensity += 100;
                    }
                }
                else if(input == 's')
                {
                    if(intensity >= CLOCK_INTENSITY_MIN)
                    {
                        intensity -= 100;
                    }
                }
                clock_setintensity(intensity);
            }
        }
    } while (input != 'q');
    
	clock_setdata(0x00000000, 0x0000000000000000, 0x00);
	
    if(setup_menu_save_dialog() == Dialog_Result_Ok)
    {
        return SETUP_Update_Intensity;
    }
    return SETUP_Update_None;
}

Menu_Item oscillator_menu[] = {
    { 'H', string_oscillator },
    { 'm', string_oscillator_modify },
    { 'q', string_ask_exit },
};

static SETUP_Update setup_menu_oscillator(SETUP_Mode mode)
{
    char input[4];
    unsigned char trim = 0;

    do
    {
        setup_menu_list(oscillator_menu, sizeof(oscillator_menu)/sizeof(oscillator_menu[0]));

        if(mode == SETUP_Mode_Console)
        {
            input[0] = console_readkey(string_notice_input);
        }

        switch (input[0])
        {
            case 'm':
                printf("\n\r(%s): ", string_oscillator_trim);

                if(scanf("%4s", input) != 1)
                {
                    uart_clear();
                    break;
                }

                unsigned char negativ = 0;

                if(input[0] == '-');
                {
                    negativ = 1;
                    input[0] = '0';
                }

                if(!types_check_isnumeric(input))
                {
                    printf(" %s!", string_notice_notanumber);
                    break;
                }
                trim = (0x7F & atoi(input));

                if(negativ == 0)
                {
                    trim |= (1<<7);
                }
        }
    } while (input[0] != 'q');

    
    if(setup_menu_save_dialog() == Dialog_Result_Ok)
    {
        mcp7940_setoscillator(trim);
        return SETUP_Update_Oscillator;
    }
    return SETUP_Update_None;
}

Menu_Item string_main_menu[] = {
    { 'H', string_system_config },
    { '0', string_time_and_date },
    { '0', string_alarm },
    { '0', string_alarm_tone },
    { '0', string_intensity },
    { '0', string_oscillator },
    { 'd', string_ask_discharge },
    { 'q', string_ask_exit },
};

void setup_menu(SETUP_Mode *mode, SETUP_Update *update)
{	
	console_newline();
	setup_menu_list(string_main_menu, sizeof(string_main_menu)/sizeof(string_main_menu[0]));
	
    while (*mode == SETUP_Mode_Manual || *mode == SETUP_Mode_Console)
    {
        char input = '\0';

        if(*mode == SETUP_Mode_Console)
        {
            input = console_readkey(string_notice_input);
        }
        else if(*mode == SETUP_Mode_Manual)
        {
            unsigned char temp = 1;

            do 
            {
                if(input_status(INPUT_SW2))
                {
                    if(temp > 1)
                    {
                        _delay_ms(INPUT_DEBOUNCE_TIME);
                        temp--;
                        while(input_status(INPUT_SW2));
                    }
                }
                else if(input_status(INPUT_SW3))
                {
                    if(temp < sizeof(string_main_menu)/sizeof(string_main_menu[0]))
                    {
                        _delay_ms(INPUT_DEBOUNCE_TIME);
                        temp++;
                        while(input_status(INPUT_SW3));
                    }
                }
                _delay_ms(INPUT_DEBOUNCE_TIME);

                clock_setdata((1UL<<0), (1ULL<<temp), CLOCK_LED_Blue1);

            } while (!(input_status(INPUT_SW1)));

            if(temp == sizeof(string_main_menu)/sizeof(string_main_menu[0]))
            {
                input = 'q';
            }
            else if(temp == (sizeof(string_main_menu)/sizeof(string_main_menu[0]) - 1))
            {
                input = 'd';
            }
            else
            {
                input = '0' + temp;
            }
        }

        switch(input)
        {
            case '1':
                *update |= setup_menu_datetime(*mode);
            break;
            case '2':
                *update |= setup_menu_alarms(*mode);
            break;
            case '3':
                *update |= setup_menu_sound(*mode);
            break;
            case '4':
                *update |= setup_menu_intensity(*mode);
            break;
            case '5':
                *update |= setup_menu_oscillator(*mode);
            break;
            case 'd':
                *mode = SETUP_Mode_None;
            return;
            case 'q':
                *mode = SETUP_Mode_Done;
            return;
            default:
                printf(" %s!", string_notice_notfound);
			continue;
        }
		break;
    }
}

void setup_save(SETUP_Update update)
{
	if(update != SETUP_Update_None)
	{
		printf("\n\n\r%s", string_system_update);
		console_line(CONSOLE_LINE_LENGTH);

		if(update & SETUP_Update_Alarm)
		{
			printf("\n\r%s %s\t", string_system_updating, string_alarm);
			alarm_copy(ToEEPROM);
			printf(" %s", string_notice_done);
		}
		
		if(update & SETUP_Update_Alarm_Tone)
		{
			printf("\n\r%s %s\t", string_system_updating, string_alarm_tone);
			if(sound_copy(ToEEPROM) != SOUND_None)
			{
				printf(" %s", string_notice_error);
			}
			else
			{
				printf(" %s", string_notice_done);
			}
		}

		if(update & SETUP_Update_Intensity)
		{
			printf("\n\r%s %s\t", string_system_updating, string_intensity);
			clock_copy_intensity(ToEEPROM);
			printf(" %s", string_notice_done);
		}
		
		if(update & SETUP_Update_Oscillator)
		{
			printf("\n\r%s %s\t", string_system_updating, string_oscillator);
			mcp7940_oscillator_copy(ToEEPROM);
			printf(" %s", string_notice_done);
		}
		console_newline();
	}
}