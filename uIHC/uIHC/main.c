/*
File:	main.c
Author: Simon Michelsen   -    simon6400()gmail.com
*/


/*
History:
2017-02-06:
splittet op i flere filer
gjort rx receive interrupt baseret


TODO:
Impl. secund og ms timere
impl taste aflæsning
impl. event hådtering
Omdøb projekt
Opstil formatering og kode pænt
// Nyt navn: IHC_IO_Interface, MicroIHC, uIHC

Sæt default auto mode

*/

#include <avr/interrupt.h>
#include <avr/io.h>
#include <string.h>       // memcpy
#include <stdio.h>
#include <avr/wdt.h>
#include <inttypes.h>

#include "main.h"
#include "timer.h"
#include "ihc.h"
#include "serial.h"

/* Defines *******************************************************************/
#define STATE_ON 1
#define STATE_OFF 0
#define STATE_AUTO 2
#define OUT_ON 1
#define OUT_OFF 0

#define TIME_LIGHT_OUTDOOR_AFTER_MOTION_SEC 120//60
#define TIME_LIGHT_SKUR_AFTER_MOTION_SEC 250//5//200


/* Type Definitions **********************************************************/
/* Constants *****************************************************************/
/* Variables *****************************************************************/
uint8_t Register[NUM_OF_REGS];

/* Prototypes ****************************************************************/
void HandleMessage(void);

/* Implementation ************************************************************/
void Init_IO(void)
{
	// DDRn 1=output, 0=input
	// PORTn Output: 1=high, 0=low, Input: 0=???, 1=???

	DDRB = DDB5_LED_3_PORT_MASK | DDB0_IHC_OUT_PORT_MASK | DDB1_DEBUG_PORT_MASK;	// Outputs

	DDRD &= ~DDD7_IHC_IN_PORT_MASK;		// Input
	PORTD |= DDD7_IHC_IN_PORT_MASK;		// Pull-up
	
//	PORTD &= ~DDD7_IHC_IN_PORT_MASK;		// no Pull-up
}


