/*
 * GPIO.h
 *
 *  Created on: 19Mar.,2018
 *      Author: jens0
 */

/*
 * only used for setting and resetting chip select. almost certainly an example of terrible coding but I think it works
 */

#ifndef GPIO_H_
#define GPIO_H_

#include "stm32f4xx.h"

void CS(uint8_t state);


#endif /* GPIO_H_ */
