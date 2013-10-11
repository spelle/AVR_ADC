/*
 * AVR_ADC.cpp
 *
 *  Created on: Oct 9, 2013
 *      Author: lhotse
 */



#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/sleep.h>



#define FOSC F_CPU // Clock Speed
#define BAUD 9600
#define MYUBRR FOSC/16/BAUD-1



void USART_Transmit( unsigned char data )
{
	/* Wait for empty transmit buffer */
	while ( !( UCSR0A & (1<<UDRE0)) )
	;
	/* Put data into buffer, sends the data */
	UDR0 = data;
}



ISR(TIMER1_COMPA_vect)
{
	// Toggle the LED
	PORTB ^= (1 << PORTB5); // Toggle the LED

	// Start ADC
	//while ( ADCSRA & (1 << ADSC) ) ;

	USART_Transmit( 'H' ) ;
}



void USART_Init( uint16_t ui16UBRR )
{
	/* Set baud rate */
	UBRR0H = (uint8_t) (ui16UBRR>>8) ;
	UBRR0L = (uint8_t) ui16UBRR ;
	/* Enable receiver and transmitter */
	UCSR0B = (1<<RXEN0) | (1<<TXEN0) ;
	/* Set frame format: 8data, 2stop bit */
	UCSR0C = (1<<USBS0) | (3<<UCSZ00) ;
}






int main( void )
{
	// Set Port B direction Register
	DDRB |= (1<<DDB5) ; // For LED on Pin PORTB5

	// Configure the Timer/Counter1 Control Register A
	TCCR1A = 0 ;
	// Configure the Timer/Counter1 Control Register B
	TCCR1B = (1 << WGM12) ; // Configure timer 1 for CTC mode

	TIMSK1 = (1 << OCIE1A); // Enable CTC interrupt

	OCR1A   = 15624 ; // Set CTC compare value to 1Hz at 1MHz AVR clock, with a prescaler of 64

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


