/*****************************************************************************************************************
 * File Name: LCD.c
 * Date: 3/8/2023
 * Driver: LCD Driver Source File
 * Author: Youssef Zaki
 ****************************************************************************************************************/
#include <avr/io.h>
#include <util/delay.h>
#include "LCD.h"
#include "Common_Macros.h"
#include "GPIO.h"

/****************************************************************************************
 *                                     Functions Definitions                            *
 ****************************************************************************************/

/*
 * Description:
 * 1. Setup the LCD pins directions by GPIO Driver
 * 2. Setup the LCD Data Mode 4-bits or 8-bits.
 */
void LCD_Init(void)
{
	/* Setup the RS and E pins as an Output pins to control the LCD */
	GPIO_SetupPinDirection(LCD_RS_PORT, LCD_RS_PIN, OUTPUT_PIN);
	GPIO_SetupPinDirection(LCD_E_PORT, LCD_E_PIN, OUTPUT_PIN);

	/* LCD Power ON delay always > 15ms */
	_delay_ms(20);

#if (LCD_BIT_MODE == 8)

	/* let all pins in LCD_DATA_PORT to be an output pins to connect with LCD Data pins  */
	GPIO_SetupPortDirection(LCD_DATA_PORT, OUTPUT_PORT);

	/* Send the command of the 8-bit mode to LCD */
	LCD_SendCommand(LCD_TWO_LINES_EIGHT_BIT_MODE);

#elif (LCD_BIT_MODE == 4)

	/* make the 4 Pins in the LCD Data Port as output pins to connect with LCD */
	GPIO_SetupPinDirection(LCD_DATA_PORT, LCD_DB4_PIN_ID, OUTPUT_PIN);
	GPIO_SetupPinDirection(LCD_DATA_PORT, LCD_DB5_PIN_ID, OUTPUT_PIN);
	GPIO_SetupPinDirection(LCD_DATA_PORT, LCD_DB6_PIN_ID, OUTPUT_PIN);
	GPIO_SetupPinDirection(LCD_DATA_PORT, LCD_DB7_PIN_ID, OUTPUT_PIN);

	/* Send for 4-bit initialization of LCD  */
	LCD_SendCommand(LCD_TWO_LINES_FOUR_BITS_MODE_INIT1);
	LCD_SendCommand(LCD_TWO_LINES_FOUR_BITS_MODE_INIT2);

	/* Send the command of the 4-bit mode to LCD */
	LCD_SendCommand(LCD_TWO_LINES_FOUR_BIT_MODE);

#endif

	/*
	 * Initialize LCD Screen by sending two commands:
	 * 1. Clear Screen before writing new data at the beginning
	 * 2. Turn Off the cursor
	 */
	LCD_SendCommand(CLEAR_DISPLAY_SCREEN);
	LCD_SendCommand(DISPLAY_ON_CURSOR_OFF);
}

/*
 * Description:
 * Send Command to LCD
 */
