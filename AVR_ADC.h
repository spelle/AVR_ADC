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



#undef PDEBUG_INIT
#undef PDEBUG



#ifdef DEBUG
	#define PDEBUG_INIT( BR ) USART_Init( BR )
	// no need of a terminal \n. put by default
	#define PDEBUG( msg ) printf( msg )
#else /* DEBUG */
	#define PDEBUG_INIT( BR ) /* not debugging: nothing */
	#define PDEBUG( msg ) /* not debugging: nothing */
#endif /* DEBUG */

#endif /* AVR_ADC_H_ */
