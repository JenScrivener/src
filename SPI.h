/*
 * SPI.h
 *
 *  Created on: 16Mar.,2018
 *      Author: jens0
 */

/*
 * SPI handles all the initialising for the SPI comms including chip select and IRQ, and enables an 8bit SPI send function
 */
#include "stm32f4xx.h"

#ifndef SPI_H_
#define SPI_H_

void initSPI1(void);
void initSPI2(void);
void initUART2(void);
uint8_t SPI_Send(uint8_t Data);
void serial(uint8_t Data);
void burstSerial(char *Data, uint8_t Len);

#endif /* SPI_H_ */
