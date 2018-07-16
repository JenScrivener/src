//includes
#include "stm32f4xx.h"
#include "RFM95.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

void init(void);
void flash (void);

//Test

int count=0;

int main(void){


	init();
	RFM95_LoRa_Init2(915.25, 30, RFM95_CODING_RATE_4_8, RFM95_SPREADING_FACTOR_4096CPS, RFM95_BW_500KHZ, 15);

//	$PMTK314,0,0,0,5,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0*2D

//	ping();
	ping2();


}

void init(void){
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD, ENABLE);

	GPIO_InitTypeDef GPIO_InitStruct;
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStruct.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_NOPULL;

	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_12;
	GPIO_Init(GPIOD, &GPIO_InitStruct);

	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_13;
	GPIO_Init(GPIOD, &GPIO_InitStruct);

	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_14;
	GPIO_Init(GPIOD, &GPIO_InitStruct);

	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_15;
	GPIO_Init(GPIOD, &GPIO_InitStruct);

	GPIO_SetBits(GPIOD, GPIO_Pin_12);

	initSPI2();

	initUserButtonEXTI0();
	initLoRaFlagEXTI1();
	initLoRaFlagEXTI2();
//	initLoRaFlagEXTI3();
	initUART1();
	initUART2();
	SysTick_Config(83990);	//2ms per trigger
	USART_ClearITPendingBit(USART1,USART_IT_RXNE);

}

void flash (void){
	switch(count) {
		case 0:
			GPIO_ToggleBits(GPIOD,GPIO_Pin_12);
			GPIO_ToggleBits(GPIOD,GPIO_Pin_13);
			count++;
			break;
		case 1:
			GPIO_ToggleBits(GPIOD,GPIO_Pin_13);
			GPIO_ToggleBits(GPIOD,GPIO_Pin_14);
			count++;
			break;
		case 2:
			GPIO_ToggleBits(GPIOD,GPIO_Pin_14);
			GPIO_ToggleBits(GPIOD,GPIO_Pin_15);
			count++;
			break;
		case 3:
			GPIO_ToggleBits(GPIOD,GPIO_Pin_15);
			GPIO_ToggleBits(GPIOD,GPIO_Pin_12);
			count=0;
			break;
	}
}
