/*
 *Lab 8, Section 5
 *Name: Justin Frisch
 *Class #: 11476
 *PI Name: Christopher Taleno
 * Purpose: To create an electric keyboard with our computer keys
 */ 
//Includes
#include <avr/io.h>
#include <avr/interrupt.h>
extern void clock_init(void);

//Prototypes
void dac_init(void);
void tcc0_init(void);
void tcc1_init(void);
void usartd0_init(void);

//Global Variables
uint16_t PER = 0;
uint8_t ii = 0;
uint8_t s_flag = 0;
uint8_t e_flag = 0;
uint8_t four_flag = 0;
uint8_t r_flag = 0;
uint8_t five_flag = 0;
uint8_t t_flag = 0;
uint8_t y_flag = 0;
uint8_t seven_flag = 0;
uint8_t u_flag = 0;
uint8_t eight_flag = 0;
uint8_t i_flag = 0;
uint8_t nine_flag = 0;
uint8_t o_flag = 0;
uint16_t sine[] = {2048,2098,2148,2198,2248,2298,2348,2398,
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

uint16_t tri[] = {0x20,0x40,0x60,0x80,0xa0,0xc0,0xe0,0x100,
0x120,0x140,0x160,0x180,0x1a0,0x1c0,0x1e0,0x200,
0x220,0x240,0x260,0x280,0x2a0,0x2c0,0x2e0,0x300,
0x320,0x340,0x360,0x380,0x3a0,0x3c0,0x3e0,0x400,
0x420,0x440,0x460,0x480,0x4a0,0x4c0,0x4e0,0x500,
0x520,0x540,0x560,0x580,0x5a0,0x5c0,0x5e0,0x600,
0x620,0x640,0x660,0x680,0x6a0,0x6c0,0x6e0,0x700,
0x720,0x740,0x760,0x780,0x7a0,0x7c0,0x7e0,0x800,
0x81f,0x83f,0x85f,0x87f,0x89f,0x8bf,0x8df,0x8ff,
0x91f,0x93f,0x95f,0x97f,0x99f,0x9bf,0x9df,0x9ff,
0xa1f,0xa3f,0xa5f,0xa7f,0xa9f,0xabf,0xadf,0xaff,
0xb1f,0xb3f,0xb5f,0xb7f,0xb9f,0xbbf,0xbdf,0xbff,
0xc1f,0xc3f,0xc5f,0xc7f,0xc9f,0xcbf,0xcdf,0xcff,
0xd1f,0xd3f,0xd5f,0xd7f,0xd9f,0xdbf,0xddf,0xdff,
0xe1f,0xe3f,0xe5f,0xe7f,0xe9f,0xebf,0xedf,0xeff,
0xf1f,0xf3f,0xf5f,0xf7f,0xf9f,0xfbf,0xfdf,0xfff,
0xfdf,0xfbf,0xf9f,0xf7f,0xf5f,0xf3f,0xf1f,0xeff,
0xedf,0xebf,0xe9f,0xe7f,0xe5f,0xe3f,0xe1f,0xdff,
0xddf,0xdbf,0xd9f,0xd7f,0xd5f,0xd3f,0xd1f,0xcff,
0xcdf,0xcbf,0xc9f,0xc7f,0xc5f,0xc3f,0xc1f,0xbff,
0xbdf,0xbbf,0xb9f,0xb7f,0xb5f,0xb3f,0xb1f,0xaff,
0xadf,0xabf,0xa9f,0xa7f,0xa5f,0xa3f,0xa1f,0x9ff,
0x9df,0x9bf,0x99f,0x97f,0x95f,0x93f,0x91f,0x8ff,
0x8df,0x8bf,0x89f,0x87f,0x85f,0x83f,0x81f,0x800,
0x7e0,0x7c0,0x7a0,0x780,0x760,0x740,0x720,0x700,
0x6e0,0x6c0,0x6a0,0x680,0x660,0x640,0x620,0x600,
0x5e0,0x5c0,0x5a0,0x580,0x560,0x540,0x520,0x500,
0x4e0,0x4c0,0x4a0,0x480,0x460,0x440,0x420,0x400,
0x3e0,0x3c0,0x3a0,0x380,0x360,0x340,0x320,0x300,
0x2e0,0x2c0,0x2a0,0x280,0x260,0x240,0x220,0x200,
0x1e0,0x1c0,0x1a0,0x180,0x160,0x140,0x120,0x100,
0xe0,0xc0,0xa0,0x80,0x60,0x40,0x20,0x0,};

//Main program
int main(void){
	//initializations
	clock_init();
	dac_init();
	usartd0_init();
	tcc1_init();
	
	PORTC_DIRSET = 0x80;
	PORTC_OUTSET = 0x80;
	
	while (1)
	{
	if (s_flag == 0){
	
		if (e_flag)
		{
			cli();
			e_flag = 0;
			PER = 60;
			tcc0_init();
			TCC1.INTFLAGS = TC1_OVFIF_bm;
			while(!(TCC1.INTFLAGS & TC1_OVFIF_bm)){
			while(!(TCC0.INTFLAGS & TC0_OVFIF_bm));
			TCC0.INTFLAGS = TC0_OVFIF_bm;
			while(!(DACA.STATUS & DAC_CH1DRE_bm));
			DACA_CH1DATA = sine[ii++];}
			sei();
		}
		else if (four_flag)
		{
			cli();
			four_flag = 0;
			PER = 57;
			tcc0_init();
			TCC1.INTFLAGS = TC1_OVFIF_bm;
			while(!(TCC1.INTFLAGS & TC1_OVFIF_bm)){
			while(!(TCC0.INTFLAGS & TC0_OVFIF_bm));
			TCC0.INTFLAGS = TC0_OVFIF_bm;
			while(!(DACA.STATUS & DAC_CH1DRE_bm));
			DACA_CH1DATA = sine[ii++];}
			sei();
		}
		else if (r_flag)
		{
			cli();
			r_flag = 0;
			PER = 54;
			tcc0_init();
			TCC1.INTFLAGS = TC1_OVFIF_bm;
			while(!(TCC1.INTFLAGS & TC1_OVFIF_bm)){
			while(!(TCC0.INTFLAGS & TC0_OVFIF_bm));
			TCC0.INTFLAGS = TC0_OVFIF_bm;
			while(!(DACA.STATUS & DAC_CH1DRE_bm));
			DACA_CH1DATA = sine[ii++];}
			sei();
		}
		else if (five_flag)
		{
			cli();
			five_flag = 0;
			PER = 51;
			tcc0_init();
			TCC1.INTFLAGS = TC1_OVFIF_bm;
			while(!(TCC1.INTFLAGS & TC1_OVFIF_bm)){
			while(!(TCC0.INTFLAGS & TC0_OVFIF_bm));
			TCC0.INTFLAGS = TC0_OVFIF_bm;
			while(!(DACA.STATUS & DAC_CH1DRE_bm));
			DACA_CH1DATA = sine[ii++];}
			sei();
		}
		else if (t_flag)
		{
			cli();
			t_flag = 0;
			PER = 48;
			tcc0_init();
			TCC1.INTFLAGS = TC1_OVFIF_bm;
			while(!(TCC1.INTFLAGS & TC1_OVFIF_bm)){
			while(!(TCC0.INTFLAGS & TC0_OVFIF_bm));
			TCC0.INTFLAGS = TC0_OVFIF_bm;
			while(!(DACA.STATUS & DAC_CH1DRE_bm));
			DACA_CH1DATA = sine[ii++];}
			sei();
		}
		else if (y_flag)
		{
			cli();
			y_flag = 0;
			PER = 45;
			tcc0_init();
			TCC1.INTFLAGS = TC1_OVFIF_bm;
			while(!(TCC1.INTFLAGS & TC1_OVFIF_bm)){
			while(!(TCC0.INTFLAGS & TC0_OVFIF_bm));
			TCC0.INTFLAGS = TC0_OVFIF_bm;
			while(!(DACA.STATUS & DAC_CH1DRE_bm));
			DACA_CH1DATA = sine[ii++];}
			sei();
		}
		else if (seven_flag)
		{
			cli();
			seven_flag = 0;
			PER = 43;
			tcc0_init();
			TCC1.INTFLAGS = TC1_OVFIF_bm;
			while(!(TCC1.INTFLAGS & TC1_OVFIF_bm)){
			while(!(TCC0.INTFLAGS & TC0_OVFIF_bm));
			TCC0.INTFLAGS = TC0_OVFIF_bm;
			while(!(DACA.STATUS & DAC_CH1DRE_bm));
			DACA_CH1DATA = sine[ii++];}
			sei();
		}
		else if (u_flag)
		{
			cli();
			u_flag = 0;
			PER = 40;
			tcc0_init();
			TCC1.INTFLAGS = TC1_OVFIF_bm;
			while(!(TCC1.INTFLAGS & TC1_OVFIF_bm)){
			while(!(TCC0.INTFLAGS & TC0_OVFIF_bm));
			TCC0.INTFLAGS = TC0_OVFIF_bm;
			while(!(DACA.STATUS & DAC_CH1DRE_bm));
			DACA_CH1DATA = sine[ii++];}
			sei();
		}
		else if (eight_flag)
		{
			cli();
			eight_flag = 0;
			PER = 38;
			tcc0_init();
			TCC1.INTFLAGS = TC1_OVFIF_bm;
			while(!(TCC1.INTFLAGS & TC1_OVFIF_bm)){
			while(!(TCC0.INTFLAGS & TC0_OVFIF_bm));
			TCC0.INTFLAGS = TC0_OVFIF_bm;
			while(!(DACA.STATUS & DAC_CH1DRE_bm));
			DACA_CH1DATA = sine[ii++];}
			sei();
		}
		else if (i_flag)
		{
			cli();
			i_flag = 0;
			PER = 36;
			tcc0_init();
			TCC1.INTFLAGS = TC1_OVFIF_bm;
			while(!(TCC1.INTFLAGS & TC1_OVFIF_bm)){
			while(!(TCC0.INTFLAGS & TC0_OVFIF_bm));
			TCC0.INTFLAGS = TC0_OVFIF_bm;
			while(!(DACA.STATUS & DAC_CH1DRE_bm));
			DACA_CH1DATA = sine[ii++];}
			sei();
		}
		else if (nine_flag)
		{
			cli();
			nine_flag = 0;
			PER = 34;
			tcc0_init();
			TCC1.INTFLAGS = TC1_OVFIF_bm;
			while(!(TCC1.INTFLAGS & TC1_OVFIF_bm)){
			while(!(TCC0.INTFLAGS & TC0_OVFIF_bm));
			TCC0.INTFLAGS = TC0_OVFIF_bm;
			while(!(DACA.STATUS & DAC_CH1DRE_bm));
			DACA_CH1DATA = sine[ii++];}
			sei();
		}
		else if (o_flag)
		{
			cli();
			o_flag = 0;
			PER = 29;
			tcc0_init();
			TCC1.INTFLAGS = TC1_OVFIF_bm;
			while(!(TCC1.INTFLAGS & TC1_OVFIF_bm)){
			while(!(TCC0.INTFLAGS & TC0_OVFIF_bm));
			TCC0.INTFLAGS = TC0_OVFIF_bm;
			while(!(DACA.STATUS & DAC_CH1DRE_bm));
			DACA_CH1DATA = sine[ii++];}
			sei();
		}
		}
	else if (s_flag == 1)
	{
		if (e_flag)
		{
			cli();
			e_flag = 0;
			PER = 60;
			tcc0_init();
			TCC1.INTFLAGS = TC1_OVFIF_bm;
			while(!(TCC1.INTFLAGS & TC1_OVFIF_bm)){
			while(!(TCC0.INTFLAGS & TC0_OVFIF_bm));
			TCC0.INTFLAGS = TC0_OVFIF_bm;
			while(!(DACA.STATUS & DAC_CH1DRE_bm));
			DACA_CH1DATA = tri[ii++];}
			sei();
		}
		else if (four_flag)
		{
			cli();
			four_flag = 0;
			PER = 57;
			tcc0_init();
			TCC1.INTFLAGS = TC1_OVFIF_bm;
			while(!(TCC1.INTFLAGS & TC1_OVFIF_bm)){
			while(!(TCC0.INTFLAGS & TC0_OVFIF_bm));
			TCC0.INTFLAGS = TC0_OVFIF_bm;
			while(!(DACA.STATUS & DAC_CH1DRE_bm));
			DACA_CH1DATA = tri[ii++];}
			sei();
		}
		else if (r_flag)
		{
			cli();
			r_flag = 0;
			PER = 54;
			tcc0_init();
			TCC1.INTFLAGS = TC1_OVFIF_bm;
			while(!(TCC1.INTFLAGS & TC1_OVFIF_bm)){
			while(!(TCC0.INTFLAGS & TC0_OVFIF_bm));
			TCC0.INTFLAGS = TC0_OVFIF_bm;
			while(!(DACA.STATUS & DAC_CH1DRE_bm));
			DACA_CH1DATA = tri[ii++];}
			sei();
		}
		else if (five_flag)
		{
			cli();
			five_flag = 0;
			PER = 51;
			tcc0_init();
			TCC1.INTFLAGS = TC1_OVFIF_bm;
			while(!(TCC1.INTFLAGS & TC1_OVFIF_bm)){
			while(!(TCC0.INTFLAGS & TC0_OVFIF_bm));
			TCC0.INTFLAGS = TC0_OVFIF_bm;
			while(!(DACA.STATUS & DAC_CH1DRE_bm));
			DACA_CH1DATA = tri[ii++];}
			sei();
		}
		else if (t_flag)
		{
			cli();
			t_flag = 0;
			PER = 48;
			tcc0_init();
			TCC1.INTFLAGS = TC1_OVFIF_bm;
			while(!(TCC1.INTFLAGS & TC1_OVFIF_bm)){
			while(!(TCC0.INTFLAGS & TC0_OVFIF_bm));
			TCC0.INTFLAGS = TC0_OVFIF_bm;
			while(!(DACA.STATUS & DAC_CH1DRE_bm));
			DACA_CH1DATA = tri[ii++];}
			sei();
		}
		else if (y_flag)
		{
			cli();
			y_flag = 0;
			PER = 45;
			tcc0_init();
			TCC1.INTFLAGS = TC1_OVFIF_bm;
			while(!(TCC1.INTFLAGS & TC1_OVFIF_bm)){
			while(!(TCC0.INTFLAGS & TC0_OVFIF_bm));
			TCC0.INTFLAGS = TC0_OVFIF_bm;
			while(!(DACA.STATUS & DAC_CH1DRE_bm));
			DACA_CH1DATA = tri[ii++];}
			sei();
		}
		else if (seven_flag)
		{
			cli();
			seven_flag = 0;
			PER = 42;
			tcc0_init();
			TCC1.INTFLAGS = TC1_OVFIF_bm;
			while(!(TCC1.INTFLAGS & TC1_OVFIF_bm)){
			while(!(TCC0.INTFLAGS & TC0_OVFIF_bm));
			TCC0.INTFLAGS = TC0_OVFIF_bm;
			while(!(DACA.STATUS & DAC_CH1DRE_bm));
			DACA_CH1DATA = tri[ii++];}
			sei();
		}
		else if (u_flag)
		{
			cli();
			u_flag = 0;
			PER = 40;
			tcc0_init();
			TCC1.INTFLAGS = TC1_OVFIF_bm;
			while(!(TCC1.INTFLAGS & TC1_OVFIF_bm)){
			while(!(TCC0.INTFLAGS & TC0_OVFIF_bm));
			TCC0.INTFLAGS = TC0_OVFIF_bm;
			while(!(DACA.STATUS & DAC_CH1DRE_bm));
			DACA_CH1DATA = tri[ii++];}
			sei();
		}
		else if (eight_flag)
		{
			cli();
			eight_flag = 0;
			PER = 38;
			tcc0_init();
			TCC1.INTFLAGS = TC1_OVFIF_bm;
			while(!(TCC1.INTFLAGS & TC1_OVFIF_bm)){
			while(!(TCC0.INTFLAGS & TC0_OVFIF_bm));
			TCC0.INTFLAGS = TC0_OVFIF_bm;
			while(!(DACA.STATUS & DAC_CH1DRE_bm));
			DACA_CH1DATA = tri[ii++];}
			sei();
		}
		else if (i_flag)
		{
			cli();
			i_flag = 0;
			PER = 36;
			tcc0_init();
			TCC1.INTFLAGS = TC1_OVFIF_bm;
			while(!(TCC1.INTFLAGS & TC1_OVFIF_bm)){
			while(!(TCC0.INTFLAGS & TC0_OVFIF_bm));
			TCC0.INTFLAGS = TC0_OVFIF_bm;
			while(!(DACA.STATUS & DAC_CH1DRE_bm));
			DACA_CH1DATA = tri[ii++];}
			sei();
		}
		else if (nine_flag)
		{
			cli();
			nine_flag = 0;
			PER = 34;
			tcc0_init();
			TCC1.INTFLAGS = TC1_OVFIF_bm;
			while(!(TCC1.INTFLAGS & TC1_OVFIF_bm)){
			while(!(TCC0.INTFLAGS & TC0_OVFIF_bm));
			TCC0.INTFLAGS = TC0_OVFIF_bm;
			while(!(DACA.STATUS & DAC_CH1DRE_bm));
			DACA_CH1DATA = tri[ii++];}
			sei();
		}
		else if (o_flag)
		{
			cli();
			o_flag = 0;
			PER = 32;
			tcc0_init();
			TCC1.INTFLAGS = TC1_OVFIF_bm;
			while(!(TCC1.INTFLAGS & TC1_OVFIF_bm)){
			while(!(TCC0.INTFLAGS & TC0_OVFIF_bm));
			TCC0.INTFLAGS = TC0_OVFIF_bm;
			while(!(DACA.STATUS & DAC_CH1DRE_bm));
			DACA_CH1DATA = tri[ii++];}
			sei();
		}
	}
  }
}
/* *************************************************************
* Name:     usartd0_init
* Purpose:  Initialize the USART and Interrupt for data recieved
* Inputs:
* Output:
**************************************************************/
void usartd0_init(void){
	/* Configure relevant TxD and RxD pins. */
	PORTD.OUTSET = PIN3_bm;
	PORTD.DIRSET = PIN3_bm;
	PORTD.DIRCLR = PIN2_bm;
	//using 125000 Baud
	int8_t BSCALE = 0;
	int8_t BSEL = 15;
	//Configure baud rate
	USARTD0.BAUDCTRLA = (uint8_t)BSEL;
	USARTD0.BAUDCTRLB = (uint8_t)((BSCALE << 4)|(BSEL >> 8));

	/* Configure remainder of serial protocol. */
	//Asynch, 8 data bits, odd parity, 1 stop bit.
	USARTD0.CTRLC =	(USART_CMODE_ASYNCHRONOUS_gc |
	USART_PMODE_ODD_gc  	 |
	USART_CHSIZE_8BIT_gc)       &
	~USART_SBMODE_bm;

	/* Enable receiver and/or transmitter systems. */
	USARTD0.CTRLB = USART_RXEN_bm | USART_TXEN_bm;

	/* Enable interrupt */
	USARTD0.CTRLA = USART_RXCINTLVL_LO_gc;
	//Enable PMIC CNTRL
	PMIC_CTRL = PMIC_LOLVLEN_bm;
	//global enable
	sei();
}
/* *************************************************************
* Name:     dac_init
* Purpose:  Initialize the DAC on Ch1
* Inputs:
* Output:
**************************************************************/
void dac_init(void){
	
	//Set Channel operation as Channel 1 and enable event trigger on CH1
	DACA_CTRLB = DAC_CHSEL_SINGLE1_gc;
	//Configure the event channel
	//DACA_EVCTRL = DAC_EVSEL1_bm;
	//Reference voltage
	DACA_CTRLC = DAC_REFSEL_AREFB_gc;
	//Enable channel 0 on DAC (Pin2)
	DACA_CTRLA = DAC_CH1EN_bm | DAC_ENABLE_bm;
}
/* *************************************************************
* Name:     tcc0_init
* Purpose:  Initialize the TCC1 to poll when to write to DACA
* Inputs:
* Output:
**************************************************************/
void tcc0_init(void){
	//Gives 1.57khz

	TCC0_PER = PER;
	TCC0_CTRLA = TC_CLKSEL_DIV2_gc;
}
/* *************************************************************
* Name:     tcc1_init
* Purpose:  Initialize TCC1 for minimum amount of time to play key
* Inputs:
* Output:
**************************************************************/
void tcc1_init(void){
	TCC1.PER = 20835;
	TCC1.CTRLA = TC_CLKSEL_DIV1024_gc;
	
}

/* *************************************************************
* Name:     USARTC0_RXC_vect 
* Purpose:  Used to handle the flags for which keys are pressed
* Inputs:
* Output:
**************************************************************/
ISR(USARTD0_RXC_vect){
	uint8_t RXC_data = USARTD0_DATA;
	//set all to 0 now to limit processing
	e_flag = 0;
	four_flag = 0;
	r_flag = 0;
	five_flag = 0;
	t_flag = 0;
	y_flag = 0;
	seven_flag = 0;
	u_flag = 0;
	eight_flag = 0;
	i_flag = 0;
	nine_flag = 0;
	o_flag = 0;
	//Toggle S if it is pressed
	if (RXC_data == 's')
	{
		if (s_flag == 0)
		{
			s_flag = 1;
		}
		else if (s_flag == 1)
		{
			s_flag = 0;
		}

	}
	else if (RXC_data == 'e')
	{
		e_flag = 1;
	}
	else if (RXC_data == '4')
	{
		four_flag = 1;
	}
	else if (RXC_data == 'r')
	{
		r_flag = 1;
	}
	else if (RXC_data == '5')
	{
		five_flag = 1;
	}
	else if (RXC_data == 't')
	{
		t_flag = 1;
	}
	else if (RXC_data == 'y')
	{
		y_flag = 1;
	}
	else if (RXC_data == '7')
	{
		seven_flag = 1;
	}
	else if (RXC_data == 'u')
	{
		u_flag = 1;
	}
	else if (RXC_data == '8')
	{
		eight_flag = 1;
	}
	else if (RXC_data == 'i')
	{
		i_flag = 1;
	}
	else if (RXC_data == '9')
	{
		nine_flag = 1;
	}
	else if (RXC_data == 'o')
	{
		o_flag = 1;
	}
}

