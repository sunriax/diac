
#include "mcp7940.h"

unsigned char EEMEM ee_mcp7940_oscillator = 0x00;
unsigned char oscillator = 0x00;

const char weeksdays[][4] = {
    "MON",
    "TUE",
    "WED",
    "THU",
    "FRI",
    "SAT",
    "SUN",
    "???"
};

const char* mcp7940_weekday_string(unsigned char day)
{
    return weeksdays[(0x07 & (day - 1))];
}

static void mcp7940_write(unsigned char address, unsigned char data)
{
    twi_start();
    twi_address(MCP7940_ADDRESS, TWI_WRITE);
    twi_set(address);
    twi_set(data);
    twi_stop();
    
    _delay_ms(1);
}

static unsigned char mcp7940_read(unsigned char address)
{
    unsigned char temp;
    
    twi_start();
    twi_address(MCP7940_ADDRESS, TWI_WRITE);
    twi_set(address);
    twi_address(MCP7940_ADDRESS, TWI_READ);
    twi_get(&temp, TWI_NACK);
    twi_stop();
    
    _delay_ms(1);
    
    return temp;
}

static unsigned char mcp7940_tobinary(unsigned char value, unsigned char mask)
{
    return (((mask & value)>>4) * 10UL + (0x0F & value));
}

static unsigned char mcp7940_data(unsigned char address, unsigned char mask)
{
    return mcp7940_tobinary(mcp7940_read(address), mask);
}

void mcp7940_init(RTC_Mode battery)
{
    twi_init();

    mcp7940_oscillator_copy(ToBUFFER);

    if(oscillator > 0)
    {
        mcp7940_setoscillator(oscillator);
    }

    if(battery == RTC_Enable)
    {
        // This funcion clears the powerfail bit
        // The power down timestamp can only be
        // red if the battery is manually enabled!
        mcp7940_battery(RTC_Enable);
    }
    mcp7940_oscillator(RTC_Enable);

    unsigned char temp = mcp7940_read(MCP7940_CONTROL);
    mcp7940_write(MCP7940_CONTROL, (RTC_SQWEN_bm | MCP7940_MFP_PORT | temp));
}

unsigned char mcp7940_oscillator_value(void)
{
    return oscillator;
}

void mcp7940_oscillator_copy(Copy_Mode mode)
{
    switch (mode)
    {
        case ToEEPROM:
            eeprom_write_byte(&ee_mcp7940_oscillator, oscillator);
        break;
        default:
            oscillator = eeprom_read_byte(&ee_mcp7940_oscillator);
        break;
    }
}

void mcp7940_battery(RTC_Mode mode)
{
    unsigned char temp = mcp7940_read(MCP7940_RTCWKDAY);
    
    if(mode == RTC_Enable)
    {
        mcp7940_write(MCP7940_RTCWKDAY, (RTC_VBATEN_bm | temp));
        return;
    }
    mcp7940_write(MCP7940_RTCWKDAY, ((~RTC_VBATEN_bm) & temp));
}

RTC_Status mcp7940_powerfail_status(void)
{
    unsigned char temp = mcp7940_read(MCP7940_RTCWKDAY);
    
    if(temp & RTC_PWRFAIL_bm)
    {
        return RTC_Power_Fail;
    }
    
    return RTC_Disabled;
}

void mcp7940_oscillator(RTC_Mode mode)
{
    unsigned char temp = mcp7940_read(MCP7940_RTCSEC);
    
    if(mode == RTC_Enable)
    {
        mcp7940_write(MCP7940_RTCSEC, (RTC_ST_bm | temp));
        return;
    }
    mcp7940_write(MCP7940_RTCSEC, ((~RTC_ST_bm) & temp));
}

RTC_Status mcp7940_oscillator_status(void)
{
    unsigned char temp = mcp7940_read(MCP7940_RTCWKDAY);
    
    if(temp & RTC_OSCRUN_bm)
    {
        return RTC_Running;
    }
    return RTC_Disabled;
}

unsigned char mcp7940_hour(RTC_Data data)
{
    unsigned char temp;

    switch (data)
    {
        case RTC_Power_Down:
			temp = MCP7940_PWRDNHOUR;
        break;
        case RTC_Power_Up:
			temp = MCP7940_PWRUPHOUR;
        break;
        default:
			temp = MCP7940_RTCHOUR;
        break;
    }
    return mcp7940_data(temp, RTC_HRTEN_bm);
}

