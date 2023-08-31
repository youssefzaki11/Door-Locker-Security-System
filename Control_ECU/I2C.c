/*****************************************************************************************************************
 * File Name: I2C.c
 * Date: 1/8/2023
 * Driver: ATmega32 I2C Driver Source File
 * Author: Youssef Zaki
 ****************************************************************************************************************/
#include <avr/io.h>
#include "I2C.h"
#include "Common_Macros.h"

/****************************************************************************************
 *                                     Functions Definitions                            *
 ****************************************************************************************/

/*
 * Description:
 * Set TWEN (TWI Enable Bit) bit in TWCR Register to activate TWI operation.
 * Two Wire Bus address my address if any master device want to call me: 0x01(used in case this MC is a slave device)
 * Bit Rate: 400.000 kbps at F_CPU = 8Mhz
 *
 */
void TWI_Init(TWI_ConfigType *Config_Ptr)
{
	/* Setting Bit Rate and pre-scaler values from configurable structure  */
	    TWBR = (Config_Ptr -> I2C_Bit_Rate);
		TWSR = (Config_Ptr -> I2C_Prescaler);

		/* Setting Bus address value from configurable structure  */
		TWAR = ((Config_Ptr -> I2C_Address) << TWA0);

		/* enable TWI */
	    TWCR = (1<<TWEN);
}

/*
 * Description:
 * 1. Clearing TWINT bit in TWCR Register by writing logic one to start transmitting operation.
 * 2. Set TWSTA bit (Start Condition bit) in TWCR register to make the MC is a master device.
 * 3. Set TWEN (TWI Enable Bit) bit in TWCR Register to activate TWI operation.
 * 4. Wait when TWINT bit is set to know that the operation is finished.
 */
void TWI_Start(void)
{
	TWCR |= (1<<TWINT) | (1<<TWSTA) | (1<<TWEN);

	while (BIT_IS_CLEAR(TWCR, TWINT));
}

/*
 * Description:
 * 1. Set TWSTO bit (Stop Condition bit) in TWCR register when the MC is a slave device.
 * 2. Set TWEN (TWI Enable Bit) bit in TWCR Register to activate TWI operation.
 * 3. Clear the TWINT flag before sending the stop bit (TWINT = 1)
 */
void TWI_Stop(void)
{
	TWCR = (1<<TWINT) | (1<<TWSTO) | (1<<TWEN);
}

/*
 * Description:
 * 1. Let the TWDR Register contains the byte you want to write.
 * 2. Set TWEN (TWI Enable Bit) bit in TWCR Register to activate TWI operation.
 * 3. Clear the TWINT flag before sending the data (TWINT = 1).
 * 4. Wait for TWINT = 1 to ensure that data is send successfully.
 */
void TWI_WriteByte(uint8 Byte)
{
	TWDR = Byte;

	TWCR = (1<<TWINT) | (1<<TWEN);

	while(BIT_IS_CLEAR(TWCR, TWINT));
}

/*
 * Description:
 * 1. Set TWEN (TWI Enable Bit) bit in TWCR Register to activate TWI operation.
 * 2. Activate TWEA (TWI Enable Acknowledge bit) to generate ACK pulse on TWI Bus after sending data.
 * 3. Clear the TWINT flag before reading data (TWINT = 1).
 * 4. read data from TWDR Register.
 */
uint8 TWI_ReadByteWithACK(void)
{
	TWCR = (1<<TWINT) | (1<<TWEA) | (1<<TWEN);

	while(BIT_IS_CLEAR(TWCR, TWINT));

	return TWDR;
}

/*
 * Description:
 * 1. Set TWEN (TWI Enable Bit) bit in TWCR Register to activate TWI operation.
 * 2. Clear the TWINT flag before reading data (TWINT = 1).
 * 3.Wait for TWINT = 1 to ensure that data is received successfully.
 * 4. read data from TWDR Register.
 */
uint8 TWI_ReadByteWithNACK(void)
{
	TWCR = (1<<TWINT) | (1<<TWEN);

	while(BIT_IS_CLEAR(TWCR, TWINT));

	return TWDR;
}

/*
 * Description:
 * masking to eliminate first 3 bits and get the last 5 bits (status bits)
 */
uint8 TWI_GetStatus(void)
{
    uint8 status;

    status = TWSR & 0xF8;

    return status;
}