void LCD_SendCommand(uint8 Command)
{
	/* Register Select Pin RS = 0 -> Transferring Instruction (Command) to LCD */
	GPIO_WritePin(LCD_RS_PORT, LCD_RS_PIN, LOGIC_LOW);

	/* processing of "tas" = 50 nsec, so delaying for 1 us */
	_delay_us(1);

	/* Data Enable Pin E = 1 -> Enable the LCD */
	GPIO_WritePin(LCD_E_PORT, LCD_E_PIN, LOGIC_HIGH);

	/* processing of (tpw - tdsw) = (290 - 100) = 190 nsec, so delaying 1 us*/
	_delay_us(1);

#if (LCD_BIT_MODE == 8)

	/* Send the Instruction from Micro-Controller to the LCD through LCD Data Port (D0 : D7) */
	GPIO_WritePORT(LCD_DATA_PORT, Command);

	/* processing tdsw = 100 ns, so delaying 1 us */
	_delay_us(1);

	/* Data Enable Pin E = 0 -> Disable the LCD */
	GPIO_WritePin(LCD_E_PORT, LCD_E_PIN, LOGIC_LOW);

	/* processing of (tpw - tdsw) = (290 - 100) = 190 nsec, so delaying 1 us*/
	_delay_us(1);

#elif (LCD_BIT_MODE == 4)

	/* Sending the Higher nibble (4 bits) of the command through 4 pins of Micro-Controller to LCD */
	GPIO_WritePin(LCD_DATA_PORT, LCD_DB4_PIN_ID, GET_BIT(Command, 4));
	GPIO_WritePin(LCD_DATA_PORT, LCD_DB5_PIN_ID, GET_BIT(Command, 5));
	GPIO_WritePin(LCD_DATA_PORT, LCD_DB6_PIN_ID, GET_BIT(Command, 6));
	GPIO_WritePin(LCD_DATA_PORT, LCD_DB7_PIN_ID, GET_BIT(Command, 7));

	/* processing tdsw = 100 ns, so delaying 1 us */
	_delay_ms(1);

	/* Data Enable Pin E = 0 -> Disable the LCD */
	GPIO_WritePin(LCD_E_PORT, LCD_E_PIN, LOGIC_LOW);

	/* delay for processing th = 13ns, so delaying 1 us */
	_delay_ms(1);

	/* Data Enable Pin E = 1 -> Enable the LCD */
	GPIO_WritePin(LCD_E_PORT, LCD_E_PIN, LOGIC_HIGH);

	/* processing of (tpw - tdsw) = (290 - 100) = 190 nsec, so delaying 1 us*/
	_delay_ms(1);

	/* Sending the Lower nibble (4 bits) of the command through 4 pins of Micro-Controller to LCD */
	GPIO_WritePin(LCD_DATA_PORT, LCD_DB4_PIN_ID, GET_BIT(Command, 0));
	GPIO_WritePin(LCD_DATA_PORT, LCD_DB5_PIN_ID, GET_BIT(Command, 1));
	GPIO_WritePin(LCD_DATA_PORT, LCD_DB6_PIN_ID, GET_BIT(Command, 2));
	GPIO_WritePin(LCD_DATA_PORT, LCD_DB7_PIN_ID, GET_BIT(Command, 3));

	/* processing tdsw = 100 ns, so delaying 1 us */
	_delay_ms(1);

	/* Data Enable Pin E = 0 -> Disable the LCD */
	GPIO_WritePin(LCD_E_PORT, LCD_E_PIN, LOGIC_LOW);

	/* delay for processing th = 13ns, so delaying 1 us */
	_delay_ms(1);

#endif
}

/*
 * Description:
 * Display Character on LCD
 */
