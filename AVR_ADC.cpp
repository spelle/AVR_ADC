/*
 * AVR_ADC.cpp
 *
 *  Created on: Oct 9, 2013
 *      Author: lhotse
 */




#include <avr/io.h>

#include <util/delay.h>



int main( void )
{

	DDRB |= (1 << PB5 ) ;

	// Main Loop
	for( ; ; )
	{
		PORTB ^= (1 << PB5 ) ;

		_delay_ms( 500.0 ) ;
	}

	return( 0 ) ;
}
