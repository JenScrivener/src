/*
 * RFM95.c
 *
 *  Created on: 16Mar.,2018
 *      Author: jens0
 */

#include "RFM95.h"

int timer=1000;
uint8_t ADDRESS = 0x01;

void RFM95_Reg_Write(uint8_t Reg, uint8_t* Data, uint8_t Len){		//Write len bytes of data to reg

	Reg |= RFM95_WRITE;												//Set first bit to write

	CS(0);															//Pull chip select low to begin communication
	SPI_Send(Reg);													//Send the write command

	for(int x=0;x<Len;x++){
		SPI_Send(*Data);											//Write the xth byte of data
		Data++;
	}

	CS(1);															//Set chip select high to end communication

}

void RFM95_Reg_Read(uint8_t Reg, uint8_t* Data, uint8_t Len){		//Read len bytes of data from reg

	CS(0);															//Pull chip select low to begin communication
	SPI_Send(Reg);													//Send the read command

	for(int x=0;x<Len;x++){
		*Data=SPI_Send(Reg);										//Write the xth byte of data
		Data++;
	}

	CS(1);															//Set chip select high to end communication

}

void RFM95_Set_Mode(uint8_t Mode){									//Set Mode reg (0x01)

	if(Mode&~RFM95_MODE){											//If setting the whole OP Mode
		RFM95_Reg_Write(RFM95_REG_01_OP_MODE, &Mode, 1);
	}
	else{
		uint8_t mode=0;
		mode = RFM95_Get_Mode();									//setting just the mode (last three bits)
		mode&=~RFM95_MODE;
		mode|=Mode;
		RFM95_Reg_Write(RFM95_REG_01_OP_MODE, &mode, 1);
	}

}

uint8_t RFM95_Get_Mode(void){										//Read Mode reg (0x01)

	uint8_t mode=0;
	RFM95_Reg_Read(RFM95_REG_01_OP_MODE, &mode, 1);
	return(mode);

}

void RFM95_Set_Freq(double Freq){									//Set freq reg (0x06,07&08) to Freq (freq should be in MHz i.e 915)

	uint8_t *freq =(uint8_t*) malloc(3);
	int temp1 = (Freq*524288/XOSC);
	int temp2 = 0;

	freq=freq+3;
	for(int x=0;x<3;x++){
		freq--;

		temp2 = temp1;
		temp2 &= 0xFF;
		*freq = temp2;

		temp1=temp1>>8;
		temp2=temp2>>8;
	}

	RFM95_Reg_Write(RFM95_REG_06_FRF_MSB, freq, 3);
	free(freq);
}

double RFM95_Get_Freq(void){										//Get freq from reg(0x06,07,08) in MHz

	uint8_t freq[3];
	RFM95_Reg_Read(RFM95_REG_06_FRF_MSB, &freq[0], 3);

	int temp=0;
	double ret=0;

	temp|=freq[0];
	for(int x=1;x<3;x++){
		temp=temp<<8;
		temp|=freq[x];
	}

	ret=(double)temp/524288;
	ret=ret*XOSC;
	return(ret);
}

void RFM95_Set_Payload_Length(uint8_t PLL){

	RFM95_Reg_Write(RFM95_REG_22_PAYLOAD_LENGTH, &PLL, 1);
}

uint8_t RFM95_Get_Payload_Length(void){

	uint8_t PLL=0;
	RFM95_Reg_Read(RFM95_REG_22_PAYLOAD_LENGTH, &PLL, 1);
	return(PLL);
}

void RFM95_Set_Coding_Rate(uint8_t CR){

	uint8_t temp=0;
	RFM95_Reg_Read(RFM95_REG_1D_MODEM_CONFIG1, &temp, 1);			//current value of the registor

	CR=CR<<1;
	CR&=RFM95_CODING_RATE;
	temp&=!RFM95_CODING_RATE;										//clear the current CR
	CR|=temp;														//set the new CR
	RFM95_Reg_Write(RFM95_REG_1D_MODEM_CONFIG1, &CR, 1);			//write to the radio
}

