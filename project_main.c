#include <LPC214x.h>
#include "types.h"
#include "delay.h"

#include "lcd_defines.h"
#include "lcd.h"

#include "adc_defines.h"
#include "adc.h"

#include "kpm_defines.h"
#include "kpm.h"

#include "rtc_defines.h"
#include "rtc.h"

#include "eint_defines.h"
#include "eint.h"

#include "buzzer_defines.h"
#include "buzzer.h"

// --------------------- Global Variables ---------------------
volatile u32 menu_mode = 0;
u32 alarm_hour = 0, alarm_min = 0;

// ------------------------------------------------------------

int main(void)
{
    s32 hour, minute, sec;
    u8 alarm_triggered = 0;
		u8 alarm_stopped = 0;
    u32 i;
		u8 msg[]="WELCOME";
		u8 id[]="V24HE11B11";

    // --------------------- Initialization ---------------------
    Init_LCD();
    Init_adc();
    Init_KPM();
    Init_rtc();
    Init_eint();
    Init_Buzzer();
    LCD_CreateCustomChars();	
	//	create_degreeC_symbol();
		lcd_uparrow();
		lcd_downarrow();
		
	cmdLCD(CLEAR_LCD);  // clear LCD
    cmdLCD(GOTO_LINE1_POS0);
		strLCD("    ");

    for(i = 0; msg[i] != '\0'; i++)
    {
        charLCD(msg[i]);  
  	delayMS(100);     
    }
		delayMS(500);
		
		cmdLCD(GOTO_LINE2_POS0);
		strLCD("     -");

    for(i = 0; id[i] != '\0'; i++)
    {
        charLCD(id[i]);   
        delayMS(100);     
    }
		delayMS(1000);



    // Initial RTC setup (example time)
    SetRTCTimeInfo(22, 40, 00);
    SetRTCDateInfo(29, 10, 2025);
    SetRTCDay(3);

   
    IODIR1 &= ~(1 << STOP_BUTTON);  // P1.28 input
    IOSET1  |=  (1 << STOP_BUTTON); // internal pull-up
	
    cmdLCD(CLEAR_LCD);
    display_time();

    // --------------------- Main Loop ---------------------
    while (1)
    {
        GetRTCTimeInfo(&hour, &minute, &sec);
	 Buzzer_OFF();
        // Menu trigger
        if (menu_mode == 1)
        {
            show_menu();
            menu_mode = 0;
        }

        // --------------------- Alarm Trigger ---------------------
if (hour == alarm_hour && minute == alarm_min && alarm_triggered == 0 && alarm_stopped == 0)
{
    alarm_triggered = 1;
    Buzzer_ON();

    cmdLCD(CLEAR_LCD);
    cmdLCD(GOTO_LINE1_POS0);
    charLCD(0); 
    strLCD(" ALARM RINGING!");

    for (i = 0; i < 60; i++)
    {
        delayMS(1000);
        GetRTCTimeInfo(&hour, &minute, &sec);

        cmdLCD(GOTO_LINE2_POS0);
        strLCD("TIME: ");
        u32LCD(hour);
        charLCD(':');
        u32LCD(minute);
        charLCD(':');
        u32LCD(sec);

        // ---- Auto stop if minute changes ----
        if (minute != alarm_min)
            break;

        // ---- Check STOP Button ----
        if ((IOPIN1 & (1 << STOP_BUTTON)) == 0)  // active LOW
        {
            Buzzer_OFF();
            alarm_triggered = 0;
            alarm_stopped = 1;   

            cmdLCD(CLEAR_LCD);
            strLCD("ALARM STOPPED");
            delayMS(1000);
            cmdLCD(CLEAR_LCD);
            display_time();
            break;
        }
    }

    Buzzer_OFF();
    cmdLCD(CLEAR_LCD);
}

// ---- Reset flags after time passes ----
if (minute != alarm_min || hour != alarm_hour)
{
    alarm_triggered = 0;
    alarm_stopped = 0;
}

// Normal clock display update
if (!alarm_triggered)
{
    display_time();
    delayMS(1000);
}

}
}

