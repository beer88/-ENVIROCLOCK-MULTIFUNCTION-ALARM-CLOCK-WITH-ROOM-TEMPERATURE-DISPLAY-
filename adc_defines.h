#define FOSC          12000000
#define CCLK          (FOSC*5)
#define PCLK          (CCLK/4)
#define ADCCLK        3000000
#define CLKDIV        ((PCLK/ADCCLK)-1)

//ADCR SFR DEFINES
#define CLKDIV_BIT   8
#define PDN_BIT       21
#define ADC_START_BIT 24

//ADDR SFR DEFINES
#define RESULT_BITS   6
#define DONE_BIT      31

