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

	double dA, dB, dB1, dB2, dY1, dY2, dX1, dX2 ;

	if( uiADCvalue < Temp_vs_ADC_Value_LookupTable[0].uiADC_Value )
	{	// Acquired ADC Value is lower than lower bound
		// Takes the two first points
		dX1 = Temp_vs_ADC_Value_LookupTable[0].uiTemp ;
		dY1 = Temp_vs_ADC_Value_LookupTable[0].uiADC_Value ;
		dX2 = Temp_vs_ADC_Value_LookupTable[1].uiTemp ;
		dY2 = Temp_vs_ADC_Value_LookupTable[1].uiADC_Value ;
	}
	else if( uiADCvalue >= Temp_vs_ADC_Value_LookupTable[sizeof(Temp_vs_ADC_Value_LookupTable)/sizeof(Temp_vs_ADC_Value_t)-1].uiADC_Value )
	{	// Acquired ADC Value is greater than upper bound
		// Takes the two last points
		dX1 = Temp_vs_ADC_Value_LookupTable[sizeof(Temp_vs_ADC_Value_LookupTable)/sizeof(Temp_vs_ADC_Value_t)-2].uiTemp ;
		dY1 = Temp_vs_ADC_Value_LookupTable[sizeof(Temp_vs_ADC_Value_LookupTable)/sizeof(Temp_vs_ADC_Value_t)-2].uiADC_Value ;
		dX2 = Temp_vs_ADC_Value_LookupTable[sizeof(Temp_vs_ADC_Value_LookupTable)/sizeof(Temp_vs_ADC_Value_t)-1].uiTemp ;
		dY2 = Temp_vs_ADC_Value_LookupTable[sizeof(Temp_vs_ADC_Value_LookupTable)/sizeof(Temp_vs_ADC_Value_t)-1].uiADC_Value ;
	}
	else
	{	// Find the interval where to perfom the computing of temperature
		uint16_t ui ;
		for(
			ui = 0 ;
			ui < sizeof(Temp_vs_ADC_Value_LookupTable)/sizeof(uint16_t) &&
			uiADCvalue >= Temp_vs_ADC_Value_LookupTable[ui].uiADC_Value ;
			ui++ ) ;

		// Computes the X1, Y1, X2, Y2 points
		dX1 = (ui*5.0-55.0) ;
		dY1 = Temp_vs_ADC_Value_LookupTable[ui].uiADC_Value ;
		dX2 = ((ui+1)*5.0-55.0) ;
		dY2 = Temp_vs_ADC_Value_LookupTable[ui+1].uiADC_Value ;
	}

	// Computes the slope and offset fir the Lookup-segment
	dA = ( dY2 - dY1)/( dX2 - dX1 ) ;
	dB2 = dY2 - dA*dX2 ;
	dB1 = dY1 - dA*dX1 ;
	dB  = (dB1+dB2)/2.0 ;

	// Computes the Temperature
	double dTemp = (uiADCvalue - dB) / dA ;

	//Removes 1.0 Celsius Degree Offset
	dTemp -= 1.0 ;

	PDEBUG( "CTN-Float\t%f\n", dTemp ) ;

	// Restart a conversion
	ADC_Start() ;
}


ISR(PCINT2_vect)
{
	if( 0 != (PIND & (1<<PIND6)) )
	{
		PORTD ^= (1<<PORTD5) ;
	}
}


int main( void )
{
	// Set Port B Data Direction Register
	DDRB	|= (1<<DDB5) ; // For LED on Pin PORTB5

	// Set Port D Data Direction Register
	DDRD	|= (1<<DDD5) ; // For LED on Pin PORTD5

	// Set All mechanics for Port D6
	PCMSK2	|= (1<<PCINT22) ;
	PCICR	|= (1<<PCIE2) ;
	PORTD	|= (1<<PORTD6) ; // Enable pullup resistor

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
	ADC_SelectVoltageReference( AREF ) ;

	// Select ADC0
	ADC_SelectChannel( 0 ) ;

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


