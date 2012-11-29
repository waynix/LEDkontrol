/*
 * spitest.c
 *
 *  Created on: 07.07.2012
 *      Author: Paul Rosset
 */
#include <avr/io.h>
#include <avr/interrupt.h>
#include <stdio.h>
#include <util/delay.h>
#include "spi.h"
/**
 * Global Variables
 */
volatile int16_t framebuffer[]={0x0,0x0,0x0800,0x03F,0x00,0x00,0x00,0x0};
int8_t serialize[4];
volatile uint8_t counter;

uint8_t getRed(int16_t c16)
{
	return (c16 >> 10)& 0x3E;
}
uint8_t getGreen(int16_t c16)
{
	return (c16 >> 5)& 0x3F;
}
uint8_t getBlue(int16_t c16)
{
	return (c16 << 1) & 0x3E;
}
/** Set the LED array to a new Image*/
void update(int h)
{
	for (int i = 0; i < 8; i++)
	{
		//HSV colorspace
		int current = (h+i * 10)%360;//46
		int8_t block = (current) / 60;
		int16_t hi = block <<5;

		int16_t f = (current) <<5;

		f /= 60;
		f -= hi;
		int p = 0;
		int q = 31 - f;
		int t = f;
		int V= 31;

                //Set led to color value
		switch (block)
		{
		case 0:
		case 6:
			framebuffer[i]=(V <<11)|(t<<6)|p;

			break;
		case 1:
			framebuffer[i]=(q <<11)|(V<<6)|p;

			break;
		case 2:
			framebuffer[i]=(p <<11)|(V<<6)|t;

			break;
		case 3:
			framebuffer[i]=(p <<11)|(q<<6)|V;

			break;
		case 4:
			framebuffer[i]=(t <<11)|(p<<6)|V;

			break;
		case 5:
			framebuffer[i]=(V <<11)|(p<<6)|q;

			break;
	}
	}

}
/**
 * Convert the Color value for each LED to power times
 */
void renderColor(volatile int16_t buff[], unsigned int counter, int ledcount)
{
	int8_t tmp = 0;
	for(int i = 0; i< ledcount ; i++)
	{
		int8_t bitcolor = 0;
		if(getRed(buff[i]) > counter)
		{
			bitcolor = 4;
		}
		if(getGreen(buff[i])> counter)
		{
			bitcolor |= 2;
		}
		if(getBlue(buff[i]) > counter)
		{
			bitcolor |= 1;
		}
		if((i & 1) == 0)
		{
			tmp =((bitcolor) << 4);
		}
		else
		{
			tmp |=((bitcolor) << 1);
			SPItransceiveByte(tmp);
			tmp = 0;
		}
	}
}

/**
 * Initialisation routine
 * */
int main()
{
	//Set the timer
	//WGM10 - 13 ctc
	TCCR1A = (0 << WGM10) | (0 << WGM11);
	TCCR1B = (0 << WGM13) | (1 << WGM12);
	// divide system clock by 64
	TCCR1B |= (0 << CS12) | (1 << CS11) | (1 << CS10);
	TIMSK1 = (1 << OCIE1A);
	OCR1A = 80;
	SPIinit();

	int16_t cnt = 0;
	DDRD |= (1 << PD5);
	sei();
	//Main loop
	while(1)
	{
		_delay_ms(5);


		PORTD ^= (1<< PD5);
		update(cnt);

		cnt+=1;
		cnt %= 360;
	}


}

ISR(TIMER1_OVF_vect){}

/**
 * Timer1 Overflow Vector
 * This timer is used to switch the leds on and of depending on their color value
 * */
ISR(TIMER1_COMPA_vect)
{
    char tmp = SREG; //backup SREG state
    cli();

    PORTB &= ~(1<<(SPI_SlaveSelect));

    counter++;
    counter %= 0x3F;
    renderColor(framebuffer,counter,8);
    
    PORTB |= (1<<SPI_SlaveSelect);
    SREG = tmp;//Restore SREG
}
