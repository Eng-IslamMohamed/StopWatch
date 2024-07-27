#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>

//Global Variables
unsigned char SEC_UNITS=0;
unsigned char SEC_TENS=0;
unsigned char MIN_UNITS=0;
unsigned char MIN_TENS=0;
unsigned char HOUR_UNITS=0;
unsigned char HOUR_TENS=0;


//Function Prototypes

void TIMER1_INIT(void);
void INT0_INIT(void);
void INT1_INIT(void);
void INT2_INIT(void);
ISR(TIMER1_COMPA_vect);
ISR(INT0_vect);
ISR(INT1_vect);
ISR(INT2_vect);

int main(void){
	TIMER1_INIT(); //Initializing TIMER1
	INT0_INIT(); //Initializing Interrupt 0
	INT1_INIT(); //Initializing Interrupt 1
	INT2_INIT(); //Initializing Interrupt 2
	SREG |= (1<<7); //Set I-bit

	DDRC |= 0x0F; //Decoder is connected to the first 4 pins in PORTC
	PORTC &= 0xF0; //Initializing our 7-Segments to Display 0 at first run

	DDRA |= 0x3F; //First 6 pins are OUTPUT to enable/disable our 6 7-Segment Display
	PORTA |= 0x3F;

	DDRD &= ~(1<<2) & ~(1<<3); //Setting pin 2 in port d as an input pin
	PORTD |= (1<<2); //Activating Internal Pull-up Resistor

	DDRB &=~(1<<2); //Setting pin 2 in port b as an input pin
	PORTB |=(1<<2); //Activating Internal Pull-up Resistor

	while(1){

			PORTA = (PORTA & 0xC0) | ( 0x20 & 0x3F); //Enabling Units of Seconds 7-Segment
			PORTC = (PORTC & 0xF0) | (SEC_UNITS & 0x0F);
			_delay_ms(1);
			PORTA = (PORTA & 0xC0) | ( 0x10 & 0x3F); //Enabling Tens of Seconds 7-Segment
			PORTC = (PORTC & 0xF0) | (SEC_TENS & 0x0F);
			_delay_ms(1);
			PORTA = (PORTA & 0xC0) | ( 0x08 & 0x3F); //Enabling Units of Minutes 7-Segment
			PORTC = (PORTC & 0xF0) | (MIN_UNITS & 0x0F);
			_delay_ms(1);
			PORTA = (PORTA & 0xC0) | ( 0x04 & 0x3F); //Enabling Tens of Minutes 7-Segment
			PORTC = (PORTC & 0xF0) | (MIN_TENS & 0x0F);
			_delay_ms(1);
			PORTA = (PORTA & 0xC0) | ( 0x02 & 0x3F); //Enabling Units of Hours 7-Segment
			PORTC = (PORTC & 0xF0) | (HOUR_UNITS & 0x0F);
			_delay_ms(1);
			PORTA = (PORTA & 0xC0) | ( 0x01 & 0x3F); //Enabling Tens of Hours 7-Segment
			PORTC = (PORTC & 0xF0) | (HOUR_TENS & 0x0F);
			_delay_ms(1);

			if(SEC_UNITS==10){
					SEC_UNITS=0;
					SEC_TENS++;
					if(SEC_TENS==6){
						SEC_TENS=0;
						SEC_UNITS=0;
						MIN_UNITS++;
					}
			}
			if(MIN_UNITS==10){
					SEC_UNITS=0;
					SEC_TENS=0;
					MIN_UNITS=0;
					MIN_TENS++;
					if(MIN_TENS==6){
							SEC_UNITS=0;
							SEC_TENS=0;
							MIN_UNITS=0;
							MIN_TENS=0;
							HOUR_UNITS++;
						}
				}
			if(HOUR_UNITS==10){
					SEC_UNITS=0;
					SEC_TENS=0;
					MIN_UNITS=0;
					MIN_TENS=0;
					HOUR_UNITS=0;
					HOUR_TENS++;
					if(HOUR_TENS==6){
							SEC_UNITS=0;
							SEC_TENS=0;
							MIN_UNITS=0;
							MIN_TENS=0;
							HOUR_UNITS=0;
							HOUR_TENS=0;
						}
			}



	}

}


void TIMER1_INIT(void){

	TCCR1A |= (1<<FOC1A);
	TCCR1B |= (1<<WGM12) | (1<<CS12);
	TCNT1=0;
	OCR1A=3907;
	//elrakam elly bageebo mn taht le fo2 mn el equations=3906.25;
	TIMSK |= (1<<OCIE1A);
}

void INT0_INIT(void){
	GICR |= (1<<INT0);
	MCUCR |= (1<<ISC11) |(1<<ISC10);
}

void INT1_INIT(void){
	GICR |= (1<<INT1);
	MCUCR |= (1<<ISC01);
}

void INT2_INIT(void){
	GICR |= (1<<INT2);
}

ISR(INT0_vect){

	TCNT1=0;
	TCCR1B |= (1<<CS12);
	SEC_UNITS=0;
	SEC_TENS=0;
	MIN_UNITS=0;
	MIN_TENS=0;
	HOUR_UNITS=0;
	HOUR_TENS=0;

}

ISR(INT1_vect){
	if(PIND&(1<<3)){
		_delay_ms(30);
		if(PIND&(1<<3)){
			TCCR1B &= ~(1<<CS12);
		}
	}
}

ISR(INT2_vect){
	TCCR1B |= (1<<CS12);
}


ISR(TIMER1_COMPA_vect){
	SEC_UNITS++;
}

