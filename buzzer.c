#include <LPC214x.h>
#include "buzzer_defines.h"

void Init_Buzzer(void)
{
    IODIR1 |= BUZZER;       
}

void Buzzer_ON(void)
{
    IOSET1 |= BUZZER;  
}

void Buzzer_OFF(void)
{
    IOCLR1 = BUZZER;   
}

