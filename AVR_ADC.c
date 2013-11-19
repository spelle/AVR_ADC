/*
 * AVR_ADC.cpp
 *
 *  Created on: Oct 9, 2013
 *      Author: lhotse
 */



#include "AVR_ADC.h"



ISR(TIMER1_COMPA_vect)
{
	// Toggle the LED
	PORTB ^= (1 << PORTB5); // Toggle the LED

	// Read the ADC value
	uint16_t uiADCvalue = ADC_Read() ;

	uiADCvalue <<= 6 ;
	uiADCvalue /= 3 ;
	uiADCvalue *= 5 ;
	uiADCvalue /= 341 ;

	PDEBUG( "Read ADC value : %d, Decimal part : %d\n", uiADCvalue, uiADCvalue&0x003F ) ;

	// Restart a conversion
	ADC_Start() ;
}



int main( void )
{
	// Set Port B direction Register
	DDRB	|= (1<<DDB5) ; // For LED on Pin PORTB5

	// Configure the Timer/Counter1 Control Register A
	TCCR1A	= 0 ;
	// Configure the Timer/Counter1 Control Register B
	TCCR1B	= (1 << WGM12) ; // Configure timer 1 for CTC mode

	TIMSK1	= (1 << OCIE1A); // Enable CTC interrupt

	OCR1A	= 62499 ; // Set CTC compare value to 1Hz at 1MHz AVR clock, with a prescaler of 64

	sei(); //  Enable global interrupts

	PDEBUG_INIT( 9600 ) ;

	TCCR1B |= (1 << CS12) ; // Start timer at Fcpu/64

	// Select the AVcc voltage reference
	ADC_SelectVoltageReference( AVCC ) ;
	//ADMUX |= (1<<REFS0) | (1<<REFS0) ;
	// Select ADC0
	ADC_SelectChannel( 0 ) ;
	//ADMUX |= (1<<MUX3) ;

	// Enable the ADC, set prescaler to F_CPU/128
	ADC_SelectPrescaler( FCPU_128 ) ;
	//ADCSRA |= (1 << ADPS2) | (1 << ADPS1) | (1 << ADPS0) ;

	ADC_Enable() ;
	//ADCSRA |= (1 << ADEN)
	//PRR &= ~(1 << PRADC) ;

	// Start a Single Conversion
	ADC_Start() ;
	//ADCSRA |= (1 << ADSC) ;

	while( 1 )
	{
		sleep_mode();
	}

	return( 0 ) ;
}


