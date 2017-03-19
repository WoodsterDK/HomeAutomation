/*
File:	ihc.h
Author: Simon Michelsen   -    simon6400()gmail.com
Date:	2017
*/


#ifndef IHC_H_
#define IHC_H_

/* Defines *******************************************************************/
#define PRE_HIGH 54
#define PRE_LOW 4
#define NUM_OF_TICKS (PRE_HIGH + PRE_LOW + 8*16)

#define INPUT_CLOSED 0
#define INPUT_OPEN 1

/* Type Definitions **********************************************************/
/* Constants *****************************************************************/
/* Variables *****************************************************************/
extern int OutputTicks[NUM_OF_TICKS];

/* Public Functions **********************************************************/
void UpdateOutputs(void);
void Init_IhcOutputTicks(uint16_t value);
void ServiceOutputModule(void);
void ServiceInputModule(void);


#endif /* IHC_H_ */