unsigned char mcp7940_minute(RTC_Data data)
{
    unsigned char temp;

    switch (data)
    {
        case RTC_Power_Down:
			temp = MCP7940_PWRDNMIN;
        break;
        case RTC_Power_Up:
			temp = MCP7940_PWRUPMIN;
        break;
        default:
			temp = MCP7940_RTCMIN;
        break;
    }
    return mcp7940_data(temp, RTC_MINTEN_bm);
}

unsigned char mcp7940_second(void)
{
    return mcp7940_data(MCP7940_RTCSEC, RTC_SECTEN_bm);
}

unsigned char mcp7940_weekday(RTC_Data data)
{
    switch (data)
    {
        case RTC_Power_Down:
			return ((0xE0 & mcp7940_read(MCP7940_PWRDNMTH))>>MCP7940_PWRWEEKDAY_bp);
        case RTC_Power_Up:
			return ((0xE0 & mcp7940_read(MCP7940_PWRUPMTH))>>MCP7940_PWRWEEKDAY_bp);
        default:
			return (0x07 & mcp7940_read(MCP7940_RTCWKDAY));
    }
}

unsigned char mcp7940_day(RTC_Data data)
{
    unsigned char temp;

    switch (data)
    {
        case RTC_Power_Down:
			temp = MCP7940_PWRDNDATE;
        break;
        case RTC_Power_Up:
			temp = MCP7940_PWRUPDATE;
        break;
        default:
			temp = MCP7940_RTCDATE;
        break;
    }
    return mcp7940_data(temp, RTC_DATETEN_bm);
}

unsigned char mcp7940_month(RTC_Data data)
{
    unsigned char temp;

    switch (data)
    {
        case RTC_Power_Down:
			temp = MCP7940_PWRDNMTH;
        break;
        case RTC_Power_Up:
			temp = MCP7940_PWRUPMTH;
        break;
        default:
			temp = MCP7940_RTCMTH;
        break;
    }
    return mcp7940_data(temp, RTC_MTHTEN_bm);
}

unsigned char mcp7940_year(void)
{
    return mcp7940_data(MCP7940_RTCYEAR, RTC_YRTEN_bm);
}

unsigned char mcp7940_leapyear(void)
{
    return ((0x07 & mcp7940_read(RTC_LPYR_bm))>>RTC_LPYR_bp);
}

RTC_Status mcp7940_setweekday(unsigned char weekday)
{
    if(weekday >= 7)
    {
        return RTC_Fail;
    }
    
    unsigned char temp = mcp7940_read(MCP7940_RTCWKDAY);
    mcp7940_write(MCP7940_RTCWKDAY, ((0xF8 & temp) | (0x07 & (weekday + 1))));
    
    return RTC_Valid;
}

static unsigned char mcp7940_tobcd(unsigned char value)
{
    return ((value / 10)<<4) | (value%10);
}

static void mcp7940_setdata(
    unsigned char address1,
    unsigned char value1,
    unsigned char address2,
    unsigned char value2,
    unsigned char address3,
    unsigned char value3)
{
    mcp7940_write(address1, mcp7940_tobcd(value1));
    mcp7940_write(address2, mcp7940_tobcd(value2));
    mcp7940_write(address3, mcp7940_tobcd(value3));
}

RTC_Status mcp7940_settime(unsigned char hour, unsigned char minute, unsigned char second)
{
    if((hour > 23) || (minute > 59) || (second > 59))
    {
        return RTC_Fail;
    }
    
    mcp7940_setdata(
        MCP7940_RTCHOUR, hour,
        MCP7940_RTCMIN,  minute,
        MCP7940_RTCSEC,  second
    );
	mcp7940_oscillator(RTC_Enable);
    return RTC_Valid;
}

RTC_Status mcp7940_setdate(unsigned char day, unsigned char month, unsigned char year)
{
    if((day > 31) || (month > 12) || (year > 99))
    {
        return RTC_Fail;
    }
    
    mcp7940_setdata(
        MCP7940_RTCDATE, day,
        MCP7940_RTCMTH,  month,
        MCP7940_RTCYEAR, year
    );
    return RTC_Valid;
}

RTC_Status mcp7940_setoscillator(unsigned char trim)
{
    oscillator = trim;
    mcp7940_write(MCP7940_OSCTRIM, trim);
    
    if(mcp7940_read(MCP7940_OSCTRIM) == trim)
    {
        return RTC_Valid;
    }
    return RTC_Fail;
}

