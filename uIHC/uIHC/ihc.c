/*
File:	ihc.c
Author: Simon Michelsen   -    simon6400()gmail.com
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

/* Type Definitions **********************************************************/
/* Constants *****************************************************************/
/* Variables *****************************************************************/
int OutputTicks[NUM_OF_TICKS];

/* Prototypes ****************************************************************/
/* Implementation ************************************************************/

// Ticks:
// 111111111111111111111111111111111111111111111111111111				      Preamble
// 0000
// 11xx0000 11xx0000 11xx0000 11xx0000 11xx0000 11xx0000 11xx0000 11xx0000    [Unused]
// 11xx0000 11xx0000 11xx0000 11xx0000 11xx0000 11xx0000 11xx0000 11xx0000    [Output 8..1]

void UpdateOutputs(void)
{
	// For Output module 1
	for(int i = 0; i < 8; i++)
	{
		OutputTicks[PRE_HIGH + PRE_LOW  + i*8 + 2] = 1 - Register[REG_001_OUT1_1_OUTDOOR + i];
		OutputTicks[PRE_HIGH + PRE_LOW  + i*8 + 3] = 1 - Register[REG_001_OUT1_1_OUTDOOR + i];
	}
	
	// For Output module 2
	// TBD
}

void Init_IhcOutputTicks(uint16_t value)
{
	int i;
	
	// HIGH for 27 x 164 us = 4,4 ms
	for(i = 0 ; i < PRE_HIGH; i++)
	{
		OutputTicks[i] = 1;
	}
	
	// LOW for 2 x 164 us = 328 us
	for(i = 0 ; i < PRE_LOW; i++)
	{
		OutputTicks[PRE_HIGH + i] = 0;
	}

	for(i = 0; i < 16; i++)
	{
		OutputTicks[PRE_HIGH + PRE_LOW + 8*i + 0] = 1;
		OutputTicks[PRE_HIGH + PRE_LOW + 8*i + 1] = 1;
		OutputTicks[PRE_HIGH + PRE_LOW + 8*i + 2] = (value >> i) & 0x01;
		OutputTicks[PRE_HIGH + PRE_LOW + 8*i + 3] = (value >> i) & 0x01;
		OutputTicks[PRE_HIGH + PRE_LOW + 8*i + 4] = 0;
		OutputTicks[PRE_HIGH + PRE_LOW + 8*i + 5] = 0;
		OutputTicks[PRE_HIGH + PRE_LOW + 8*i + 6] = 0;
		OutputTicks[PRE_HIGH + PRE_LOW + 8*i + 7] = 0;
	}
}

// about 2 us
void ServiceOutputModule(void)
{
	static int i = 0;
DEBUG_OUT_CLR();
	if(OutputTicks[i])
	{
		IHC_OUT_SET();
	}
	else
	{
		IHC_OUT_CLR();
	}

	i++;
	if(i > (NUM_OF_TICKS))
	{
		i = 0;
	}
DEBUG_OUT_SET();
}


void ServiceInputModule(void)
{
	static int input_index = 0;
	static int high_cnt = 0;
	
	if(IHC_IN())
	{
		high_cnt++;
	}
	else
	{
		// high_cnt is the number of hight samples before this low sample
		
		if(high_cnt == 0)		// Vi har allerede behandlet denne lave værdi én gang
		{
			
		}
		else if(high_cnt < 3)	// har lige set et 0-tal
		{
			Register[REG_021_IN_01_PIR_OUTDOOR + input_index] = 0;
			input_index++;
		}
		else if(high_cnt < 20)	// har lige set et 1-tal
		{
			Register[REG_021_IN_01_PIR_OUTDOOR + input_index] = 1;
			input_index++;
		}
		else					// er lige kommet forbi pre-amble
		{
			input_index = 0;
		}
		
		high_cnt = 0;
	}
}