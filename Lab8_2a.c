/*
 *Lab 8, Section 2a
 *Name: Justin Frisch
 *Class #: 11476
 *PI Name: Christopher Taleno
 * Purpose: Using the DAC 
 */  

//Includes
#include <avr/io.h>
#include <avr/interrupt.h>
extern void clock_init(void);
//Prototypes
void dac_init(void);
void tcc0_init(void);

//Global variables
uint8_t ii = 0;
uint16_t arr[] = {2048,2098,2148,2198,2248,2298,2348,2398,
	2447,2496,2545,2594,2642,2690,2737,2784,
	2831,2877,2923,2968,3013,3057,3100,3143,
	3185,3226,3267,3307,3346,3385,3423,3459,
	3495,3530,3565,3598,3630,3662,3692,3722,
	3750,3777,3804,3829,3853,3876,3898,3919,
	3939,3958,3975,3992,4007,4021,4034,4045,
	4056,4065,4073,4080,4085,4089,4093,4094,
	4095,4094,4093,4089,4085,4080,4073,4065,
	4056,4045,4034,4021,4007,3992,3975,3958,
	3939,3919,3898,3876,3853,3829,3804,3777,
	3750,3722,3692,3662,3630,3598,3565,3530,
	3495,3459,3423,3385,3346,3307,3267,3226,
	3185,3143,3100,3057,3013,2968,2923,2877,
	2831,2784,2737,2690,2642,2594,2545,2496,
	2447,2398,2348,2298,2248,2198,2148,2098,
	2048,1997,1947,1897,1847,1797,1747,1697,
	1648,1599,1550,1501,1453,1405,1358,1311,
	1264,1218,1172,1127,1082,1038,995,952,
	910,869,828,788,749,710,672,636,
	600,565,530,497,465,433,403,373,
	345,318,291,266,242,219,197,176,
	156,137,120,103,88,74,61,50,
	39,30,22,15,10,6,2,1,
	0,1,2,6,10,15,22,30,
	39,50,61,74,88,103,120,137,
	156,176,197,219,242,266,291,318,
	345,373,403,433,465,497,530,565,
	600,636,672,710,749,788,828,869,
	910,952,995,1038,1082,1127,1172,1218,
	1264,1311,1358,1405,1453,1501,1550,1599,
	1648,1697,1747,1797,1847,1897,1947,1997};


//Main program
int main(void){
	clock_init();
	dac_init();
	tcc0_init();
	while (1){
		
	}
	
}

/* *************************************************************
* Name:     dac_init
* Purpose:  Initialize the DAC
* Inputs:
* Output:
**************************************************************/
void dac_init(void){
	//Set Channel operation on Channel 0
	DACA_CTRLB = DAC_CHSEL_SINGLE_gc;
	//Reference voltage
	DACA_CTRLC = DAC_REFSEL_AREFB_gc;
	//Enable channel 0 on DAC (Pin2)
	DACA_CTRLA = DAC_CH0EN_bm | DAC_ENABLE_bm;
}
/* *************************************************************
* Name:     tcc0_init
* Purpose:  Initialize the TCC0 for 880Hz
* Inputs:
* Output:
**************************************************************/
void tcc0_init(void){
	//32,000,000/(2*880*256)=71.022727
	//72 gives 881Hz
	//uint16_t PER = (72);
	//980 Hz at 64, actual is 990Hz..10Hz error
	//uint16_t PER = (64);
	//1080 Hz theory, 1090 hz actual
	//uint16_t PER = (58);
	//1180 Hz theory, 1191 Hz actual
	//uint16_t PER = (53);
	//1280 Hz theory, 1191 Hz actual
	uint16_t PER = (72);
	TCC0.PER = PER;
	
	//Configure Timer clock interrupts
	TCC0_INTCTRLA = TC_OVFINTLVL_LO_gc;
	//PMIC Controller
	PMIC_CTRL = PMIC_LOLVLEN_bm;
	//Enable global interrupts
	sei();
	
	TCC0_CTRLA = TC_CLKSEL_DIV2_gc;
}

/* *************************************************************
* Name:     ISR TCC0_OVF_vect
* Purpose:  Runs the sine wave array in Interrupt
* Inputs:
* Output:
**************************************************************/
ISR(TCC0_OVF_vect){

	DACA_CH0DATA = arr[ii++];
	
}