uint8_t RFM95_Get_Coding_Rate(void){

	uint8_t CR=0;
	RFM95_Reg_Read(RFM95_REG_1D_MODEM_CONFIG1, &CR, 1);
	CR&=RFM95_CODING_RATE;
	CR=CR>>1;
	return(CR);
}

void RFM95_Set_Spreading_Factor(uint8_t SF){

	uint8_t temp=0;
	RFM95_Reg_Read(RFM95_REG_1E_MODEM_CONFIG2, &temp, 1);			//current value of the registor

	SF=SF<<4;
	SF&=RFM95_SPREADING_FACTOR;
	temp&=!RFM95_SPREADING_FACTOR;									//clear the current SF
	SF|=temp;														//set the new SF
	RFM95_Reg_Write(RFM95_REG_1E_MODEM_CONFIG2, &SF, 1);
}

uint8_t RFM95_Get_Spreading_Factor(void){

	uint8_t SF=0;
	RFM95_Reg_Read(RFM95_REG_1E_MODEM_CONFIG2, &SF, 1);
	SF&=RFM95_SPREADING_FACTOR;
	SF=SF>>4;
	return(SF);
}

void RFM95_Set_Bandwidth(uint8_t BW){

	uint8_t temp=0;
	RFM95_Reg_Read(RFM95_REG_1D_MODEM_CONFIG1, &temp, 1);			//current value of the registor

	BW=BW<<4;
	BW&=RFM95_BW;
	temp&=!RFM95_BW;												//clear the current BW
	BW|=temp;														//set the new BW
	RFM95_Reg_Write(RFM95_REG_1D_MODEM_CONFIG1, &BW, 1);
}

uint8_t RFM95_Get_Bandwidth(void){

	uint8_t BW=0;
	RFM95_Reg_Read(RFM95_REG_1D_MODEM_CONFIG1, &BW, 1);				//current value of the registor
	BW&=RFM95_BW;
	BW=BW>>4;
	return(BW);
}

void RFM95_Set_Output_Power(uint8_t OPP){

	uint8_t temp=0;
	RFM95_Reg_Read(RFM95_REG_09_PA_CONFIG, &temp, 1);				//current value of the registor

	OPP&=RFM95_OUTPUT_POWER;
	temp&=!RFM95_OUTPUT_POWER;										//clear the current OPP
	OPP|=temp;														//set the new OPP
	OPP|=RFM95_PA_SELECT;											//set PA_BOOST
	RFM95_Reg_Write(RFM95_REG_09_PA_CONFIG, &OPP, 1);
}

uint8_t RFM95_Get_Output_Power(void){

	uint8_t OPP=0;
	RFM95_Reg_Read(RFM95_REG_09_PA_CONFIG, &OPP, 1);				//current value of the registor
	OPP&=RFM95_OUTPUT_POWER;
	return(OPP);
}

void RFM95_Set_Hop_Period(uint8_t HP){

	RFM95_Reg_Write(RFM95_REG_24_HOP_PERIOD , &HP, 1);
}

uint8_t RFM95_Get_Hop_Period(void){

	uint8_t HP=0;
	RFM95_Reg_Read(RFM95_REG_24_HOP_PERIOD , &HP, 1);
	return(HP);
}

void RFM95_DIO_MapReg1(uint8_t DIO, uint8_t Map){
	uint8_t map =0;

	RFM95_Reg_Read(RFM95_REG_40_DIO_MAPPING1 , &map, 1);
	map=map&!DIO;
	Map=Map&0x03;
	DIO=DIO/3;
	while(DIO!=1){
		DIO=DIO>>2;
		Map=Map<<2;
	}
	Map=Map|map;
	RFM95_Reg_Write(RFM95_REG_40_DIO_MAPPING1 , &Map, 1);

}

void RFM95_DIO_MapReg2(uint8_t DIO, uint8_t Map){
	uint8_t map =0;

	RFM95_Reg_Read(RFM95_REG_41_DIO_MAPPING2 , &map, 1);
	map=map&!DIO;
	Map=Map&0x03;
	DIO=DIO/3;
	while(DIO!=1){
		DIO=DIO>>2;
		Map=Map<<2;
	}
	Map=Map|map;
	RFM95_Reg_Write(RFM95_REG_41_DIO_MAPPING2 , &Map, 1);

}

