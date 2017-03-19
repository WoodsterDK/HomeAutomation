/*
File:	main.h
Author: Simon Michelsen   -    simon6400()gmail.com
Date:	2017
*/


#ifndef MAIN_H_
#define MAIN_H_

/* Defines *******************************************************************/




// PB5 / D13 / LED
#define DDB5_LED_3_PORT_MASK			(1<<DDB5)    // PORTB
#define LED_3_OFF()				(PORTB &= ~DDB5_LED_3_PORT_MASK)
#define LED_3_ON()				(PORTB |= DDB5_LED_3_PORT_MASK)

// Port PB0 / D8 / IHC Output Module
#define DDB0_IHC_OUT_PORT_MASK			(1<<DDB0)    // PORTB
#define IHC_OUT_CLR()				(PORTB &= ~DDB0_IHC_OUT_PORT_MASK)
#define IHC_OUT_SET()				(PORTB |= DDB0_IHC_OUT_PORT_MASK)

// Port PB1 / D9 / Debug out
#define DDB1_DEBUG_PORT_MASK			(1<<DDB1)    // PORTB
#define DEBUG_OUT_CLR()				(PORTB &= ~DDB1_DEBUG_PORT_MASK)
#define DEBUG_OUT_SET()				(PORTB |= DDB1_DEBUG_PORT_MASK)

// Port PD7 / D7 / IHC Input Module
#define DDD7_IHC_IN_PORT_MASK			(1<<DDD7)    // PORTD
#define IHC_IN()				(PIND & DDD7_IHC_IN_PORT_MASK)

// 2 x 8 outputs (Light)
#define REG_001_OUT1_1_OUTDOOR		 1
#define REG_002_OUT1_2_OUTDOOR		 2
#define REG_003_OUT1_3_OUTDOOR		 3
#define REG_004_OUT1_4_OUTDOOR		 4
#define REG_005_OUT1_5_OUTDOOR		 5
#define REG_006_OUT1_6_OUTDOOR		 6
#define REG_007_OUT1_7_OUTDOOR		 7
#define REG_008_OUT1_8_SKUR			 8
#define REG_009_OUT2_1				 9
#define REG_010_OUT2_2				 10
#define REG_011_OUT2_3				 11
#define REG_012_OUT2_4				 12
#define REG_013_OUT2_5				 13
#define REG_014_OUT2_6				 14
#define REG_015_OUT2_7				 15
#define REG_016_OUT2_8				 16

// 16 inputs (PIR og buttons)
#define REG_021_IN_01_PIR_OUTDOOR	21
#define REG_022_IN_02_PIR_OUTDOOR	22
#define REG_023_IN_03_PIR_OUTDOOR	23
#define REG_024_IN_04_PIR_OUTDOOR	24
#define REG_025_IN_05_PIR_OUTDOOR	25
#define REG_026_IN_06_PIR_OUTDOOR	26
#define REG_027_IN_07_UNUSED		27
#define REG_028_IN_08_PIR_SKUR		28
#define REG_029_IN_11_UNUSED		29
#define REG_030_IN_12_UNUSED		30
#define REG_031_IN_13_UNUSED		31
#define REG_032_IN_14_UNUSED		32
#define REG_033_IN_15_B_OUT_TGL		33
#define REG_034_IN_16_B_OUT_AUTO	34
#define REG_035_IN_17_B_SKUR_TGL	35
#define REG_036_IN_18_B_SKUR_AUTO	36

// 8 motion sensors timeouts (e.g. 10 sec og 60 sec)
#define REG_051_SEC2NO_MOTION_01	51
#define REG_052_SEC2NO_MOTION_02	52
#define REG_053_SEC2NO_MOTION_03	53
#define REG_054_SEC2NO_MOTION_04	54
#define REG_055_SEC2NO_MOTION_05	55
#define REG_056_SEC2NO_MOTION_06	56
#define REG_057_SEC2NO_MOTION_07	57
#define REG_058_SEC2NO_MOTION_08_SKUR	58

// Light states (ON, OFF, AUTO)
#define REG_061_LIGHT_STATE_01_OUT	61
#define REG_062_LIGHT_STATE_02_OUT	62
#define REG_063_LIGHT_STATE_03_OUT	63
#define REG_064_LIGHT_STATE_04_OUT	64
#define REG_065_LIGHT_STATE_05_OUT	65
#define REG_066_LIGHT_STATE_06_OUT	66
#define REG_067_LIGHT_STATE_07_OUT	67
#define REG_068_LIGHT_STATE_08_SKUR	68


#define LAST_REG_INDEX 76
#define NUM_OF_REGS (LAST_REG_INDEX+1)

/* Type Definitions **********************************************************/
/* Constants *****************************************************************/
/* Variables *****************************************************************/
extern uint8_t Register[];

/* Public Functions **********************************************************/


#endif /* MAIN_H_ */


/*

I1 - PIR_01
I2 - PIR_02
I3 - PIR_03
I4 - PIR_04
I5 - PIR_05
I6 - PIR_06
I7
I8 - PIR_08 skur
I09
I10
I11
I12
I13 - Kontakt Ude Toggle
I14 - Kontakt Ude Auto
I15 - Kontakt Skur Toggle
I16 - Kontakt Skur Auto

O1 - Lys_01
O2
O3
O4

O8 - Lys_08 (skur)


toggle out
auto out
8 PIR out+skur



*/