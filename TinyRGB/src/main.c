#ifndef F_CPU
#define F_CPU 1000000UL     				
#endif

#include <avr/interrupt.h>
#include <avr/io.h>
#include <util/delay.h>
#include <avr/eeprom.h>

///TODO: Check if this is needed
// EEMEM wird bei aktuellen Versionen der avr-lib in eeprom.h definiert
// hier: definiere falls noch nicht bekannt ("alte" avr-libc)
#ifndef EEMEM
// alle Textstellen EEMEM im Quellcode durch __attribute__ ... ersetzen
#define EEMEM  __attribute__ ((section (".eeprom")))
#endif

#define LED_DDR		DDRB
#define LED_PORT	PORTB
///WARNING: We are working inverted here so high means actually OFF and low ON
#define	LED_R	(1<<PB1)
#define LED_G	(1<<PB4)
#define LED_B	(1<<PB3)

#define DELAY	_delay_ms(50)
volatile uint8_t pwm_r,pwm_g,pwm_b;
uint8_t mode = 3;
volatile uint8_t isr_count = 0;
volatile uint8_t ledstate = 255;

//***************************************************************
ISR (TIM0_OVF_vect) 
{
	///save value of timer register at beginning of ISR routine and disable
	///timer so that we are able to returne to the same state after the
	///routine finished
	uint8_t timer = TCNT0;
	TIMSK &= ~(1 << TOIE0); // disable timer interrupt	
	//static uint8_t pwm_count;

	///We want to keep track how often the isr routine is called	
	++isr_count;
	if(isr_count >= 256) isr_count = 0;
	
	LED_PORT = ledstate;
	//if(!(ledstate & LED_R))	LED_PORT &= ~LED_R;	
	
	//LED_PORT ^= LED_R;
	///reset timer on old value and enable interrupt routine
	TCNT0 = 254;
	TIMSK |= (1<< TOIE0);
}

//***************************************************************
int main (void) 
{

	LED_DDR = LED_R|LED_G|LED_B;		// Ausgaenge setzen
	LED_PORT |= (LED_R|LED_G|LED_B);
	//ledstate = LED_PORT;
	///TODO: Check here in datasheet that this goes well
	TIMSK |= (1 << TOIE0);				// Timer0 Interrupt
	TCCR0B |= (1<<CS00);				// Prescale 1:1024 
	sei();
	
	pwm_r = pwm_g = pwm_b = 63;			// alle LEDs aus (PWM)
		
	while(1)
	{
	///here goes the code that actually does something
	///we want to have different operation modes so use switch

	switch(mode) {
		case 0:

			ledstate &= ~LED_R;	// rot ein
			break;
		case 1:
			ledstate &= ~LED_G;	// gelb ein
			break;
		case 2:
			ledstate &= ~LED_B;	// blau ein
			break;
		case 3:
			if(isr_count > 253) {
				ledstate &= ~LED_R;
			} else
				ledstate |= LED_R;
			break;
		default:
			break;	
	}
///TODO: remove legacy code
/* 
		// Rot einzeln
		for (pwm_r = 63; pwm_r > 0; pwm_r--) DELAY;
		for (pwm_r = 0; pwm_r < 64; pwm_r++) DELAY;

		// Gruen einzeln
		for (pwm_g = 63; pwm_g > 0; pwm_g--) DELAY;
		for (pwm_g = 0; pwm_g < 64; pwm_g++) DELAY;
		
		// Blau einzeln
		for (pwm_b = 63; pwm_b > 0; pwm_b--) DELAY;
		for (pwm_b = 0; pwm_b < 64; pwm_b++) DELAY;
		
		// Rot gegen Gruen
		for (pwm_r = 63; pwm_r > 0; pwm_r--){
			pwm_g = 63 - pwm_r;
			DELAY;
		}
		for (pwm_r = 0; pwm_r < 64; pwm_r++){
			pwm_g = 63 - pwm_r;
			DELAY;
		}

		// Rot gegen Blau		
		pwm_g = 63;			
		for (pwm_r = 63; pwm_r > 0; pwm_r--){
			pwm_b = 63 - pwm_r;
			DELAY;
		}
		for (pwm_r = 0; pwm_r < 64; pwm_r++){
			pwm_b = 63 - pwm_r;
			DELAY;
		}

		// Gruen gegen Blau		
		pwm_b = 63;			
		for (pwm_g = 63; pwm_g > 0; pwm_g--){
			pwm_b = 63 - pwm_g;
			DELAY;
		}
		for (pwm_g = 0; pwm_g < 64; pwm_g++){
			pwm_b = 63 - pwm_g;
			DELAY;
		}

		// alle gleichzeitig
		pwm_b = 63;			
		for (pwm_r = 63; pwm_r > 0; pwm_r--){
			pwm_g = pwm_b = pwm_r;
			DELAY;
		}
		for (pwm_r = 0; pwm_r < 64; pwm_r++){
			pwm_g = pwm_b = pwm_r;
			DELAY;
		}
*/
	}
}

