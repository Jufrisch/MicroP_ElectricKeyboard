/*
 *Lab 8, Section 1
 *Name: Justin Frisch
 *Class #: 11476
 *PI Name: Christopher Taleno
 * Purpose: Using the DAC to output a constant voltage of 1.5V
 */ 

//includes
#include <avr/io.h>
extern void clock_init(void);
//prototypes
void dac_init(void);
void dac_out(int16_t data_out);

//main program
int main(void)
{
	clock_init();
	dac_init();
	//I did (1.5 V x 4095)/2.5 (2.5 = ref voltage)
	//2457
	dac_out(2457);
	
    /* Replace with your application code */
    while (1) 
    {
		
    }
}

/* *************************************************************
* Name:     dac_init
* Purpose:  Initialize the DAC. CH0, AREFB
* Inputs:
* Output:
**************************************************************/
void dac_init(void){
	//Enable channel 0 on DAC (Pin2)
	DACA_CTRLA = DAC_CH0EN_bm | DAC_ENABLE_bm;
	//Set Channel operation on Channel 0
	DACA_CTRLB = DAC_CHSEL_SINGLE_gc;
	//Reference voltage
	DACA_CTRLC = DAC_REFSEL_AREFB_gc;
}

void dac_out(int16_t data_out){
	DACA_CH0DATA = data_out;
	
}


