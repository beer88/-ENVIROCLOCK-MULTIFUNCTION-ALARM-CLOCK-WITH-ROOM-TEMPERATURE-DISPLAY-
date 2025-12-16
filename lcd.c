//lcd.c
#include "types.h"
#include "lcd_defines.h"
#include "delay.h"
#include "defines.h"
#include <lpc213x.h>
#include "lcd.h"
#include "rtc.h"
#include "kpm.h"
#include "adc.h"

extern u32 alarm_hour, alarm_min, alarm_sec;

const u8 BellSymbol[8] = {0x04,0x0E,0x0E,0x0E,0x1F,0x04,0x00,0x04};
//u8 degreeC_symbol[8] = {0x00,0x06,0x09,0x06,0x08,0x08,0x07,0x00};
u8 up_arrow[8] = {0x04, 0x0E, 0x15, 0x04, 0x04, 0x04, 0x04, 0x00};
u8 down_arrow[8] = {0x04, 0x04, 0x04, 0x04, 0x15, 0x0E, 0x04, 0x00};

void Init_LCD(void) {
    WRITEBYTE(IODIR0, LCD_DATA, 0xFF);
    SETBIT(IODIR0, LCD_RS);
    SETBIT(IODIR0, LCD_EN);
    SETBIT(IODIR0, LCD_RW);
    delayMS(15);
    cmdLCD(0x30);
    delayMS(5);
    cmdLCD(0x30);
    delayUS(100);
    cmdLCD(0x30);
    cmdLCD(MODE_8BIT_2LINE);
    cmdLCD(SHIFT_CUR_RIGHT);
    cmdLCD(DSP_ON_CUR_OFF);   //  No blinking cursor
    cmdLCD(CLEAR_LCD);
}

void writeLCD(u8 dat) {
    WRITEBYTE(IOPIN0, LCD_DATA, dat);
    SSETBIT(IOSET0, LCD_EN);
    delayUS(1);
    SCLRBIT(IOCLR0, LCD_EN);
    delayMS(2);
}

void cmdLCD(u8 cmd) {
    SCLRBIT(IOCLR0, LCD_RS);
    writeLCD(cmd);
}

void charLCD(u8 ascii) {
    SSETBIT(IOSET0, LCD_RS);
    writeLCD(ascii);
}

void strLCD(s8 *p) {
    while(*p) {
        charLCD(*p++);
    }
}

void u32LCD(u32 n) {
    u8 a[10];
    s32 i=0;
    if(n==0) {
        charLCD('0');
    } else {
        while(n>0) {
            a[i++]=(n%10)+48;
            n/=10;
        }
        for(--i;i>=0;i--) {
            charLCD(a[i]);
        }
    }
}

void s32LCD(s32 n) {
    if(n==0)
        charLCD('0');
    else if(n<0) {
        charLCD('-');
        n=-n;
        u32LCD(n);
    } else {
        u32LCD(n);
    }
}

void f32LCD(f32 f, u8 ndp) {
    u32 i,j;
    if(f<0.0) {
        charLCD('-');
        f=-f;
    }
    i=f;
    u32LCD(i);
    charLCD('.');
    for(j=0;j<ndp;j++) {
        f=(f-i)*10;
        i=f;
        charLCD(i+48);
    }
}
void LCD_CreateCustomChars(void)
{
	  u8 i;
    cmdLCD(0x40);  // Set CGRAM address to 0
    for (i = 0; i < 8; i++)
        charLCD(BellSymbol[i]);
    cmdLCD(0x80);  // Return to DDRAM (normal display)
}
/*void create_degreeC_symbol(void)
{
    u8 i;
    cmdLCD(0x48);
    for(i = 0; i < 8; i++)
        charLCD(degreeC_symbol[i]);
    cmdLCD(0x80);
}*/

void lcd_uparrow(void)
{
    u8 i;
    
        cmdLCD(0x40 + (2 * 8));  // Set CGRAM address
        for (i = 0; i < 8; i++)
            charLCD(up_arrow[i]);       // Write 8 bytes to CGRAM
    
}
void lcd_downarrow(void)
{
    u8 i;
    
        cmdLCD(0x40 + (3 * 8));  // Set CGRAM address
        for (i = 0; i < 8; i++)
            charLCD(down_arrow[i]);       // Write 8 bytes to CGRAM
    
}



