#include "types.h"
#include "delay.h"

void Init_LCD(void);
void cmdLCD(u8);
void writeLCD(u8);
void charLCD(u8);
void strLCD(s8 *);
void u32LCD(u32);
void s32LCD(s32);
void f32LCD(f32,u8);
void strLCD_num(u8 num);
void LCD_CreateCustomChars(void);
void create_degreeC_symbol(void);
void lcd_uparrow(void);
void lcd_downarrow(void);
void show_menu(void);
void edit_time(void);
void edit_date(void);
void edit_day(void);
u32 Read_2DigitValue(void);
void display_time(void);
void set_alarm(void);