uint8_t RFM95_Get_DIO_MapReg1(uint8_t DIO){
	uint8_t map =0;

	RFM95_Reg_Read(RFM95_REG_40_DIO_MAPPING1 , &map, 1);
	map=map&DIO;
	DIO=DIO/3;
	while(DIO!=1){
		DIO=DIO>>2;
		map=map>>2;
	}
	return (map);

}

uint8_t RFM95_Get_DIO_MapReg2(uint8_t DIO){
	uint8_t map =0;

	RFM95_Reg_Read(RFM95_REG_41_DIO_MAPPING2 , &map, 1);
	map=map&DIO;
	DIO=DIO/3;
	while(DIO!=1){
		DIO=DIO>>2;
		map=map>>2;
	}
	return (map);
}

void RFM95_LoRa_Init(double Freq){									//Use default settings for all registers except for frequency registors.

	uint8_t mode = RFM95_LONG_RANGE_MODE;
	mode|=RFM95_MODE_SLEEP;

	RFM95_Set_Mode(mode);
	RFM95_Set_Mode(mode);
	RFM95_Set_Freq(Freq);

}

void RFM95_LoRa_Init2(double Freq, uint8_t PayloadLength, uint8_t CodingRate, uint8_t SpreadingFactor, uint8_t Bandwidth, uint8_t OutputPower){

	uint8_t mode = RFM95_LONG_RANGE_MODE;
	mode|=RFM95_MODE_SLEEP;

	RFM95_Set_Mode(mode);											//enter LoRa mode, first to enter sleep mode
	RFM95_Set_Mode(mode);											//then to enter LoRa mode.

	RFM95_Set_Freq(Freq);
	RFM95_Set_Payload_Length(PayloadLength);
	RFM95_Reg_Write(RFM95_REG_13_RX_NB_BYTES, &PayloadLength, 1);
	RFM95_Set_Coding_Rate(CodingRate);
	RFM95_Set_Spreading_Factor(SpreadingFactor);
	RFM95_Set_Bandwidth(Bandwidth);
	RFM95_Set_Output_Power(OutputPower);

	RFM95_Reg_Read(RFM95_REG_1D_MODEM_CONFIG1, &mode, 1);			//Implicit header mode
	mode|=RFM95_IMPLICIT_HEADER_MODE_ON;
	RFM95_Reg_Write(RFM95_REG_1D_MODEM_CONFIG1, &mode, 1);

	RFM95_DIO_MapReg1(RFM95_DIO0,3);
//	RFM95_DIO_MapReg1(RFM95_DIO3,1);
	RFM95_Set_Hop_Period(3);

}

void SysTick_Handler(void){
	timer--;
}

void RFM95_LoRa_Test_Send(uint8_t Data){							//Send one byte without addressing

	RFM95_Set_Mode(RFM95_LONG_RANGE_MODE|RFM95_MODE_STDBY);			//Enter stand by mode

	uint8_t txbase = 0;												//Set FifoPtrAddr to FifoTxPtrBase
	RFM95_Reg_Read(RFM95_REG_0E_FIFO_TX_BASE_ADDR,&txbase,1);
	RFM95_Reg_Write(RFM95_REG_0D_FIFO_ADDR_PTR , &txbase, 1);

	RFM95_Reg_Write(RFM95_REG_00_FIFO , &Data, 1);					//Write data to FIFO

	RFM95_Set_Mode(RFM95_LONG_RANGE_MODE|RFM95_MODE_TX);			//Enter Transmit mode

}

