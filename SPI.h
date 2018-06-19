/*
 * SPI.h
 *
 *  Created on: 16Mar.,2018
 *      Author: jens0
 */

#include "stm32f4xx.h"

#ifndef SPI_H_
#define SPI_H_

void initSPI1(void);
void initSPI2(void);
void initUART1(void);
void initUART2(void);
uint8_t SPI_Send(uint8_t Data);
void serial1(uint8_t Data);
void burstSerial1(char *Data, uint8_t Len);
void serial2(uint8_t Data);
void burstSerial2(char *Data, uint8_t Len);
void USART2_IRQHandler(void);
#endif /* SPI_H_ */
