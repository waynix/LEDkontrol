/*
 * spi.c
 *
 *  Created on: 07.07.2012
 *      Author: paul
 */

#include "avr/io.h"
#include "spi.h"
void SPIinit() {

	DDR_SPI = (1 << SPI_SlaveSelect) |
			  (1 << SPI_MOSI) |
			  (0 << SPI_MISO) |
			  (1 << SPI_CLK);
	SPCR = (1 << SPE) |
		   (0 << DORD) |
		   (1 << MSTR) |
		   (0 << SPR1) |
		   (0 << SPR0) |
		   (0 << CPOL) |
		   (0 << CPHA);
	//SPSR = (1<<SPI2X);
	PORTB |= (1 << SPI_SlaveSelect);
}
/*
 * Send and receive a byte to over the SPI bus and wait until sending is finished.
 * param
 */
int8_t SPItransceiveByte(int8_t value)
{
	SPDR = value;
	while(!(SPSR & (1<<SPIF)));
	return 0;
}
