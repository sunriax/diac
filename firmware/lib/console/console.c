
#include "console.h"

unsigned char EEMEM ee_project[] = "\n\rDIU - Digitaluhr";
unsigned char EEMEM ee_creator[] = "\n\rAuthor: Ing. Raffael GAECHTER";
unsigned char EEMEM ee_copyright[] = "\n\r(c) 2024, All rights reserved!";
unsigned char EEMEM ee_version = 0x10;

unsigned char version;
char buffer[CONSOLE_BUFFER_LENGTH];

void console_init(void)
{
    uart_init();
    console_clear();

    eeprom_read_block(buffer, ee_project, sizeof(ee_project));
    printf("\n\n\r%s\n\r", buffer);
    
    console_line(CONSOLE_LINE_LENGTH);
    
    version = eeprom_read_byte(&ee_version);
    printf("\n\rVersion: %1u.%1u", (version>>4), (0x0F & version));
    eeprom_read_block(buffer, ee_creator, sizeof(ee_creator));
    printf("%s", buffer);
    eeprom_read_block(buffer, ee_copyright, sizeof(ee_copyright));
    printf("%s", buffer);
    
    console_line(CONSOLE_LINE_LENGTH);
}

void console_newline(void)
{
    uart_putchar('\n');
    uart_putchar('\r');
}

void console_clear(void)
{
    printf("\e[1;1H\e[2J");
}

void console_line(unsigned char length)
{
    console_newline();

    for (unsigned char i=0; i < length; i++)
    {
        uart_putchar(CONSOLE_LINE_CHARACTER);
    }
}

void console_alarm(unsigned char id, unsigned long alarm, CONSOLE_Border border)
{
    if(border & CONSOLE_Alarm_Header)
    {
		console_newline();
        printf("\n\r+----+-ALARM-+-WEEKDAY-+---SNOOZE---+");
        printf("\n\r| ID | hh:mm | MTWTFSS | E531 | MIN |");
        printf("\n\r+----+-------+---------+------+-----+");
    }
    
    char weekday_string[(ALARM_WEEKDAY_FIELD_LENGTH + 1)];
    char snooze_mode_string[(ALARM_SNOOZE_MODE_FIELD_LENGTH + 1)];

    alarm_weekday_string(alarm, weekday_string);
    alarm_snooze_mode_string(alarm, snooze_mode_string);

    printf("\n\r| %2u | %2u:%2u | %7s | %3s | %3u |",
    id,
    alarm_hour(alarm),
    alarm_minute(alarm),
    weekday_string,
    snooze_mode_string,
    alarm_snooze_minutes(alarm)
    );

    if(border & CONSOLE_Alarm_Footer)
    {
        printf("\n\r+----+-------+---------+------+-----+");
    }
}

void console_alarm_list(unsigned long *alarm, unsigned char length)
{
    for (unsigned char i=0; i < length; i++)
    {
	    CONSOLE_Border border = CONSOLE_Alarm_None;
		
		if(i == 0)
		{
			border = CONSOLE_Alarm_Header;
		}
		else if (i == (length - 1))
		{
			border = CONSOLE_Alarm_Footer;
		}
        console_alarm(i, alarm[i], border);
    }
}

void console_table_border(unsigned char length)
{
    console_newline();

    for (unsigned char x=0; x < length; x++)
    {
        if(x == 0)
        {
            uart_putchar(CONSOLE_TABLE_CORNER_CHARACTER);
        }
        else if(x == (length - 1))
        {
            uart_putchar(CONSOLE_TABLE_CORNER_CHARACTER);
        }
        else
        {
            uart_putchar(CONSOLE_TABLE_LINE_CHARACTER);
        }
    }
}

void console_table_column(const char *text, unsigned char length)
{
    console_newline();

    for (unsigned char x=0; x < length; x++)
    {
        if((x == 0) || (x == (length - 1)))
        {
            uart_putchar(CONSOLE_TABLE_COLUMN_CHARACTER);
        }
        else if(x == 1)
        {
            uart_putchar(SPACE_CHARACTER);
        }
        else
        {
            if(*text != '\0')
            {
                uart_putchar(*(text++));
            }
            else
            {
                uart_putchar(SPACE_CHARACTER);
            }
        }
    }
}

void console_table_header(const char *text, unsigned char length)
{
    console_table_border(length);
    console_table_column(text, length);
    console_table_border(length);
}

char console_readkey(const char *input)
{
    char temp = '\0';

    printf("\n\r%s: ", input);

    if(scanf("%c", &temp) != 1)
    {
        uart_clear();
    }
    return temp;
}

void console_readline(const char *input, char *buffer, unsigned char length)
{
    printf("\n\r%s: ", input);

    UART_Data status;

    for (unsigned char i=0; i < length; i++)
    {
        buffer[i] = uart_getchar(&status);

        if(buffer[i] == '\r' || status == UART_Fault)
        {
            buffer[i] = '\0';
            break;
        }
    }
}