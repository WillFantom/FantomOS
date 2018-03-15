#ifndef __RTC_H
#define __RTC_H

#include <core/Common.h>

#define CMOS_CONTROL 0x70
#define CMOS_DATA 0x71

#define CMOS_STAT_A 0x0A
#define CMOS_STAT_B 0x0B

#define RTC_SECOND 0x00
#define RTC_MINUTE 0x02
#define RTC_HOUR 0x04

typedef struct RTC_time
{
    uint8_t hour;
    uint8_t minute;
    uint8_t second;
} RTC_time_t;

bool RTC_init();
void RTC_printTime();

#endif