void LCD_DisplayCharacter(uint8 Data)
{
	/* Register Select Pin RS = 1 -> Transferring Data to LCD */
	GPIO_WritePin(LCD_RS_PORT, LCD_RS_PIN, LOGIC_HIGH);

	/* processing of "tas" = 50 nsec, so delaying for 1 us */
	_delay_ms(1);

	/* Data Enable Pin E = 1 -> Enable the LCD */
	GPIO_WritePin(LCD_E_PORT, LCD_E_PIN, LOGIC_HIGH);

	/* processing of (tpw - tdsw) = (290 - 100) = 190 nsec, so delaying 1 us*/
	_delay_ms(1);

#if (LCD_BIT_MODE == 8)

	/* Send the data from Micro-Controller to the LCD through LCD Data Port (D0 : D7) */
	GPIO_WritePORT(LCD_DATA_PORT, Data);

	/* processing tdsw = 100 ns, so delaying 1 us */
	_delay_us(1);

	/* Data Enable Pin E = 0 -> Disable the LCD */
	GPIO_WritePin(LCD_E_PORT, LCD_E_PIN, LOGIC_LOW);

	/* processing of (tpw - tdsw) = (290 - 100) = 190 nsec, so delaying 1 us*/
	_delay_us(1);

#elif (LCD_BIT_MODE == 4)

	/* Sending the Higher nibble (4 bits) of the data through 4 pins of Micro-Controller to LCD */
	GPIO_WritePin(LCD_DATA_PORT, LCD_DB4_PIN_ID, GET_BIT(Data, 4));
	GPIO_WritePin(LCD_DATA_PORT, LCD_DB5_PIN_ID, GET_BIT(Data, 5));
	GPIO_WritePin(LCD_DATA_PORT, LCD_DB6_PIN_ID, GET_BIT(Data, 6));
	GPIO_WritePin(LCD_DATA_PORT, LCD_DB7_PIN_ID, GET_BIT(Data, 7));

	/* processing tdsw = 100 ns, so delaying 1 us */
	_delay_ms(1);

	/* Data Enable Pin E = 0 -> Disable the LCD */
	GPIO_WritePin(LCD_E_PORT, LCD_E_PIN, LOGIC_LOW);

	/* delay for processing th = 13ns, so delaying 1 us */
	_delay_ms(1);

	/* Data Enable Pin E = 1 -> Enable the LCD */
	GPIO_WritePin(LCD_E_PORT, LCD_E_PIN, LOGIC_HIGH);

	/* processing of (tpw - tdsw) = (290 - 100) = 190 nsec, so delaying 1 us*/
	_delay_ms(1);

	/* Sending the Lower nibble (4 bits) of the data through 4 pins of Micro-Controller to LCD */
	GPIO_WritePin(LCD_DATA_PORT, LCD_DB4_PIN_ID, GET_BIT(Data, 0));
	GPIO_WritePin(LCD_DATA_PORT, LCD_DB5_PIN_ID, GET_BIT(Data, 1));
	GPIO_WritePin(LCD_DATA_PORT, LCD_DB6_PIN_ID, GET_BIT(Data, 2));
	GPIO_WritePin(LCD_DATA_PORT, LCD_DB7_PIN_ID, GET_BIT(Data, 3));

	/* processing tdsw = 100 ns, so delaying 1 us */
	_delay_ms(1);

	/* Data Enable Pin E = 0 -> Disable the LCD */
	GPIO_WritePin(LCD_E_PORT, LCD_E_PIN, LOGIC_LOW);

	/* delay for processing th = 13ns, so delaying 1 us */
	_delay_ms(1);

#endif
}

/*
 * Description:
 * Display String on LCD
 */
void LCD_DisplayString(const char *Str)
{
	uint8 i = 0;
	while(Str[i] != '\0')
	{
		LCD_DisplayCharacter(Str[i]);
		i++;
	}

	/*
	while (*Str != '\0')
	{
		LCD_DisplayCharacter(*Str);
		Str++;
	}
	*/
}

/*
 * Description:
 * Move cursor to the required position on LCD
 */
void LCD_MoveCursor(uint8 row,uint8 col)
{
	uint8 LCD_Memory_Address;

	/* Calculate the required address in the LCD DDRAM */
	switch (row)
	{
	case 0:
		LCD_Memory_Address = col;
		break;
	case 1:
		LCD_Memory_Address = col+0x40;
		break;
	case 2:
		LCD_Memory_Address = col+0x10;
		break;
	case 3:
		LCD_Memory_Address = col+0x50;
		break;
	}

	/* Move the LCD cursor to this specific address */
	LCD_SendCommand(LCD_Memory_Address | SET_CURSOR_POSITION);
}

/*
 * Description:
 * 1. Move cursor to the required position on LCD
 * 2. Display string from the required position determined by cursor
 */
void LCD_DisplayStringRowColumn(uint8 row, uint8 col, const char *Str)
{
	/* Go to the required LCD Position */
	LCD_MoveCursor(row, col);

	/* Display the Required String */
	LCD_DisplayString(Str);
}

/*
 * Description:
 * Clear string on LCD
 */
void LCD_ClearString(void)
{
	LCD_SendCommand(CLEAR_DISPLAY_SCREEN);
}

/*
 * Description:
 * Display the required decimal value on the screen
 * Data variable type is "integer" because uint8 max value is only 255.
 */
void LCD_IntegerToString(int Data)
{
	/* String to hold the ASCII result */
	char buff[16];

	/* Use itoa C function to convert the data to its corresponding ASCII value, 10 for decimal */
	itoa(Data, buff, 10);

	 /* Display the string */
	LCD_DisplayString(buff);
}