void RFM95_LoRa_Test_Send2(uint8_t *Data, uint8_t Len){

	RFM95_Set_Mode(RFM95_LONG_RANGE_MODE|RFM95_MODE_STDBY);			//Enter stand by mode
	RFM95_Set_Freq(915.25);
	uint8_t txbase = 0;												//Set FifoPtrAddr to FifoTxPtrBase
	RFM95_Reg_Read(RFM95_REG_0E_FIFO_TX_BASE_ADDR,&txbase,1);
	RFM95_Reg_Write(RFM95_REG_0D_FIFO_ADDR_PTR , &txbase, 1);

	RFM95_Reg_Write(RFM95_REG_22_PAYLOAD_LENGTH, &Len, 1);			//Set the payload length


	RFM95_Reg_Write(RFM95_REG_00_FIFO , Data, Len);					//Write data to FIFO

	RFM95_Set_Mode(RFM95_LONG_RANGE_MODE|RFM95_MODE_TX);			//Enter Transmit mode

//	uint8_t txdone=0;
//	while(!txdone){
//		RFM95_Reg_Read(0x12, &txdone, 1);
//	}
//
//	Clear_Flags2();
}

void RFM95_LoRa_Test_Send3(void){									//Resend last transmision (requires implicit header mode and FifoTxPtrBase=0x80)

	uint8_t txbase=0x80;
	RFM95_Reg_Write(RFM95_REG_0D_FIFO_ADDR_PTR , &txbase, 1);

	RFM95_Set_Mode(RFM95_LONG_RANGE_MODE|RFM95_MODE_TX);			//Enter Transmit mode

}

uint8_t RFM95_LoRa_Test_Recieve(void){								//Recieve one byte

	RFM95_Set_Mode(RFM95_LONG_RANGE_MODE|RFM95_MODE_RXCONTINUOUS);	//Enter RX continuous mode

	uint8_t rxdone=0;												//Wait to recieve a packet
	while(!rxdone){
		RFM95_Reg_Read(RFM95_REG_12_IRQ_FLAGS , &rxdone, 1);
		rxdone&=RFM95_RX_DONE;
	}

	rxdone=0xff;													//clear flags
	RFM95_Reg_Write(RFM95_REG_12_IRQ_FLAGS , &rxdone, 1);
	rxdone=0x00;
	RFM95_Reg_Write(RFM95_REG_12_IRQ_FLAGS , &rxdone, 1);

	uint8_t rxbase = 0;												//Set FifoPtrAddr to FifoRxCurrentAddr
	RFM95_Reg_Read(RFM95_REG_10_FIFO_RX_CURRENT_ADDR,&rxbase,1);
	RFM95_Reg_Write(RFM95_REG_0D_FIFO_ADDR_PTR , &rxbase, 1);

	uint8_t data=0;													//Read data from the FIFO
	RFM95_Reg_Read(RFM95_REG_00_FIFO , &data, 1);
	return(data);
}


//void EXTI2_IRQHandler(void) {
//	uint8_t IRQ_Flags;
//	RFM95_Reg_Read(RFM95_REG_12_IRQ_FLAGS, &IRQ_Flags, 1);
//
//	if(IRQ_Flags&RFM95_RX_TIMEOUT || IRQ_Flags&RFM95_PAYLOAD_CRC_ERROR){
//		// Bad RX
//		Clear_Flags2();
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
//		Clear_Flags2();
//
//	}
//	else if(IRQ_Flags&RFM95_TX_DONE){
//		//finish sending
//		GOODTX=1;
//
//		Clear_Flags2();
//	}
//	else{
//		Clear_Flags2();
//	}
//}

void Clear_Flags1(void){

	uint8_t IRQ_Flags=RFM95_FHSS_CHANGE_CHANNEL_MASK;				//clear flags on LoRa Radio
	RFM95_Reg_Write(RFM95_REG_12_IRQ_FLAGS , &IRQ_Flags, 1);
	RFM95_Reg_Write(RFM95_REG_12_IRQ_FLAGS , &IRQ_Flags, 1);

	EXTI_ClearFlag(EXTI_Line1);										//clear flags on STM

}

void Clear_Flags2(void){

	uint8_t IRQ_Flags=RFM95_TX_DONE_MASK | RFM95_RX_DONE_MASK | RFM95_VALID_HEADER_MASK;											//clear flags on LoRa Radio
	RFM95_Reg_Write(RFM95_REG_12_IRQ_FLAGS , &IRQ_Flags, 1);
	RFM95_Reg_Write(RFM95_REG_12_IRQ_FLAGS , &IRQ_Flags, 1);

	EXTI_ClearFlag(EXTI_Line2);										//clear flags on STM
}

