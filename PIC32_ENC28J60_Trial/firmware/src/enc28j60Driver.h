/*
 * enc28j60.h
 *
 *  Created on: Aug 6, 2022
 *      Author: kolel
 */

#ifndef INC_ENC28J60_H_
#define INC_ENC28J60_H_

#include "enc28j60defines.h"
#include <stdint.h>
#include <stdbool.h>

void ENC28J60_Init(void); //Initialize the Physic layer

void Bank_Select(uint8_t bank);
uint8_t Read_Control_Register(uint8_t reg, bool flag);
void Write_Control_Reg(uint8_t reg, uint8_t bank, uint8_t data);
void Bit_Field_Set(uint8_t reg, uint8_t bank, uint8_t field);
void Bit_Field_Clear(uint8_t reg, uint8_t bank, uint8_t field);
void System_Reset(void);
void enc28j60_CS_Low(void);
void enc28j60_CS_High(void);
void SPI_send(uint8_t data);
uint8_t SPI_Read(void);
uint16_t read_PHY_Register(uint8_t reg);
void delay11uS(void);
void write_PHY_Register(uint8_t reg, uint16_t data);
bool enc28j60_Link_Up(void);
void enc28j60Read_Buffer(uint16_t len, uint8_t * data);
void enc28j60Write_Buffer(uint16_t len, uint8_t * data);
void enc28j60BeginPacketSend(void);
void enc28j60EndPacketSend(void);
bool enc28j60_BuildAndSend(uint8_t * buf, uint16_t length);
bool enc2860TX_Completed(void);
bool ETH_CheckLinkUp(void); 
#endif /* INC_ENC28J60_H_ */