void display_time(void) {
    s32 hour, minute, sec;
    s32 date, month, year;
    s32 weekday;
    f32 temperature;


    GetRTCTimeInfo(&hour, &minute, &sec);
    GetRTCDateInfo(&date, &month, &year);
    GetRTCDay(&weekday);


    // ---- Line 1 ----
		cmdLCD(GOTO_LINE1_POS0);
		DisplayRTCTime(hour, minute, sec);

		Read_LM35_Temperature(1, &temperature);
		cmdLCD(GOTO_LINE1_POS0 + 10);
		strLCD("T:");
		s32LCD(temperature);
		cmdLCD(GOTO_LINE1_POS0 + 14);
		//charLCD(1);   // Custom symbol °C
		charLCD(0xDF);   // Display degree symbol (°)
		strLCD("C");     

    cmdLCD(GOTO_LINE2_POS0);
    DisplayRTCDate(date, month, year);

   
    cmdLCD(GOTO_LINE2_POS0 + 12);
		DisplayRTCDay(weekday);
}

void edit_time(void)
{
    s32 hour, min, sec;
    u32 key;
    u8 menu_pos = 0;  

    // Read current RTC time
    GetRTCTimeInfo(&hour, &min, &sec);

    while (1)
    {
        // --- Display main menu ---
        cmdLCD(CLEAR_LCD);
        strLCD("> Edit Hour");

        cmdLCD(GOTO_LINE2_POS0);
        if (menu_pos == 1) strLCD("> Edit Minute");
        else if (menu_pos == 2) strLCD(">Save & Exit");
        else strLCD("  Edit Minute");

        // Move cursor/arrow display correctly
        if (menu_pos == 0)
        {
            cmdLCD(CLEAR_LCD);
            strLCD("> Edit Hour");
            cmdLCD(GOTO_LINE2_POS0);
            strLCD("  Edit Minute");
        }
        else if (menu_pos == 1)
        {
            cmdLCD(CLEAR_LCD);
            strLCD("  Edit Hour");
            cmdLCD(GOTO_LINE2_POS0);
            strLCD("> Edit Minute");
        }
        else if (menu_pos == 2)
        {
            cmdLCD(CLEAR_LCD);
            strLCD("  Edit Minute");
            cmdLCD(GOTO_LINE2_POS0);
            strLCD("> Save & Exit ");
        }

        key = Read_KPM_Key();

        // --- Menu navigation ---
        if (key == '8')   // Move up
        {
            if (menu_pos == 0)
                menu_pos = 2;
            else
                menu_pos--;
        }
        else if (key == '2')  // Move down
        {
            menu_pos = (menu_pos + 1) % 3;
        }
        else if (key == '5')  // Enter key
        {
            if (menu_pos == 0)  // Edit Hour
            {

                cmdLCD(CLEAR_LCD);
                strLCD("Edit Hour:");
                cmdLCD(GOTO_LINE2_POS0);
                cmdLCD(0x0C);  // Cursor ON

                while (1)
                {
                    // --- Display hour with up/down arrows ---
										
										cmdLCD(GOTO_LINE2_POS0);
										strLCD("    ");
										charLCD(2);               // Up arrow at position 0
										strLCD(" ");              // Space
										if (hour < 10) charLCD('0');
										u32LCD(hour);             // Hour at position 2–3
										strLCD(" ");              // Space
										charLCD(3);               // Down arrow at position 6


                    key = Read_KPM_Key();

                    if (key == '8')
                        hour = (hour + 1) % 24;
                    else if (key == '2')
                        hour = (hour == 0) ? 23 : hour - 1;
                    else if (key == '5')
                        break;

                    delayMS(200);
                }
            }
            else if (menu_pos == 1)  // Edit Minute
            {

                cmdLCD(CLEAR_LCD);
                strLCD("Edit Minute:");
                cmdLCD(GOTO_LINE2_POS0);
                cmdLCD(0x0C);  // Cursor ON

                while (1)
                {
                    cmdLCD(GOTO_LINE2_POS0);
										strLCD("    ");
										charLCD(2);               // Up arrow at position 0
										strLCD(" ");             
                    if (min < 10) charLCD('0');
										u32LCD(min);             // Hour at position 2–3
										strLCD(" ");              // Space
										charLCD(3);

                    key = Read_KPM_Key();

                    if (key == '8')
                        min = (min + 1) % 60;
                    else if (key == '2')
                        min = (min == 0) ? 59 : min - 1;
                    else if (key == '5')
                        break;

                    delayMS(200);
                }
            }
            else if (menu_pos == 2)  // Exit
            {
                break;
            }

            // After each edit, update RTC
            SetRTCTimeInfo(hour, min, sec);
            cmdLCD(0x0C);   // Cursor OFF
            cmdLCD(CLEAR_LCD);
            strLCD("UPDATED!");
            delayMS(500);
        }

        delayMS(200); // debounce
    }

    cmdLCD(CLEAR_LCD);
    strLCD("EXITING MENU...");
    delayMS(500);
    cmdLCD(CLEAR_LCD);
}
void set_alarm(void)
{
    u32 key;
    u8 menu_pos = 0;  // 0 = Edit Hour, 1 = Edit Minute, 2 = Exit
    u32 hour = alarm_hour;
    u32 min  = alarm_min;

    // Ensure valid initial values
    if (hour > 23) hour = 0;
    if (min  > 59) min  = 0;

    while (1)
    {
        // --- Display menu ---
        if (menu_pos == 0)
        {
            cmdLCD(CLEAR_LCD);
            strLCD("> Set Hour");
            cmdLCD(GOTO_LINE2_POS0);
            strLCD("  Set Minute");
        }
        else if (menu_pos == 1)
        {
            cmdLCD(CLEAR_LCD);
            strLCD("  Set Hour");
            cmdLCD(GOTO_LINE2_POS0);
            strLCD("> Set Minute");
        }
        else if (menu_pos == 2)
        {
            cmdLCD(CLEAR_LCD);
            strLCD("  Set Minute");
            cmdLCD(GOTO_LINE2_POS0);
            strLCD("> Exit");
        }

        key = Read_KPM_Key();

        // --- Navigation keys ---
        if (key == '8')   // Up
        {
            if (menu_pos == 0)
                menu_pos = 2;
            else
                menu_pos--;
        }
        else if (key == '2')  // Down
        {
            menu_pos = (menu_pos + 1) % 3;
        }
        else if (key == '5')  // Enter
        {
            if (menu_pos == 0)  // Set Hour
            {

                cmdLCD(CLEAR_LCD);
                strLCD("Set Hour:");
                cmdLCD(GOTO_LINE2_POS0);
                cmdLCD(0x0C);

                while (1)
                {
                    cmdLCD(GOTO_LINE2_POS0);
										strLCD("    ");
										charLCD(2);
										strLCD(" ");
										if (hour < 10) charLCD('0');
                    u32LCD(hour);
									  strLCD(" ");
										charLCD(3);

                    key = Read_KPM_Key();

                    if (key == '8')
                        hour = (hour + 1) % 24;
                    else if (key == '2')
                        hour = (hour == 0) ? 23 : hour - 1;
                    else if (key == '5')
                        break;

                    delayMS(200);
                }
            }
            else if (menu_pos == 1)  // Set Minute
            {

                cmdLCD(CLEAR_LCD);
                strLCD("Set Minute:");
                cmdLCD(GOTO_LINE2_POS0);
                cmdLCD(0x0C);

                while (1)
                {
                    cmdLCD(GOTO_LINE2_POS0);
										strLCD("    ");
										charLCD(2);               // Up arrow at position 0
										strLCD(" ");              
                    if (min < 10) charLCD('0');
										u32LCD(min);
										strLCD(" ");
										charLCD(3);

                    key = Read_KPM_Key();

                    if (key == '8')
                        min = (min + 1) % 60;
                    else if (key == '2')
                        min = (min == 0) ? 59 : min - 1;
                    else if (key == '5')
                        break;

                    delayMS(200);
                }
            }
            else if (menu_pos == 2)  // Exit
            {
                break;
            }

            // After each edit, update alarm values
            alarm_hour = hour;
            alarm_min = min;

            cmdLCD(0x0C);   // Cursor OFF
            cmdLCD(CLEAR_LCD);
            strLCD("UPDATED!");
            delayMS(500);
						cmdLCD(CLEAR_LCD);
        }

        delayMS(200);  // debounce
    }

    cmdLCD(CLEAR_LCD);
    strLCD("EXITING...");
    delayMS(500);
    cmdLCD(CLEAR_LCD);
}
u8 GetMaxDays(u8 month, u16 year)
{
    switch (month)
    {
        case 1: case 3: case 5: case 7: case 8: case 10: case 12:
            return 31;
        case 4: case 6: case 9: case 11:
            return 30;
        case 2:
            if ((year % 400 == 0) || ((year % 4 == 0) && (year % 100 != 0)))
                return 29;
            else
                return 28;
        default:
            return 31;
    }
}
void edit_date(void)
{
    s32 date, month, year;
    u32 key;
    u8 menu_pos = 0;  // 0=Day, 1=Month, 2=Year, 3=Exit

    // Read current RTC date
    GetRTCDateInfo(&date, &month, &year);

    if (date == 0 || date > 31) date = 1;
    if (month == 0 || month > 12) month = 1;
    if (year < 2000) year = 2025; // default safe year

    while (1)
    {
        // --- Display main menu ---
        if (menu_pos == 0)
        {
            cmdLCD(CLEAR_LCD);
            strLCD("> Set Date");
            cmdLCD(GOTO_LINE2_POS0);
            strLCD("  Set Month");
        }
        else if (menu_pos == 1)
        {
            cmdLCD(CLEAR_LCD);
            strLCD("  Set Date");
            cmdLCD(GOTO_LINE2_POS0);
            strLCD("> Set Month");
        }
        else if (menu_pos == 2)
        {
            cmdLCD(CLEAR_LCD);
            strLCD("  Set Month");
            cmdLCD(GOTO_LINE2_POS0);
            strLCD("> Set Year");
        }
        else if (menu_pos == 3)
        {
            cmdLCD(CLEAR_LCD);
            strLCD("  Set Year");
            cmdLCD(GOTO_LINE2_POS0);
            strLCD("> Exit");
        }

        // --- Wait for key ---
        key = Read_KPM_Key();

        // --- Navigation ---
        if (key == '8')  // Up
        {
            if (menu_pos == 0)
                menu_pos = 3;
            else
                menu_pos--;
        }
        else if (key == '2')  // Down
        {
            menu_pos = (menu_pos + 1) % 4;
        }
        else if (key == '5')  // Enter
        {
            if (menu_pos == 0)  // Set Day
            {

                cmdLCD(CLEAR_LCD);
                strLCD("Set Date:");
                cmdLCD(GOTO_LINE2_POS0);
                cmdLCD(0x0C);

                while (1)
                {
                    u8 maxDays = GetMaxDays(month, year);
                    if (date > maxDays) date = maxDays;

                    cmdLCD(GOTO_LINE2_POS0);
										strLCD("    ");
										charLCD(2);               // Up arrow at position 0
										strLCD(" ");
                    if (date < 10) charLCD('0');
                    u32LCD(date);
										strLCD(" ");
										charLCD(3);

                    key = Read_KPM_Key();

                    if (key == '8')
                    {
                        date++;
                        if (date > maxDays) date = 1;
                    }
                    else if (key == '2')
                    {
                        date = (date == 1) ? maxDays : date - 1;
                    }
                    else if (key == '5')
                    {
                        break;
                    }

                    delayMS(200);
                }
            }
            else if (menu_pos == 1)  // Set Month
            {

                cmdLCD(CLEAR_LCD);
                strLCD("Set Month:");
                cmdLCD(GOTO_LINE2_POS0);
                cmdLCD(0x0C);

                while (1)
                {
                    cmdLCD(GOTO_LINE2_POS0);
										strLCD("    ");
										charLCD(2);               // Up arrow at position 0
										strLCD(" ");
                    if (month < 10) charLCD('0');
                    u32LCD(month);
										strLCD(" ");
										charLCD(3);

                    key = Read_KPM_Key();

                    if (key == '8')
                    {
                        month++;
                        if (month > 12) month = 1;
                    }
                    else if (key == '2')
                    {
                        month = (month == 1) ? 12 : month - 1;
                    }
                    else if (key == '5')
                    {
                        break;
                    }

                    delayMS(200);
                }
            }
            else if (menu_pos == 2)  // Set Year
            {

                cmdLCD(CLEAR_LCD);
                strLCD("Set Year:");
                cmdLCD(GOTO_LINE2_POS0);
                cmdLCD(0x0C);

                while (1)
                {
									
					cmdLCD(GOTO_LINE2_POS0);
					strLCD("    ");
					charLCD(2);               // Up arrow at position 0
					strLCD(" ");
                    u32LCD(year);
					strLCD(" ");
					charLCD(3);

                    key = Read_KPM_Key();

                    if (key == '8')
                        year++;
                    else if (key == '2')
                        year = (year > 2000) ? year - 1 : 2099;
                    else if (key == '5')
                        break;

                    delayMS(200);
                }
            }
            else if (menu_pos == 3)  // Exit
            {
                break;
            }

            // Validate date
            if (date > GetMaxDays(month, year)) date = GetMaxDays(month, year);

            // Update RTC date
            SetRTCDateInfo(date, month, year);

            cmdLCD(0x0C);
            cmdLCD(CLEAR_LCD);
            strLCD("UPDATED!");
            delayMS(500);
        }

        delayMS(200);
    }

    cmdLCD(CLEAR_LCD);
    strLCD("MENU...");
    delayMS(500);
    cmdLCD(CLEAR_LCD);
}