void ping(void){
//	#define ping

	uint8_t data=0;
	char serial[40]="waiting for signal";
	while(1){

		if(timer<0){
			GPIO_SetBits(GPIOD,GPIO_Pin_12);
			RFM95_LoRa_Test_Send(data);
			GPIO_ResetBits(GPIOD,GPIO_Pin_12);
			sprintf(serial, "waiting for signal.");
			burstSerial(&serial[0], strlen(serial));
			timer=5000;

			RFM95_Set_Mode(RFM95_LONG_RANGE_MODE|RFM95_MODE_RXCONTINUOUS);
		}
		else{
			//wait
		}
	}
}

#ifdef ping

void EXTI2_IRQHandler(void) {
	uint8_t IRQ_Flags;
//	uint8_t rssi=0;
//	int RSSI=0;
	char serial[80];


	RFM95_Reg_Read(RFM95_REG_12_IRQ_FLAGS, &IRQ_Flags, 1);

	if (IRQ_Flags&RFM95_RX_DONE && IRQ_Flags&RFM95_VALID_HEADER){
//		RFM95_Reg_Read(RFM95_REG_1A_PKT_RSSI_VALUE , &rssi, 1);
//		RSSI=rssi-157;
//		sprintf(serial, "RSSI = %d" , RSSI);
//		burstSerial(&serial[0], strlen(serial));

		timer=4000;

		uint8_t rxbase = 0;												//Set FifoPtrAddr to FifoRxCurrentAddr
		RFM95_Reg_Read(RFM95_REG_10_FIFO_RX_CURRENT_ADDR,&rxbase,1);
		RFM95_Reg_Write(RFM95_REG_0D_FIFO_ADDR_PTR , &rxbase, 1);

		uint8_t len =0;
		RFM95_Reg_Read(RFM95_REG_22_PAYLOAD_LENGTH,&len,1);

		uint8_t *buf = (uint8_t*) malloc(len);
		RFM95_Reg_Read(RFM95_REG_00_FIFO, buf, len);

		burstSerial((char*)buf,len);
		free(buf);

		sprintf(serial, "Hello World, Test, Testing 1,2,3,4,5,6, Hello");
		RFM95_LoRa_Test_Send2((uint8_t*)&serial,strlen(serial));
		Clear_Flags2();
	}
	if(IRQ_Flags&RFM95_TX_DONE){
		RFM95_Set_Mode(RFM95_LONG_RANGE_MODE|RFM95_MODE_RXCONTINUOUS);
		sprintf(serial, "tx");
		burstSerial(&serial[0], strlen(serial));
		RFM95_Set_Freq(915.25);
		Clear_Flags2();
	}

}

#endif /* ping */

void ping2(void){
	#define ping2
	RFM95_Set_Mode(RFM95_LONG_RANGE_MODE|RFM95_MODE_RXCONTINUOUS);
	while(1){
//		if(USART1->SR & USART_SR_RXNE){
//			char t = USART_ReceiveData(USART1);
//			serial(t);
//			GPIO_ToggleBits(GPIOD,GPIO_Pin_12);
//			GPIO_ToggleBits(GPIOD,GPIO_Pin_13);
//		}
	}
}

#ifdef ping2

