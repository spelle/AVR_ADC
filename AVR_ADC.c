/*
 * AVR_ADC.cpp
 *
 *  Created on: Oct 9, 2013
 *      Author: lhotse
 */



#include "AVR_ADC.h"



#define B 13349 // 208.9 in FIXED(6)
#define A 774 // 12.1 in FIXED(6)



uint16_t uiCurrentADCChannel = 0 ;



ISR(TIMER1_COMPA_vect)
{
	// Toggle the LED
	PORTB ^= (1 << PORTB5); // Toggle the LED

	// Read the ADC value
	uint16_t uiADCvalue = ADC_Read() ;

	if ( 0 == uiCurrentADCChannel )
	{
		// uiADCvalue = a * T + b (=) T = ( uiADCvalue - b ) / a
#ifdef DEBUG
		double fTemp = ((double)uiADCvalue - 208.99) / 12.1 - 1;
		PDEBUG( "CTN-Float\t%f\t", fTemp ) ;
#endif
		uiADCvalue -= 209 ;
		uiADCvalue <<= 6 ; // for better resolution
		uiADCvalue /= 121 ;
		uiADCvalue *= 10 ;
		uiADCvalue -= 1<<6 ;

		PDEBUG( "CTN\t%d.%2.2d\t", uiADCvalue>>6, ((uiADCvalue&0x003F)*100+32)/64 ) ;
		uiCurrentADCChannel = 1 ;
	}
	else
		if( 1 == uiCurrentADCChannel )
		{
#ifdef DEBUG
			double fTemp = uiADCvalue / 610.0 * 25.0 ;
			PDEBUG( "LM335-Float\t%f\t", fTemp ) ;
#endif
			PDEBUG( "RAW\t%d\t", uiADCvalue ) ;
			uint32_t ui32ADCvalue = uiADCvalue ;
			ui32ADCvalue <<= 6 ; // for better resolution
			ui32ADCvalue *= 5 ;
			ui32ADCvalue /= 1023 ;
			ui32ADCvalue *= 100 ;
			ui32ADCvalue -= 17482 ; // 273.15 << 6
			uiADCvalue = (uint16_t) ui32ADCvalue ;
			PDEBUG( "LM335\t%d.%2.2d\n", uiADCvalue>>6, ((uiADCvalue&0x003F)*100+32)/64 ) ;

			uiCurrentADCChannel = 0 ;
		}

	// Restart a conversion
	ADC_SelectChannel( uiCurrentADCChannel ) ;
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

	// Select ADC0
	ADC_SelectChannel( uiCurrentADCChannel ) ;

	// Enable the ADC, set prescaler to F_CPU/128
	ADC_SelectPrescaler( FCPU_128 ) ;

	ADC_Enable() ;

	// Start a Single Conversion
	ADC_Start() ;

	while( 1 )
	{
		sleep_mode();
	}

	return( 0 ) ;
}


