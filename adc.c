#include <LPC214x.h>
#include "types.h"
#include "adc_defines.h"
#include "pin_connect_block.h"
#include "defines.h"
#include "delay.h"

void Init_adc(void)
{
    CfgPortPinFunc(0, 28, FUN2);               
    AD0CR = (1 << PDN_BIT) | (CLKDIV << CLKDIV_BIT); 
}

void Read_LM35_Temperature(u32 chno, f32 *temperature)
{
    u32 adc_value = 0;
    f32 voltage = 0.0;

    AD0CR &= 0xFFFFFF00;            
    AD0CR |= (1 << chno) | (1 << ADC_START_BIT); 
    delayUS(3);

    while (((AD0GDR >> DONE_BIT) & 1) == 0);

    AD0CR &= ~(1 << ADC_START_BIT);      
    adc_value = (AD0GDR >> RESULT_BITS) & 1023;
    voltage = (adc_value * 3.3) / 1023.0;
    *temperature = voltage * 100.0; 
}

