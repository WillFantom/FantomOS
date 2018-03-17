#include <modules/drivers/RTC.h>


#include <core/debug/Common.h>
#include <core/debug/Debug.h>

RTC_time_t currentTime;

bool RTC_init()
{
    return true;
}

bool RTC_updateInProgress()
{
    outb(CMOS_CONTROL, CMOS_STAT_A);
    return inb(CMOS_DATA) & (0x1 << 7);
}

uint8_t RTC_getValue(uint8_t reg)
{
    outb(CMOS_CONTROL, reg);
    return inb(CMOS_DATA);
}

uint8_t RTC_bcdConvert(uint8_t bcd)
{
    return ((bcd & 0xF0) >> 1) + ((bcd & 0xF0) >> 3) + (bcd & 0xF);
}

bool RTC_bcdMode()
{
    outb(CMOS_CONTROL, CMOS_STAT_B);
    return inb(CMOS_DATA) & (0x1 << 2);
}

void RTC_printTime()
{
    while(RTC_updateInProgress());

    if(RTC_bcdMode)
    {
        currentTime.second = RTC_bcdConvert(RTC_getValue(RTC_SECOND));
        currentTime.minute = RTC_bcdConvert(RTC_getValue(RTC_MINUTE));
        currentTime.hour = RTC_bcdConvert(RTC_getValue(RTC_HOUR));
    }
    else
    {
        currentTime.second = (RTC_getValue(RTC_SECOND));
        currentTime.minute = (RTC_getValue(RTC_MINUTE));
        currentTime.hour = (RTC_getValue(RTC_HOUR));
    }
    kprintf("Time %d:%d:%d \n", currentTime.hour,currentTime.minute,currentTime.second);
}
