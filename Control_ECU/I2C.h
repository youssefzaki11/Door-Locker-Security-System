/*****************************************************************************************************************
 * File Name: I2C.h
 * Date: 1/8/2023
 * Driver: ATmega32 I2C Driver Header File
 * Author: Youssef Zaki
 ****************************************************************************************************************/
#include "Standard_Types.h"

#ifndef I2C_H_
#define I2C_H_

/******************************************************************************************
 *                                    Macros Definitions                                  *
 ******************************************************************************************/

/* I2C Status Bits in the TWSR Register */
#define TWI_START         0x08 /* start has been sent */
#define TWI_REP_START     0x10 /* repeated start */
#define TWI_MT_SLA_W_ACK  0x18 /* Master transmit ( slave address + Write request ) to slave + ACK received from slave. */
#define TWI_MT_SLA_R_ACK  0x40 /* Master transmit ( slave address + Read request ) to slave + ACK received from slave. */
#define TWI_MT_DATA_ACK   0x28 /* Master transmit data and ACK has been received from Slave. */
#define TWI_MR_DATA_ACK   0x50 /* Master received data and send ACK to slave. */
#define TWI_MR_DATA_NACK  0x58 /* Master received data but doesn't send ACK to slave. */

/*******************************************************************************************
 *                                      Types Declaration                                  *
 *******************************************************************************************/
typedef enum
{
	Prescaler_1, Prescaler_4, Prescaler_16, Prescaler_64
}TWI_PrescalarSelect;

typedef struct{
	uint8 I2C_Address;
	uint8 I2C_Bit_Rate;
	TWI_PrescalarSelect I2C_Prescaler;
}TWI_ConfigType;
/*******************************************************************************************
 *                                      Functions Prototypes                               *
 *******************************************************************************************/

/*
 * Description:
 * Set TWEN (TWI Enable Bit) bit in TWCR Register to activate TWI operation.
 * Two Wire Bus address my address if any master device want to call me: 0x01(used in case this MC is a slave device)
 * Bit Rate: 400.000 kbps at F_CPU = 8Mhz
 *
 */
void TWI_Init(TWI_ConfigType *Config_Ptr);

/*
 * Description:
 * 1. Clearing TWINT bit in TWCR Register by writing logic one to start transmitting operation.
 * 2. Set TWSTA bit (Start Condition bit) in TWCR register to make the MC is a master device.
 * 3. Set TWEN (TWI Enable Bit) bit in TWCR Register to activate TWI operation.
 * 4. Wait when TWINT bit is set to know that the operation is finished.
 */
void TWI_Start(void);

/*
 * Description:
 * 1. Set TWSTO bit (Stop Condition bit) in TWCR register when the MC is a slave device.
 * 2. Set TWEN (TWI Enable Bit) bit in TWCR Register to activate TWI operation.
 * 3. Clear the TWINT flag before sending the stop bit (TWINT = 1)
 */
void TWI_Stop(void);

/*
 * Description:
 * 1. Let the TWDR Register contains the byte you want to write.
 * 2. Set TWEN (TWI Enable Bit) bit in TWCR Register to activate TWI operation.
 * 3. Clear the TWINT flag before sending the data (TWINT = 1).
 * 4. Wait for TWINT = 1 to ensure that data is send successfully.
 */
void TWI_WriteByte(uint8 Byte);

/*
 * Description:
 * 1. Set TWEN (TWI Enable Bit) bit in TWCR Register to activate TWI operation.
 * 2. Activate TWEA (TWI Enable Acknowledge bit) to generate ACK pulse on TWI Bus after sending data.
 * 3. Clear the TWINT flag before reading data (TWINT = 1).
 * 4. read data from TWDR Register.
 */
uint8 TWI_ReadByteWithACK(void);

/*
 * Description:
 * 1. Set TWEN (TWI Enable Bit) bit in TWCR Register to activate TWI operation.
 * 2. Clear the TWINT flag before reading data (TWINT = 1).
 * 3.Wait for TWINT = 1 to ensure that data is received successfully.
 * 4. read data from TWDR Register.
 */
uint8 TWI_ReadByteWithNACK(void);

/*
 * Description:
 * masking to eliminate first 3 bits and get the last 5 bits (status bits)
 */
uint8 TWI_GetStatus(void);


#endif /* I2C_H_ */
