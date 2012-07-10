/*
 * spi.h
 *
 *  Created on: 07.07.2012
 *      Author: paul
 */

#ifndef SPI_H_
#define SPI_H_
#define DDR_SPI DDRB
#define SPI_SlaveSelect DDB2
#define SPI_MOSI DDB3
#define SPI_MISO DDB4
#define SPI_CLK DDB5
/**
 * Set pins initialize DDR registers interrupts etc
 */
void SPIinit();
int8_t SPItransceiveByte(int8_t);

int8_t SPIdisable();

#endif /* SPI_H_ */
