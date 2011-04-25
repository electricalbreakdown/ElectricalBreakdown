#ifndef F_CPU
#define F_CPU 1000000UL     				
#endif

#include <avr/interrupt.h>
#include <avr/io.h>
#include <util/delay.h>

#define LED_DDR		DDRB
#define LED_PORT	PORTB

#define	LED_R	(1<<PB1)
#define LED_G	(1<<PB3)
#define LED_B	(1<<PB4)

#define DELAY	_delay_ms(50)
volatile uint8_t pwm_r,pwm_g,pwm_b;

//***************************************************************
ISR (TIM0_OVF_vect) 
{
	static uint8_t pwm_count;
	
	pwm_count++;
	if (pwm_count > 63) {
		LED_PORT = 0x00;						// alle LEDs aus
		pwm_count = 0;
	}
	if (pwm_count > pwm_r) LED_PORT |= LED_R;	// rot ein
	if (pwm_count > pwm_g) LED_PORT |= LED_G;	// gelb ein
	if (pwm_count > pwm_b) LED_PORT |= LED_B;	// blau ein
}

//***************************************************************
int main (void) 
{

	LED_DDR = LED_R|LED_G|LED_B;		// Ausgaenge setzen
	TIMSK |= (1 << TOIE0);				// Timer0 Interrupt
	TCCR0B |= (1<<CS00);				// Prescale 1:1024 
	sei();
	
	pwm_r = pwm_g = pwm_b = 63;			// alle LEDs aus (PWM)
		
	while(1)
	{ 
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
	}
}

