/*
 * EXTI.c
 *
 *  Created on: 29Mar.,2018
 *      Author: jens0
 */

#include "EXTI.h"

void initUserButtonEXTI0(void){
	EXTI_InitTypeDef EXTI_InitStruct;
    NVIC_InitTypeDef NVIC_InitStruct;
	GPIO_InitTypeDef GPIO_InitStruct;

	// enable clock for used IO pins and syscfg
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_SYSCFG, ENABLE);

	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_IN;
	GPIO_InitStruct.GPIO_OType = GPIO_OType_OD;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_0;
	GPIO_Init(GPIOA, &GPIO_InitStruct);

    SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOB, EXTI_PinSource1);

	EXTI_InitStruct.EXTI_Line = EXTI_Line0;
	EXTI_InitStruct.EXTI_LineCmd = ENABLE;
	EXTI_InitStruct.EXTI_Mode = EXTI_Mode_Interrupt;
	EXTI_InitStruct.EXTI_Trigger = EXTI_Trigger_Rising;

	EXTI_Init(&EXTI_InitStruct);

    /* Add IRQ vector to NVIC */
    /* PB0 is connected to EXTI_Line0, which has EXTI0_IRQn vector */
    NVIC_InitStruct.NVIC_IRQChannel = EXTI0_IRQn;
    /* Set priority */
    NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority = 0x00;
    /* Set sub priority */
    NVIC_InitStruct.NVIC_IRQChannelSubPriority = 0x00;
    /* Enable interrupt */
    NVIC_InitStruct.NVIC_IRQChannelCmd = ENABLE;
    /* Add to NVIC */
    NVIC_Init(&NVIC_InitStruct);
}

void initLoRaFlagEXTI1(void){

	EXTI_InitTypeDef EXTI_InitStruct;
	NVIC_InitTypeDef NVIC_InitStruct;
	GPIO_InitTypeDef GPIO_InitStruct;

	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_SYSCFG, ENABLE);

	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_IN;
	GPIO_InitStruct.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_1;
	GPIO_Init(GPIOB, &GPIO_InitStruct);

	SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOB, EXTI_PinSource1);

	EXTI_InitStruct.EXTI_Line = EXTI_Line1;
	EXTI_InitStruct.EXTI_LineCmd = ENABLE;
	EXTI_InitStruct.EXTI_Mode = EXTI_Mode_Interrupt;
	EXTI_InitStruct.EXTI_Trigger = EXTI_Trigger_Rising;

	EXTI_Init(&EXTI_InitStruct);

	/* Add IRQ vector to NVIC */
	/* PB2 is connected to EXTI_Line2, which has EXTI2_IRQn vector */
	NVIC_InitStruct.NVIC_IRQChannel = EXTI1_IRQn;
	/* Set priority */
	NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority = 0x00;
	/* Set sub priority */
	NVIC_InitStruct.NVIC_IRQChannelSubPriority = 0x00;
	/* Enable interrupt */
	NVIC_InitStruct.NVIC_IRQChannelCmd = ENABLE;
	/* Add to NVIC */
	NVIC_Init(&NVIC_InitStruct);
}

void initLoRaFlagEXTI2(void){

	EXTI_InitTypeDef EXTI_InitStruct;
	NVIC_InitTypeDef NVIC_InitStruct;
	GPIO_InitTypeDef GPIO_InitStruct;

	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_SYSCFG, ENABLE);

	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_IN;
	GPIO_InitStruct.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_2;
	GPIO_Init(GPIOB, &GPIO_InitStruct);

	SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOB, EXTI_PinSource2);

	EXTI_InitStruct.EXTI_Line = EXTI_Line2;
	EXTI_InitStruct.EXTI_LineCmd = ENABLE;
	EXTI_InitStruct.EXTI_Mode = EXTI_Mode_Interrupt;
	EXTI_InitStruct.EXTI_Trigger = EXTI_Trigger_Rising;

	EXTI_Init(&EXTI_InitStruct);

	/* Add IRQ vector to NVIC */
	/* PB2 is connected to EXTI_Line2, which has EXTI2_IRQn vector */
	NVIC_InitStruct.NVIC_IRQChannel = EXTI2_IRQn;
	/* Set priority */
	NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority = 0x00;
	/* Set sub priority */
	NVIC_InitStruct.NVIC_IRQChannelSubPriority = 0x00;
	/* Enable interrupt */
	NVIC_InitStruct.NVIC_IRQChannelCmd = ENABLE;
	/* Add to NVIC */
	NVIC_Init(&NVIC_InitStruct);
}

void EXTI0_IRQHandler(void) {

	GPIO_ToggleBits(GPIOD,GPIO_Pin_12);
	GPIO_ToggleBits(GPIOD,GPIO_Pin_13);
	for(int x=0;x<1000000;x++);
	EXTI_ClearFlag(EXTI_Line0);

}

/*EXTI2 is handled in RFM95 */

//void EXTI2_IRQHandler(void) {
//	uint8_t IRQ_Flags;
//	RFM95_Reg_Read(RFM95_REG_12_IRQ_FLAGS, &IRQ_Flags, 1);
//
//	if(IRQ_Flags&RFM95_RX_TIMEOUT || IRQ_Flags&RFM95_PAYLOAD_CRC_ERROR){
//		// Bad RX
//		Clear_Flags();
//	}
//	else if (IRQ_Flags&RFM95_RX_DONE && IRQ_Flags&RFM95_VALID_HEADER){
//		//recieve data
//
//		uint8_t rxbase = 0;												//Set FifoPtrAddr to FifoRxCurrentAddr
//		RFM95_Reg_Read(RFM95_REG_10_FIFO_RX_CURRENT_ADDR,&rxbase,1);
//		RFM95_Reg_Write(RFM95_REG_0D_FIFO_ADDR_PTR , &rxbase, 1);
//
//		uint8_t len =0;
//		RFM95_Reg_Read(RFM95_REG_22_PAYLOAD_LENGTH,&len,1);
//
//		uint8_t *buf = (uint8_t*) malloc(len);
//		RFM95_Reg_Read(RFM95_REG_00_FIFO, buf, len);
//
//		burstSerial(buf,len);
//		free(buf);
//
//		Clear_Flags();
//
//	}
//	else if(IRQ_Flags&RFM95_TX_DONE){
//		//finish sending
//		GOODTX=1;
//
//		Clear_Flags();
//	}
//	else{
//		Clear_Flags();
//	}
//}
