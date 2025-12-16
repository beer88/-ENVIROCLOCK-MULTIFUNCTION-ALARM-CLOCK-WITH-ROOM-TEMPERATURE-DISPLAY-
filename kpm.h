#include "types.h"
void Init_KPM(void);
u32  ColScan(void);
u32  RowCheck(void);
u32  ColCheck(void);
u32  KeyScan(void);

void Read_KPM(u32 *,u8 *);
u8 KeyScanNonBlocking(void);
u8 Read_KPM_Key(void);
u8 Read_KPM1(void);
u32 Read_KPM_2Digits(u8 lcd_pos);
