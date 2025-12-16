#include <LPC214x.h>
#include "types.h"
#include "rtc_defines.h"
#include "rtc.h"
#include "lcd_defines.h"
#include "lcd.h"

s32 hour,min,sec,date,month,year,day;


s8 week[][4] = {"SUN","MON","TUE","WED","THU","FRI","SAT"};

void Init_rtc(void){
	CCR = RTC_RST;
	//PREINT = PREINT_VAL;
	//PREFRAC = PREFRAC_VAL;
	  PREINT  = 0x000001C8;      // 456
    PREFRAC = 0x000061C0;      // 25024
	CCR = RTC_ENABLE;
}

void GetRTCTimeInfo(s32 *hour,s32 *minute,s32 *second){
	*hour = HOUR;
	*minute = MIN;
	*second = SEC;
}

void DisplayRTCTime(s32 hour,s32 minute,s32 second){
	

	charLCD(((hour/10))+48);
	charLCD(((hour%10))+48);
	charLCD(':');
	charLCD(((minute/10))+48);
	charLCD(((minute%10))+48);
	charLCD(':');
	charLCD(((second/10))+48);
	charLCD(((second%10))+48);
}

void GetRTCDateInfo(s32 *date,s32 *month,s32 *year){
	*date =  DOM;
	*month = MONTH;
	*year = YEAR;
}
void DisplayRTCDate(u32 date,u32 month,u32 year){
	

	charLCD(((date/10))+48);
	charLCD(((date%10))+48);
	charLCD('/');
	charLCD(((month/10))+48);
	charLCD(((month%10))+48);
	charLCD('/');
	u32LCD(year);
	
}

void SetRTCTimeInfo(u32 hour,u32 minute,u32 second){
	HOUR = hour;
	MIN = minute;
	SEC = second;
}

void SetRTCDateInfo(u32 date,u32 month,u32 year){
	DOM = date;
	MONTH = month;
	YEAR = year;
	
}

void GetRTCDay(s32 *day){
	*day=DOW;
}
void DisplayRTCDay(u32 day){
	
	strLCD(week[day]);
}

void SetRTCDay(u32 day){
	DOW = day;
}