/******************************************************************************
  Name   : main
  Summary: Main program loop for non interrupt-based functionality
******************************************************************************/
int main (void)
{
	wdt_disable();
	Init_Timer();
	Init_USART();
	Init_IO();
	Init_IhcOutputTicks(0x00);
	
	SendText("IHC Output Module v1 - 2017-01-31 e\n\r");
	SendText("Reg [ 0- 8] Output module 1\n\rReg [10-18] Output module 2\n\rReg [20-36] Input Module\n\r");
	SendText("Set Register: s <reg> <val>, e.g. 's 3 1'\n\r");
	SendText("Get Register: g <reg>, e.g. 'g 4'\n\r");

	for (;;)
	{
		static uint8_t knap_last_out_toggle = 0;
		static uint8_t knap_last_out_auto = 0;
		static uint8_t knap_last_skur_toggle = 0;
		static uint8_t knap_last_skur_auto = 0;
		
		if(msg_rx_complete == 1)
		{
			HandleMessage();
			msg_rx_complete = 0;
		}

		// Pushed Button 'Ude Toggle'
		if((Register[REG_033_IN_15_B_OUT_TGL] == INPUT_CLOSED) && (knap_last_out_toggle == INPUT_OPEN))
		{
			if(Register[REG_001_OUT1_1_OUTDOOR] == STATE_ON)
			{
				for(char i = 0; i < 7; i++)
				{	Register[REG_061_LIGHT_STATE_01_OUT + i] = STATE_OFF;	}
				SendText("OUT OFF\n\r");
			}
			else
			{
				for(char i = 0; i < 7; i++)
				{	Register[REG_061_LIGHT_STATE_01_OUT + i] = STATE_ON;	}
				SendText("OUT ON\n\r");
			}
		}
		knap_last_out_toggle = Register[REG_033_IN_15_B_OUT_TGL];
		
		// Pushed Button  'Ude AUTO'
		if((Register[REG_034_IN_16_B_OUT_AUTO] == INPUT_CLOSED) && (knap_last_out_auto == INPUT_OPEN))
		{
			for(char i = 0; i < 7; i++)
			{Register[REG_061_LIGHT_STATE_01_OUT + i] = STATE_AUTO;}

			SendText("OUT AUTO\n\r");
		}
		knap_last_out_auto = Register[REG_034_IN_16_B_OUT_AUTO];
	
	
		// Pushed Button  'SKUR Toggle'
		if((Register[REG_035_IN_17_B_SKUR_TGL] == INPUT_CLOSED) && (knap_last_skur_toggle == INPUT_OPEN))
		{
			if(Register[REG_008_OUT1_8_SKUR] == STATE_ON)
			{
				Register[REG_068_LIGHT_STATE_08_SKUR] = STATE_OFF;

				//Register[REG_051_SEC2NO_MOTION_01] = 0;
				SendText("SKUR OFF\n\r");
			}
			else
			{
				Register[REG_068_LIGHT_STATE_08_SKUR] = STATE_ON;
				SendText("SKUR ON\n\r");
			}
		}
		knap_last_skur_toggle = Register[REG_035_IN_17_B_SKUR_TGL];
			
		// Pushed Button 'SKUR AUTO'
		if((Register[REG_036_IN_18_B_SKUR_AUTO] == INPUT_CLOSED) && (knap_last_skur_auto == INPUT_OPEN))
		{
			Register[REG_068_LIGHT_STATE_08_SKUR] = STATE_AUTO;
			SendText("SKUR AUTO\n\r");
		}
		knap_last_skur_auto = Register[REG_036_IN_18_B_SKUR_AUTO];	

		// Håndter PIR sensorer, og sæt timere ved bevægelse
		if(Register[REG_021_IN_01_PIR_OUTDOOR] == INPUT_CLOSED)		{ Register[REG_051_SEC2NO_MOTION_01] = TIME_LIGHT_OUTDOOR_AFTER_MOTION_SEC;		SendText("P1");		}
		if(Register[REG_022_IN_02_PIR_OUTDOOR] == INPUT_CLOSED)		{ Register[REG_052_SEC2NO_MOTION_02] = TIME_LIGHT_OUTDOOR_AFTER_MOTION_SEC;		SendText("P2");		}
		if(Register[REG_023_IN_03_PIR_OUTDOOR] == INPUT_CLOSED)		{ Register[REG_053_SEC2NO_MOTION_03] = TIME_LIGHT_OUTDOOR_AFTER_MOTION_SEC;		SendText("P3");		}
		if(Register[REG_024_IN_04_PIR_OUTDOOR] == INPUT_CLOSED)		{ Register[REG_054_SEC2NO_MOTION_04] = TIME_LIGHT_OUTDOOR_AFTER_MOTION_SEC;		SendText("P4");		}
		if(Register[REG_025_IN_05_PIR_OUTDOOR] == INPUT_CLOSED)		{ Register[REG_055_SEC2NO_MOTION_05] = TIME_LIGHT_OUTDOOR_AFTER_MOTION_SEC;		SendText("P5");		}
		if(Register[REG_026_IN_06_PIR_OUTDOOR] == INPUT_CLOSED)		{ Register[REG_056_SEC2NO_MOTION_06] = TIME_LIGHT_OUTDOOR_AFTER_MOTION_SEC;		SendText("P6");		}
		if(Register[REG_028_IN_08_PIR_SKUR] == INPUT_CLOSED)	{ Register[REG_058_SEC2NO_MOTION_08_SKUR] = TIME_LIGHT_SKUR_AFTER_MOTION_SEC;		SendText("P8");		}

		// Sæt lys ude
		Register[REG_001_OUT1_1_OUTDOOR]		= (Register[REG_061_LIGHT_STATE_01_OUT] == STATE_ON) ||  ((Register[REG_061_LIGHT_STATE_01_OUT] == STATE_AUTO) && (Register[REG_051_SEC2NO_MOTION_01] > 0));
		Register[REG_002_OUT1_2_OUTDOOR]		= (Register[REG_062_LIGHT_STATE_02_OUT] == STATE_ON) ||  ((Register[REG_062_LIGHT_STATE_02_OUT] == STATE_AUTO) && (Register[REG_052_SEC2NO_MOTION_02] > 0));
		Register[REG_003_OUT1_3_OUTDOOR]		= (Register[REG_063_LIGHT_STATE_03_OUT] == STATE_ON) ||  ((Register[REG_063_LIGHT_STATE_03_OUT] == STATE_AUTO) && (Register[REG_053_SEC2NO_MOTION_03] > 0));
		Register[REG_004_OUT1_4_OUTDOOR]		= (Register[REG_064_LIGHT_STATE_04_OUT] == STATE_ON) ||  ((Register[REG_064_LIGHT_STATE_04_OUT] == STATE_AUTO) && (Register[REG_054_SEC2NO_MOTION_04] > 0));
		Register[REG_005_OUT1_5_OUTDOOR]		= (Register[REG_065_LIGHT_STATE_05_OUT] == STATE_ON) ||  ((Register[REG_065_LIGHT_STATE_05_OUT] == STATE_AUTO) && (Register[REG_055_SEC2NO_MOTION_05] > 0));
		Register[REG_006_OUT1_6_OUTDOOR]		= (Register[REG_066_LIGHT_STATE_06_OUT] == STATE_ON) ||  ((Register[REG_066_LIGHT_STATE_06_OUT] == STATE_AUTO) && (Register[REG_056_SEC2NO_MOTION_06] > 0));
		Register[REG_007_OUT1_7_OUTDOOR]		= (Register[REG_067_LIGHT_STATE_07_OUT] == STATE_ON) ||  ((Register[REG_067_LIGHT_STATE_07_OUT] == STATE_AUTO) && (Register[REG_057_SEC2NO_MOTION_07] > 0));
		Register[REG_008_OUT1_8_SKUR]	= (Register[REG_068_LIGHT_STATE_08_SKUR] == STATE_ON) ||  ((Register[REG_068_LIGHT_STATE_08_SKUR] == STATE_AUTO) && (Register[REG_058_SEC2NO_MOTION_08_SKUR] > 0));
		// ...
		

		UpdateOutputs();	
	}
}

