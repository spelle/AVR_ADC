/*
 * AVR_ADC.h
 *
 *  Created on: 6 nov. 2013
 *      Author: A127590
 */

#ifndef AVR_ADC_H_
#define AVR_ADC_H_



#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/sleep.h>



#include <AVR_USART_lib.h>
#include <AVR_ADC_lib.h>



#undef PDEBUG_INIT
#undef PDEBUG

typedef struct
{
	uint16_t uiTemp ;
	uint16_t uiADC_Value ;
} Temp_vs_ADC_Value_t ;

static Temp_vs_ADC_Value_t Temp_vs_ADC_Value_LookupTable[] =
{
	{ -55,	   8},
	{ -50,	  12},
	{ -45,	  17},
	{ -40,	  24},
	{ -35,	  33},
	{ -30,	  45},
	{ -25,	  61},
	{ -20,	  82},
	{ -15,	 108},
	{ -10,	 140},
	{  -5,	 179},
	{   0,	 225},
	{   5,	 276},
	{  10,	 331},
	{  15,	 390},
	{  20,	 451},
	{  25,	 512},
	{  30,	 570},
	{  35,	 627},
	{  40,	 679},
	{  45,	 725},
	{  50,	 767},
	{  55,	 804},
	{  60,	 836},
	{  65,	 864},
	{  70,	 887},
	{  75,	 907},
	{  80,	 924},
	{  85,	 939},
	{  90,	 951},
	{  95,	 961},
	{ 100,	 970},
	{ 105,	 977},
	{ 110,	 983},
	{ 115,	 989},
	{ 120,	 993},
	{ 125,	 997},
	{ 130,	1000},
	{ 135,	1003},
	{ 140,	1006},
	{ 145,	1008},
	{ 150,	1010},
	{ 155,	1011},
} ;


#ifdef DEBUG
	#define PDEBUG_INIT( BR ) USART_Init( BR )
	// no need of a terminal \n. put by default
	#define PDEBUG( ... ) printf( __VA_ARGS__ )
#else /* DEBUG */
	#define PDEBUG_INIT( BR ) /* not debugging: nothing */
	#define PDEBUG( ... ) /* not debugging: nothing */
#endif /* DEBUG */

#endif /* AVR_ADC_H_ */