void EXTI2_IRQHandler(void){
	uint8_t IRQ_Flags;
	uint8_t rssi_temp;
	int RSSI;
	char data[40]={0};

	RFM95_Reg_Read(RFM95_REG_12_IRQ_FLAGS, &IRQ_Flags, 1);

	if (IRQ_Flags&RFM95_RX_DONE && IRQ_Flags&RFM95_VALID_HEADER){
		RFM95_Set_Freq(915.25);
		uint8_t rxbase = 0;												//Set FifoPtrAddr to FifoRxCurrentAddr
		RFM95_Reg_Read(RFM95_REG_10_FIFO_RX_CURRENT_ADDR,&rxbase,1);
		RFM95_Reg_Write(RFM95_REG_0D_FIFO_ADDR_PTR , &rxbase, 1);

		uint8_t len =0;
		RFM95_Reg_Read(RFM95_REG_22_PAYLOAD_LENGTH,&len,1);

		uint8_t *buf2 = (uint8_t*) malloc(len);
		RFM95_Reg_Read(RFM95_REG_00_FIFO, buf2, len);

		if(*buf2==ADDRESS){
//		if(1){
			buf2+=4;
			RFM95_Reg_Read(RFM95_REG_1A_PKT_RSSI_VALUE, &rssi_temp, 1);
			RSSI=rssi_temp-157;
			burstSerial((char*)buf2,len-4);
			char serial[40]={0};
			sprintf(serial, "My RSSI was %d",RSSI);
			burstSerial(&serial[0], strlen(serial));
			GPIO_ToggleBits(GPIOD,GPIO_Pin_12);
			GPIO_ToggleBits(GPIOD,GPIO_Pin_13);
			sprintf(data, "The other node's RSSI was %d",RSSI);

			for(int x=0;x<4;x++){
				serial[x]=1;
			}
			serial[0]=(char)ADDRESS;
			for(int x=4;x<40;x++){
				serial[x]=data[x-4];
			}

			RFM95_LoRa_Test_Send2((uint8_t*)&serial,strlen(serial));

//			free(buf2);
		}
		else{
			char serial[40];
			sprintf(serial, "not for me :-[");
			burstSerial(&serial[0], strlen(serial));
		}

		Clear_Flags2();
	}

	if(IRQ_Flags&RFM95_TX_DONE){
		RFM95_Set_Freq(915.25);
		Clear_Flags2();
		RFM95_Set_Mode(RFM95_LONG_RANGE_MODE|RFM95_MODE_RXCONTINUOUS);
	}

//	else{
//		Clear_Flags2();
//	}
}

#endif /* ping2 */


void EXTI1_IRQHandler(void) {
	uint8_t IRQ_Flags;

	RFM95_Reg_Read(RFM95_REG_12_IRQ_FLAGS, &IRQ_Flags, 1);
	if(IRQ_Flags&RFM95_FHSS_CHANGE_CHANNEL){
		Hop();
		Clear_Flags1();
	}
}

void Hop (void){
	uint8_t hop;
	double freq;

	RFM95_Reg_Read(RFM95_REG_1C_HOP_CHANNEL, &hop, 1);
	hop&=RFM95_FHSS_PRESENT_CHANNEL;
	hop=hop%20;

	if(hop>1){
		freq = (915+LIPD_BW/2)+((LIPD_BW+LIPD_Gap)*(hop));
	}
	else{
		freq=915.25;
	}

	RFM95_Set_Freq(freq);
//	char serial[80];
//	sprintf(serial, "freq = %f %d" ,freq,hop);
//	burstSerial(&serial[0], strlen(serial));
}

//void EXTI0_IRQHandler(void) {
//	char serial[40];
//	char data[40];
//	sprintf(data, "Hello World, Test, Testing 1");
//
//	for(int x=0;x<4;x++){
//		serial[x]=1;
//	}
//	serial[0]=(char)ADDRESS;
//	for(int x=4;x<40;x++){
//		serial[x]=data[x-4];
//	}
//
//	RFM95_LoRa_Test_Send2((uint8_t*)&serial,strlen(serial));
//	EXTI_ClearFlag(EXTI_Line0);
//}

void EXTI0_IRQHandler(void) {

	char serial[40];
	RTC_TimeTypeDef RTC_TimeStruct;
	RTC_GetTime(RTC_Format_BIN, &RTC_TimeStruct);
	sprintf(serial, "Time is %d:%d.%d",RTC_TimeStruct.RTC_Hours,RTC_TimeStruct.RTC_Minutes,RTC_TimeStruct.RTC_Seconds);
	burstSerial(&serial[0], strlen(serial));

	EXTI_ClearFlag(EXTI_Line0);
}

