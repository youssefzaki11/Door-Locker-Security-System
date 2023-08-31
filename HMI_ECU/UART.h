/*****************************************************************************************************************
 * File Name: UART.h
 * Date: 23/7/2023
 * Driver: ATmega32 UART Driver Header File
 * Author: Youssef Zaki
 ****************************************************************************************************************/
#include "Standard_types.h"

#ifndef UART_H_
#define UART_H_

/*******************************************************************************************
 *                                      Types Declaration                                  *
 *******************************************************************************************/
typedef enum
{
Asynchronous, Synchronous
}UART_ModeSelect;

typedef enum
{
	Normal, Double_Speed
}UART_AsynchronousSpeed;

typedef enum
{
	Disabled, Reversed, Even_Parity, Odd_Parity
}UART_ParityModeSelect;

typedef enum
{
	One_Bit, Two_Bits
}UART_StopBitSelect;

typedef enum
{
	Five_Bit_0, Six_Bit_1, Seven_Bit_2, Eight_Bit_3, Reserved_4, Reserved_5, Reserved_6, Nine_Bit_7
}UART_BitDataSize;

typedef struct
{
	UART_ModeSelect Mode;
	UART_AsynchronousSpeed Speed;
	UART_ParityModeSelect Parity_Mode;
	UART_StopBitSelect Stop_Bit;
	UART_BitDataSize Data_Size;
	uint32 Baud_Rate;
}UART_ConfigType;
/*******************************************************************************************
 *                                      Functions Prototypes                               *
 *******************************************************************************************/

/*
 * Description:
 * 1. To write in UCRSC Register, so the URSEL bit must be Set.
 * 2. Enable RXEN or TXEN according to device to be receiver or transmitter respectively.
 * 3. Enable RXCIE or TXCIE according to device to be receiver or transmitter respectively (for interrupt enable)
 * 4. Select the Mode of the UART to be Asynchronous or Synchronous
 * 5. Based of the mode to be Asynchronous or Synchronous, Choose the Speed and Baud Rate.
 * 6. from UPM1:0 bits in UCSRC Register, configure the parity mode.
 * 7. from USBS bit in UCSRC Register, Select the number of stop bits to be one or two.
 * 8. from UCSZ2 in UCSRB and from USCZ1:0 in UCSRC bits, select the character size.
 */
void UART_Init(const UART_ConfigType *Config_Ptr);

/*
 * Description:
 * Function to Send byte to the another device.
 * 1. The UDRE is the flag which be set automatically when the Tx Buffer is empty and ready to send new byte.
 * 2. We wait until the buffer is empty, and flag is set to one.
 * 3. The required data is put in UDR Register and consequently, the UDRE flag is cleared while writing.
 */
void UART_SendByte(uint8 Byte);

/*
 * Description:
 * Function to receive byte from the another device.
 * 1. RXC bit is set automatically when the received buffer Rx hasn't any unread data.
 * 2. We wait until the all data is read.
 * 3. Then, we can read the received data from the Rx buffer through UDR Register.
 * 4. After read the data, the RXC flag will be cleared.
 */
uint8 UART_ReceiveByte(void);

/*
 * Description:
 * Function to send string to the another device.
 */
void UART_SendString(const uint8 *Str);

/*
 * Function to receive String from the another device.
 * 1. Receive the required string until the '#' symbol through UART from the other UART device.
 * 2. when the receiving process is finished, we should re-assign the last character to '\0' to read the actual data.
 */
void UART_ReceiveString(uint8 *Str);

#endif /* UART_H_ */