void edit_day(void)
{
    s8 *days[] = {"SUN", "MON", "TUE", "WED", "THU", "FRI", "SAT"};
    u32 key;
    s32 current_day;

    // Get current RTC day
    GetRTCDay(&current_day);

    cmdLCD(CLEAR_LCD);
    strLCD("Edit Day:");
    cmdLCD(GOTO_LINE2_POS0);
    cmdLCD(0x0C);  // cursor blinking ON

    while (1)
    {
        // Display current day on line 2
        cmdLCD(GOTO_LINE2_POS0);
        strLCD("    ");         // clear old text
        cmdLCD(GOTO_LINE2_POS0);
				strLCD("    ");
				charLCD(2);
				strLCD(" ");
        strLCD(days[current_day]);			// show current day
				strLCD(" ");
				charLCD(3);
				delayMS(1000);

        key = Read_KPM_Key();

        if (key == '8')  // UP
        {
            current_day = (current_day == 0) ? 6 : current_day - 1;
        }
        else if (key == '2')  // DOWN
        {
            current_day = (current_day + 1) % 7;
        }
        else if (key == '5')  // ENTER
        {
						
            SetRTCDay(current_day);  // Save selected day
            cmdLCD(0x0C);            // cursor OFF
            cmdLCD(CLEAR_LCD);
            strLCD("DAY UPDATED!");
            delayMS(500);
            cmdLCD(CLEAR_LCD);
            break;
        }

        delayMS(200);  // debounce
    }
}
u8 Read_KPM_Key(void)
{
    u8 key;
    while (1)
    {
        key = KeyScan();
        if (key != 0)
        {
            while (ColScan() == 0); // wait for key release
            return key;
        }
    }
}
void show_menu(void)
{
    u8 key;
    u8 pos = 0;
    const u8 total_items = 5;
    s8 *menu_items[] = {"Edit Time", "Set Alarm", "Edit Date", "Edit Day", "Exit Menu"};

    		cmdLCD(CLEAR_LCD);
		strLCD("8.UP      2.DOWN");
		cmdLCD(GOTO_LINE2_POS0);
		strLCD("    5.ENTER ");
		delayMS(1000);

    cmdLCD(CLEAR_LCD);
    while (1)
    {
        // Display menu items
        cmdLCD(GOTO_LINE1_POS0);
        strLCD(">");
        strLCD(menu_items[pos]);
        cmdLCD(GOTO_LINE2_POS0);
        if (pos + 1 < total_items)
            strLCD(menu_items[pos + 1]);
        else
            strLCD(menu_items[0]); 

        key = KeyScan();
		
        if (key == '2') // DOWN
        {
            pos = (pos + 1) % total_items;
            cmdLCD(CLEAR_LCD);
        }
        else if (key == '8') // UP
        {
            pos = (pos == 0) ? total_items - 1 : pos - 1;
            cmdLCD(CLEAR_LCD);
        }
        else if (key == '5') // ENTER
        {
            cmdLCD(CLEAR_LCD);
            switch (pos)
            {
                case 0: edit_time(); break;
                case 1: set_alarm(); break;
                case 2: edit_date(); break;
                case 3: edit_day(); break;
							  case 4:
									cmdLCD(CLEAR_LCD);
									strLCD("Returning Home....");
									delayMS(500);
									cmdLCD(CLEAR_LCD);
									return;
            }
            return;
        }
        delayMS(200);
    }
}
