/*
 * AVR_ADC.cpp
 *
 *  Created on: Oct 9, 2013
 *      Author: lhotse
 */



#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/sleep.h>



#include <AVR_USART_lib.h>



ISR(TIMER1_COMPA_vect)
{
	// Toggle the LED
	PORTB ^= (1 << PORTB5); // Toggle the LED

	// Start ADC
	//while ( ADCSRA & (1 << ADSC) ) ;

	USART_Transmit( 'H' ) ;
}



int main( void )
{
	// Set Port B direction Register
	DDRB 	|= (1<<DDB5) ; // For LED on Pin PORTB5

	// Configure the Timer/Counter1 Control Register A
	TCCR1A	= 0 ;
	// Configure the Timer/Counter1 Control Register B
	TCCR1B	= (1 << WGM12) ; // Configure timer 1 for CTC mode

	TIMSK1	= (1 << OCIE1A); // Enable CTC interrupt

	OCR1A	= 15624 ; // Set CTC compare value to 1Hz at 1MHz AVR clock, with a prescaler of 64

	sei(); //  Enable global interrupts

	USART_Init( MYUBRR ) ;

	// Configure ADC
	ADMUX |= (1 << REFS0) | (1 << MUX3) ; // Select ADC8
	// Enable ADC, Enable Interrupts, Select Prescaler to CLK/128
	ADCSRA |= (1 << ADEN) | (1 << ADPS2) | (1 << ADPS1) | (1 << ADPS0) ;


	TCCR1B |= (1 << CS12) ; // Start timer at Fcpu/64

	while( 1 )
	{
		//sleep_mode();
		//PORTB ^= (1 << PORTB5); // Toggle the LED

		//_delay_ms( 250.0 ) ;
	}

	return( 0 ) ;
}


