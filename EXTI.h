/*
 * EXTI.h
 *
 *  Created on: 29Mar.,2018
 *      Author: jens0
 */

#ifndef EXTI_H_
#define EXTI_H_

#include "stm32f4xx.h"

void initUserButtonEXTI0(void);
void initLoRaFlagEXTI1(void);
void initLoRaFlagEXTI2(void);

void EXTI0_IRQHandler(void);
//void EXTI1_IRQHandler(void); 	//handled in RFM95
//void EXTI2_IRQHandler(void); 	//handled in RFM95

#endif /* EXTI_H_ */
