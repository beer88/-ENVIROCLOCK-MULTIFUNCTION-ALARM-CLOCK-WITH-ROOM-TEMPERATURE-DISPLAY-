#include <LPC213x.h>
#include "eint.h"
#include "eint_defines.h"
#include "pin_connect_block.h"

extern volatile unsigned int menu_mode;

void Init_eint(void)
{
    CfgPortPinFunc(0, 1, FUN4);   // P0.1 ? EINT0 (Function 4)

    EXTMODE |= (1 << 1);
    EXTPOLAR = (1 << 1);

   // Clear interrupt flag
    EXTINT = (1 << 1);
    // Enable interrupt in VIC
    VICIntSelect = 0;	// IRQ mode
    VICVectAddr0 = (u32)eint0_isr;
    VICVectCntl0 = (1 << 5) | EINT0_VIC_CHNO;
    VICIntEnable |= (1 << EINT0_VIC_CHNO);
}

void eint0_isr(void) __irq
{
    menu_mode = 1;             // Set flag for main loop
    EXTINT = (1 << 1);         // Clear EINT1 flag
    VICVectAddr = 0;           // Acknowledge VIC
}
