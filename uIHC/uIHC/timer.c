/*
File:	main.c
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
uint8_t Timeout_ToNoMotion_sec = 0;

/* Prototypes ****************************************************************/
void On10MS(void);
void OnSecond(void);

/* Implementation ************************************************************/

// hver 84 us, igang 4 us
ISR(TIMER0_COMPA_vect)        // interrupt service routine that wraps a user defined function supplied by attachInterrupt
{
	//static int toggle = 0;
//	DEBUG_OUT_CLR();
	/*if(toggle)
	{
		LED_3_ON();
	}
	else
	{
		LED_3_OFF();
	}
	
	toggle = 1 - toggle;*/
	
	ServiceOutputModule();
	ServiceInputModule();

// Service Timers
	static uint8_t timer_10ms = 0;
	static uint8_t timer_sec = 0;
	timer_10ms++;
	if( timer_10ms > 119)
	{

		timer_sec++;
		if(timer_sec > 100)
		{
			OnSecond();
			timer_sec = 0;
		}
		On10MS();
		timer_10ms = 0;
	}
	
//	DEBUG_OUT_SET();
}

void On10MS(void)
{



}

void OnSecond(void)
{
	for(char i = 0; i < 8; i++)
	if(Register[REG_051_SEC2NO_MOTION_01 + i] > 0)
	{
		Register[REG_051_SEC2NO_MOTION_01 + i]--;
	}
}



void Init_Timer(void)
{
	// initialize timer0 - For generating output pulses
	cli();		// disable all interrupts
	TCCR0A = 0;
	TCCR0B = 0;

	OCR0A = 0x14; // every 84 us

	TCCR0A |= (1 << WGM01);    // CTC-mode,
	TCCR0B |= /*(1 << FOC0A) | */(1 << CS00) | (1 << CS01) ;    // 64 prescaler,  Force output compare B

	TIMSK0 |= (1 << OCIE0A);              // Output compare interrupt enable

	sei();			// enable all interrupts
}


void OnTimer_10ms()
{


}


/*

toggle lys med 1 sec
holde lys i 30 sec efter signal
detektere tastetryk

*/