void HandleMessage(void)
{
	char cmd;
	int reg;
	int val;

	if(ser_rx_buf[0] == 's')	// [S]et
	{
		sscanf(ser_rx_buf, "%c %d %d", &cmd, &reg, &val);
		
		if(reg < 0 || reg > LAST_REG_INDEX)
		{
			SendText("Fejl: reg\n\r");
		}
		else
		{
			sprintf(ser_rx_buf, "%d=%d\n\r", reg, val);
			SendText(ser_rx_buf);
			Register[reg] = val;
			UpdateOutputs();
		}
	}
	else if(ser_rx_buf[0] == 'g')	// [G]et
	{
		sscanf(ser_rx_buf, "%c %d", &cmd, &reg);
		
		if(reg < 1 || reg > LAST_REG_INDEX)
		{
			SendText("Fejl: reg\n\r");
		}
		else
		{
			val = Register[reg];
			sprintf(ser_rx_buf, "%d=%d\n\r", reg, val);
			SendText(ser_rx_buf);
		}
	}
	else if(ser_rx_buf[0] == 'o')	// Print [O]utput ticks
	{
		SendText("Output Ticks:\n\r");
		for(int i = 0; i < 8; i++)
		{
			USART_Transmit_Byte(OutputTicks[i] + '0');
		}
		SendText("\n\r");
	}
	else if(ser_rx_buf[0] == 'i')	// Print [I]input ticks
	{
		SendText("Input Ticks:\n\r");
		for(int i = 0; i < 16; i++)
		{
			USART_Transmit_Byte(Register[REG_021_IN_01_PIR_OUTDOOR + i] + '0');
		}
		SendText("\n\r");
	}
	else
	{
		SendText("Fejl: cmd\n\r");
	}

}