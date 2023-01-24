/*
 * enc28j60.c
 *
 *  Created on: Aug 6, 2022
 *      Author: Nestor Kolela Kalambay
 */


#include <stdbool.h>

/*
 * Extern Variables
 */



uint8_t enc28j60Bank = 10;
uint16_t nextPacketPointer;
const uint8_t macAddress[] = {0xE4, 0x8E, 0x8D, 0x4F, 0xE1, 0x18}; // make address
const uint8_t destAddress[] = {0xE4, 0x8D, 0x8C, 0x4F, 0xE0, 0x16}; // make address
uint16_t receivePtr = 0;
extern uint8_t offset = 0;
void ENC28J60_Init(void)
{

	//1) Reset the System
	System_Reset();

	Bank_Select(0);

	receivePtr = RXSTART;
	uint8_t tempValue;
	do
	{
		tempValue = Read_Control_Register(ESTAT, 0, false); //read ESTAT register
	}while(!(tempValue & 0x01));



	Bit_Field_Set(ECON2, 0, 0x80);
	nextPacketPointer = RXSTART;

	//2) Receive Buffer


	Bank_Select(0);//select bank 0
	Write_Control_Reg(ERXSTH, 0 ,(RXSTART >> 8));
	Write_Control_Reg(ERXSTL, 0, (RXSTART & 0xFF));

	Write_Control_Reg(ERXNDH, 0, (RXEND >> 8));
	Write_Control_Reg(ERXNDL, 0, (RXEND & 0xFF));

	Write_Control_Reg(ERXRDPTH, 0, (nextPacketPointer >> 8));
	Write_Control_Reg(ERXRDPTL, 0, (nextPacketPointer & 0xFF));

	Write_Control_Reg(ETXSTH, 0, (TXSTART >> 8));
	Write_Control_Reg(ETXSTL, 0, (TXSTART & 0xFF));

	Write_Control_Reg(ETXNDH, 0, (TXEND >> 8));
	Write_Control_Reg(ETXNDL, 0, (TXEND & 0xFF));



	Bank_Select(1);

	//3) Receive Filters
	Write_Control_Reg(ERXFCON, 1, 0b10101001); //This is the default value so ignoring the writing

	//4) MAC Initialization Settings
	//Remember to wait for OST
	Bank_Select(2);
	Write_Control_Reg(MACON1, 2, 0x0D);
#ifdef FULL_DUPLEX
	Write_Control_Reg(MACON3, 2, 0x23);
#else
	Write_Control_Reg(MACON3, 2, 0x20);
#endif
	Write_Control_Reg(MACON4, 2, 0x40);

	Write_Control_Reg(MAMXFLH, 2, (MAXIMUM_FRAME_SIZE >> 8)); //Set maximum frame size to 1518 as per IEEE
	Write_Control_Reg(MAMXFLL, 2, (MAXIMUM_FRAME_SIZE & 0xFF));


	Write_Control_Reg(MAIPGL, 2, 0x12);
	Write_Control_Reg(MAIPGH,  2, 0x0C);
	Write_Control_Reg(MABBIPG, 2, 0x12);

	//Leave MACLCON1 AND MACLCON2 untouched...


	Bank_Select(3);
	Write_Control_Reg(MAADR1, 3, macAddress[0]);
	Write_Control_Reg(MAADR2, 3, macAddress[1]);
	Write_Control_Reg(MAADR3, 3, macAddress[2]);
	Write_Control_Reg(MAADR4, 3, macAddress[3]);
	Write_Control_Reg(MAADR5, 3, macAddress[4]);
	Write_Control_Reg(MAADR6, 3, macAddress[5]);


	//PHY Initialization Settings

	//Remember to write zero to the unimplemented PHY Registers...
#ifdef FULL_DUPLEX
	write_PHY_Register(PHCON1, 0x0100);
#endif

#if LOOP_BACK
	write_PHY_Register(PHCON1, 0x4000);
#endif

	unsigned int nestor = read_PHY_Register(PHCON1);

	write_PHY_Register(PHCON2, 0x0100);

	write_PHY_Register(PHLCON, 0x01DE);// 472

	write_PHY_Register(PHIE, 0x12);

	/*Check the values contained in PHCON1 and SET MACON3 accordingly... FULL- DUPLEX ? */


	Bank_Select(0);
	Write_Control_Reg(ECON1, 0, 0x04); //Enable Packet reception
	Write_Control_Reg(EIE, 0, 0xD3);
}

