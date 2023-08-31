/*****************************************************************************************************************
 * File Name: EEPROM.c
 * Date: 14/8/2023
 * Driver: EEPROM Driver Source File
 * Author: Youssef Zaki
 ****************************************************************************************************************/
#include <avr/io.h>
#include "I2C.h"
#include "EEPROM.h"

/****************************************************************************************
 *                                     Functions Definitions                            *
 ****************************************************************************************/

uint8 EEPROM_WriteByte(uint16 EEPROM_Byte_Address, uint8 EEPROM_Data)
{
	/* Send the Start Bit */
	TWI_Start();
	if (TWI_GetStatus() != TWI_START)
		return ERROR;

	/* Send the device address, we need to get A8 A9 A10 address bits from the
	 * memory location address and R/W=0 (write) */
	TWI_WriteByte((uint8)(0xA0 | ((EEPROM_Byte_Address & 0x0700)>>7)));
	if (TWI_GetStatus() != TWI_MT_SLA_W_ACK)
		return ERROR;

	/* Send the required memory location address */
	TWI_WriteByte((uint8)(EEPROM_Byte_Address));
	if (TWI_GetStatus() != TWI_MT_DATA_ACK)
		return ERROR;

	/* write byte to EEPROM */
	TWI_WriteByte(EEPROM_Data);
	if (TWI_GetStatus() != TWI_MT_DATA_ACK)
		return ERROR;

	/* Send the Stop Bit */
	TWI_Stop();

	return SUCCESS;
}

uint8 EEPROM_ReadByte(uint16 EEPROM_Byte_Address, uint8 *EEPROM_Data)
{
	/* Send the Start Bit */
	TWI_Start();
	if (TWI_GetStatus() != TWI_START)
		return ERROR;

	/* Send the device address, we need to get A8 A9 A10 address bits from the
	 * memory location address and R/W=0 (write) */
	TWI_WriteByte((uint8)((0xA0) | ((EEPROM_Byte_Address & 0x0700)>>7)));
	if (TWI_GetStatus() != TWI_MT_SLA_W_ACK)
		return ERROR;

	/* Send the required memory location address */
	TWI_WriteByte((uint8)(EEPROM_Byte_Address));
	if (TWI_GetStatus() != TWI_MT_DATA_ACK)
		return ERROR;

	/* Send the Repeated Start Bit */
	TWI_Start();
	if (TWI_GetStatus() != TWI_REP_START)
		return ERROR;

	/* Send the device address, we need to get A8 A9 A10 address bits from the
	 * memory location address and R/W=1 (Read) */
	TWI_WriteByte((uint8)((0xA0) | ((EEPROM_Byte_Address & 0x0700)>>7) | 1));
	if (TWI_GetStatus() != TWI_MT_SLA_R_ACK)
		return ERROR;

	/* Read Byte from Memory without send ACK */
	*EEPROM_Data = TWI_ReadByteWithNACK();
	if (TWI_GetStatus() != TWI_MR_DATA_NACK)
		return ERROR;

	/* Send the Stop Bit */
	TWI_Stop();

	return SUCCESS;
}
