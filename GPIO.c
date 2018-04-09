/*
 * GPIO.c
 *
 *  Created on: 19Mar.,2018
 *      Author: jens0
 */
#include "GPIO.h"

void CS(uint8_t state){
	if(state){
		GPIO_SetBits(GPIOB, GPIO_Pin_12);
	}
	else{
		GPIO_ResetBits(GPIOB, GPIO_Pin_12);
	}
}