uint8_t Read_Control_Register(uint8_t reg, uint8_t pBank, bool flag) //
{
	uint8_t pReadValue;
	switch(flag)
	{
	//pass true to function when reading from MAC or MII registers
	case  true:
	{
		uint8_t	tempValue = (READ_CONTROL_REG | (reg & 0x1F));
		enc28j60_CS_Low();
		SPI_send(tempValue);
		SPI_Read();
		pReadValue = SPI_Read(); //read the content
		enc28j60_CS_High();
		return pReadValue;
	}break;

	case false:
	{
		uint8_t	tempValue = (READ_CONTROL_REG | (reg & 0x1F));
		enc28j60_CS_Low();
		SPI_send(tempValue);
		pReadValue = SPI_Read();
		enc28j60_CS_High();
		return pReadValue;
	}break;

	}

}


void Write_Control_Reg(uint8_t reg, uint8_t bank, uint8_t data)
{
	//Bank_Select(bank);//select the bank
	enc28j60_CS_Low();
	SPI_send(WRITE_CONTROL_REG | (reg & 0x1F));
	SPI_send(data);
	enc28j60_CS_High();
}
void Bit_Field_Set(uint8_t reg, uint8_t bank, uint8_t field)
{
	//Bank_Select(bank);
	uint8_t tempValue = BIT_FIELD_SET | (reg & 0x1F);
	enc28j60_CS_Low();
	SPI_send(tempValue);
	SPI_send(field);
	enc28j60_CS_High();
}
void Bit_Field_Clear(uint8_t reg, uint8_t bank, uint8_t field)
{
	//Bank_Select(bank);
	uint8_t tempValue = BIT_FIELD_CLEAR | (reg & 0x1F);
	enc28j60_CS_Low(); // get on the BUS
	SPI_send(tempValue);
	SPI_send(field);
	enc28j60_CS_High();// release the bus

}
void System_Reset(void)
{
	enc28j60_CS_Low();
	SPI_send(SYSTEM_RESET | 0x1F); //
	enc28j60_CS_High(); //release the BUS
}

void Bank_Select(uint8_t bank)
{
	uint8_t tempValue = 0;
	if(bank == enc28j60Bank)
	{
		return;
	}else
	{
		tempValue = Read_Control_Register(ECON1, 0, false);
		tempValue &= 0xFC;
		tempValue = (tempValue | (bank & 0x03));
		Write_Control_Reg(ECON1, 0, tempValue);
		enc28j60Bank = bank;
	}
}

void enc28j60_CS_Low(void)
{
	HAL_GPIO_WritePin(ethernetCS_GPIO_Port, ethernetCS_Pin, GPIO_PIN_RESET);
}
void enc28j60_CS_High(void)
{
	HAL_GPIO_WritePin(ethernetCS_GPIO_Port, ethernetCS_Pin, GPIO_PIN_SET);
}

void SPI_send(uint8_t data)
{
	HAL_SPI_Transmit(&hspi2, &data, 1, HAL_MAX_DELAY);
}
uint8_t SPI_Read(void)
{
	uint8_t pReadValue;
	HAL_SPI_Receive(&hspi2, &pReadValue, 1, HAL_MAX_DELAY);
	return pReadValue;
}


void write_PHY_Register(uint8_t reg, uint16_t data)
{
	//You must first read the PHY register,
	uint16_t currentRegValue = read_PHY_Register(reg); //read the current value
	//
	if(currentRegValue == data)
	{
		return;
	}
	//0b1010; //previous value

	//current //value 0b1111;

	uint16_t valueToWriteBack = 0;
	for(uint8_t cnt = 0; cnt < 16; cnt++)
	{
		if((currentRegValue & (1 << cnt)) != (data & (1 << cnt)))
		{
			if(data & (1 << cnt))
			{
				valueToWriteBack |= (1 << cnt);
			}else
			{
				valueToWriteBack &= ~(1 << cnt);
			}
		}
	}

	Bank_Select(2);//go to bank 2
	Write_Control_Reg(MIREGADR, 2, reg);

	Write_Control_Reg(MIWRL, 2, (uint8_t ) (valueToWriteBack & 0xFF));
	Write_Control_Reg(MIWRH, 2, (uint8_t ) (valueToWriteBack >> 8));

	delay11uS(); //wait for 10.24uS but is it possible in freeRTOS?

}
uint16_t read_PHY_Register(uint8_t reg)
{
	uint16_t returnValue = 0;

	Bank_Select(2);
	Write_Control_Reg(MIREGADR, 2, reg);

	Bank_Select(2);
	Bit_Field_Set(MICMD, 2, 0x01);

	delay11uS();
	Bank_Select(3);


	uint8_t status = 0;
	do
	{
		status = Read_Control_Register(MISTAT, 3, true);

	}while(status & 0x01); // wait here brother.

	Bank_Select(2);
	Bit_Field_Clear(MICMD, 2, 0x01);

	Bank_Select(2);

	returnValue = Read_Control_Register(MIRDL, 2, true);
	uint8_t u8HighByte = Read_Control_Register(MIRDH, 2, true);

	returnValue |= (u8HighByte << 8);

	return returnValue;

}

void delay11uS(void)
{
	//HAL_Delay(1); //wait for 1 mS
	vTaskDelay(1 / portTICK_PERIOD_MS);
}