void initTimer(void){

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM5, ENABLE);

	TIM_RemapConfig(TIM5, TIM5_LSI);

	TIM_PrescalerConfig(TIM5, 0, TIM_PSCReloadMode_Immediate);

	TIM_ICInitTypeDef TIM_ICInitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	RCC_ClocksTypeDef  RCC_ClockFreq;

	TIM_ICInitStructure.TIM_Channel = TIM_Channel_4;
	TIM_ICInitStructure.TIM_ICPolarity = TIM_ICPolarity_Rising;
	TIM_ICInitStructure.TIM_ICSelection = TIM_ICSelection_DirectTI;
	TIM_ICInitStructure.TIM_ICPrescaler = TIM_ICPSC_DIV8;
	TIM_ICInitStructure.TIM_ICFilter = 0xF;
	TIM_ICInit(TIM5, &TIM_ICInitStructure);

	NVIC_InitStructure.NVIC_IRQChannel = TIM5_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);

	/* Enable TIM5 counter */
	TIM_Cmd(TIM5, ENABLE);

	/* Reset the flags */
	TIM5->SR = 0;

	/* Enable the CC4 Interrupt Request */
	TIM_ITConfig(TIM5, TIM_IT_CC4, ENABLE);

//	TIM5->ARR = 0;
}

void TIM5_IRQHandler(void){

	char serial[40];
	sprintf(serial,"Working, %d",(int)(TIM5->CCR4));
	burstSerial(&serial[0], strlen(serial));
	TIM5->CNT=0;
	TIM_ClearFlag(TIM5,TIM_FLAG_CC4);
}

void initRTC(void){

	//Enable Back up domain write access
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR,ENABLE);
	PWR_BackupAccessCmd(ENABLE);

	//Reset Back up domain
	RCC->BDCR |= RCC_BDCR_BDRST;
	RCC->BDCR &=!RCC_BDCR_BDRST;

	//Turn on the Low speed internal oscillator
	RCC->CSR |= RCC_CSR_LSION;
	while(!(RCC->CSR&RCC_CSR_LSIRDY));

	//Set RTCCLK to LSI
	RCC_RTCCLKConfig(RCC_RTCCLKSource_LSI);
	//Enable RTC
	RCC_RTCCLKCmd(ENABLE);

	//Disable write protection
	RTC_WriteProtectionCmd(DISABLE);

	//Enter initialization mode
	RTC->ISR |= RTC_ISR_INIT;
	while(!(RTC->ISR&RTC_ISR_INITF));

	//Set date and time
	RTC_InitTypeDef RTC_InitStruct;
	RTC_InitStruct.RTC_HourFormat=RTC_HourFormat_24;
	RTC_Init(&RTC_InitStruct);

	RTC_TimeTypeDef RTC_TimeStruct;
	RTC_TimeStruct.RTC_H12=RTC_H12_AM;
	RTC_TimeStruct.RTC_Hours=8;
	RTC_TimeStruct.RTC_Minutes=0;
	RTC_TimeStruct.RTC_Seconds=0;
	RTC_SetTime(RTC_Format_BIN,&RTC_TimeStruct);

	RTC_DateTypeDef RTC_DateStruct;
	RTC_DateStruct.RTC_Date=1;
	RTC_DateStruct.RTC_Month=RTC_Month_January;
	RTC_DateStruct.RTC_Year=18;
	RTC_DateStruct.RTC_WeekDay=RTC_Weekday_Monday;
	RTC_SetDate(RTC_Format_BCD,&RTC_DateStruct);

//	RTC_AlarmTypeDef RTC_AlarmStruct;
//	RTC_AlarmStruct.RTC_AlarmTime = RTC_TimeStruct;

	//Exit initialization mode
	RTC->ISR &=!RTC_ISR_INIT;

	//Enable write protection
	RTC_WriteProtectionCmd(ENABLE);


}

void USART1_IRQHandler(void){
	if(USART_GetITStatus(USART1,USART_IT_RXNE)){
		char t = USART_ReceiveData(USART1);
		serial(t);
		GPIO_ToggleBits(GPIOD,GPIO_Pin_12);
		GPIO_ToggleBits(GPIOD,GPIO_Pin_13);
	}
}