bool enc28j60_Link_Up(void)
{
	uint16_t status = read_PHY_Register(PHSTAT2);
	return (status & (1 << 10u)) ? true: false;
}

void enc28j60Read_Buffer(uint16_t len, uint8_t * data)
{

}

//uint8_t nestor[1600] = {0x0E, };
void enc28j60Write_Buffer(uint16_t len, uint8_t * data)
{

#if 0

	enc28j60_CS_Low(); //pull it low
	uint16_t pointer = TXSTART; //START by writing to the beginning of the TX Pointer...

	SPI_send(WRITE_BUFFER_MEM | 0x1A); //start by sending this command WBM

	SPI_send(0x0E);

	pointer ++;

	HAL_SPI_Transmit(&hspi2, (uint8_t *) destAddress, sizeof(destAddress), HAL_MAX_DELAY); //send the source address...

	pointer += 6;

	HAL_SPI_Transmit(&hspi2, (uint8_t *) macAddress, sizeof(macAddress), HAL_MAX_DELAY); //send the source address...

	pointer += 6;

	uint8_t lowByte = (len & 0xFF);

	uint8_t highByte = (len >> 8);

	HAL_SPI_Transmit(&hspi2, (uint8_t *) &lowByte, 1, HAL_MAX_DELAY);//send this
	pointer += 1;

	HAL_SPI_Transmit(&hspi2, (uint8_t *) &highByte, 1, HAL_MAX_DELAY);//send this

	pointer += 1;


	HAL_SPI_Transmit(&hspi2, data, len, HAL_MAX_DELAY);//send this

	pointer += len;

	Bank_Select(0);
	Write_Control_Reg(ETXNDH, 0, pointer >> 8);

	Write_Control_Reg(ETXNDL, 0, (pointer & 0xFF));



	//Write the control Byte into memory
	//1)
	//Use WBM SPI to write the packet control byte,the destination address, the source MAC address, the length/length and the payload...
	enc28j60_CS_High();//pull it high

#endif

	enc28j60_CS_Low(); //make it low

	SPI_send(WRITE_BUFFER_MEM | 0x1A); //start by sending this command WBM

	HAL_SPI_Transmit(&hspi2, data, len, HAL_MAX_DELAY); //send all the bytes at once.

	enc28j60_CS_High(); //make it high
}

uint8_t buffer[2048]; //get from the stack quickly

bool enc28j60_BuildAndSend(uint8_t * buf, uint16_t length)
{

	// Load the CONTROL byte
	uint8_t index = 0;



	for(unsigned int x = 0; x < 2048; x++)
	{
		buffer[x] = '\0';
	}


	*(buffer + index++) = 0x0E;

	//Load 6 bytes for the destination address.
	for(uint8_t cnt = 0; cnt < 6; cnt++)
	{
		*(buffer + index++) = destAddress[cnt];
	}

	//Load 6 bytes for the source address.
	for(uint8_t cnt = 0; cnt < 6; cnt++)
	{
		*(buffer + index++) = macAddress[cnt];
	}

	//Load the length
	*(buffer + index++) = (length & 0xFF);
	*(buffer + index++) = (length >> 8);

	uint16_t strLength = strlen((char * ) buf);

	for(uint8_t cnt = 0; cnt < strLength; cnt++)
	{
		*(buffer + index++) = *(buf + cnt);
	}

	//All data has been load so we can proceed...

	enc28j60Write_Buffer(index, buffer);

	//Write the ETXST, WRITE THE POINTER THEN WRITE THE ETXND, then start transmission on the link...
	Bank_Select(0);
	Write_Control_Reg(ETXSTH, 0, (TXSTART >> 8));
	Write_Control_Reg(ETXSTL, 0, (TXSTART & 0xFF));

	//Write the pointer...
	/**/
	Bank_Select(0);
	Write_Control_Reg(EWRPTH, 0, (TXSTART >> 8));
	Write_Control_Reg(EWRPTL, 0, (TXSTART & 0xFF));

	uint16_t end = (TXSTART + index);
	Bank_Select(0);
	Write_Control_Reg(ETXNDH, 0, (end >> 8));
	Write_Control_Reg(ETXNDL, 0, (end & 0xFF));

	enc28j60BeginPacketSend();


	while(enc2860TX_Completed() == false);

}


void enc28j60BeginPacketSend(void)
{
	Bank_Select(0);
	Bit_Field_Set(ECON1, 0, 0x08);
}

void enc28j60EndPacketSend(void)
{
	Bank_Select(0);
	Bit_Field_Clear(ECON1, 0, 0x08); //stop the transmission
}

bool enc2860TX_Completed(void)
{
	Bank_Select(0);
	uint8_t returnValue = Read_Control_Register(ECON1, 0, false);
	return (returnValue & 0x08) ? false : true;
}






