/*****************************************************************************************************************
 * File Name: KEYPAD.c
 * Date: 8/8/2023
 * Driver: Keypad Driver Source File
 * Author: Youssef Zaki
 ****************************************************************************************************************/
#include <util/delay.h>
#include "KEYPAD.h"
#include "GPIO.h"

/****************************************************************************************
 *                                     Functions Definitions                            *
 ****************************************************************************************/
//#define KEYPAD4x4Eta32mini

#ifndef KEYPAD4x4Eta32mini

#if (KEYPAD_NUMBER_OF_COLUMNS == 3)

/*
 * Description:
 * Adjust the pressed button according to Proteus 4x3 Keypad
 */
static uint8 KEYPAD_4x3_AdjustKeyNumber(uint8 Pressed_Button);

#elif (KEYPAD_NUMBER_OF_COLUMNS == 4)

/*
 * Description:
 * Adjust the pressed button according to Proteus 4x4 Keypad
 */
static uint8 KEYPAD_4x4_AdjustKeyNumber(uint8 Pressed_Button);

#endif

#endif /* KEYPAD4x4Eta32mini */

uint8 KEYPAD_GetPressedKey(void)
{
	uint8 row;
	uint8 col;

	/* 4 rows and 3 columns */
	GPIO_SetupPinDirection(KEYPAD_ROWS_PORT_ID, KEYPAD_ROW0_PIN_ID, INPUT_PIN);
	GPIO_SetupPinDirection(KEYPAD_ROWS_PORT_ID, KEYPAD_ROW1_PIN_ID, INPUT_PIN);
	GPIO_SetupPinDirection(KEYPAD_ROWS_PORT_ID, KEYPAD_ROW2_PIN_ID, INPUT_PIN);
	GPIO_SetupPinDirection(KEYPAD_ROWS_PORT_ID, KEYPAD_ROW3_PIN_ID, INPUT_PIN);

	GPIO_SetupPinDirection(KEYPAD_COLUMNS_PORT_ID, KEYPAD_COL0_PIN_ID, INPUT_PIN);
	GPIO_SetupPinDirection(KEYPAD_COLUMNS_PORT_ID, KEYPAD_COL1_PIN_ID, INPUT_PIN);
	GPIO_SetupPinDirection(KEYPAD_COLUMNS_PORT_ID, KEYPAD_COL2_PIN_ID, INPUT_PIN);

	/* 4 rows and 4 columns */
#if (KEYPAD_NUMBER_OF_COLUMNS == 4)
	GPIO_SetupPinDirection(KEYPAD_COLUMNS_PORT_ID, KEYPAD_COL3_PIN_ID, INPUT_PIN);
#endif

	while (1)
	{
		_delay_ms(200);
		for (row = 0; row < KEYPAD_NUMBER_OF_ROWS; row++)
		{
			GPIO_SetupPinDirection(KEYPAD_ROWS_PORT_ID, KEYPAD_ROW0_PIN_ID+row, OUTPUT_PIN);
			GPIO_WritePin(KEYPAD_ROWS_PORT_ID, KEYPAD_ROW0_PIN_ID+row, LOGIC_LOW);

			for (col = 0; col < KEYPAD_NUMBER_OF_COLUMNS; col++)
			{
				if (GPIO_ReadPin(KEYPAD_COLUMNS_PORT_ID, KEYPAD_COL0_PIN_ID + col) == LOGIC_LOW)
				{
                    #if (KEYPAD_NUMBER_OF_COLUMNS == 4)
                         #ifdef KEYPAD4x4Eta32mini
					            return ((row*KEYPAD_NUMBER_OF_COLUMNS) + col +1);
                         #else
					            return KEYPAD_4x4_AdjustKeyNumber((row*KEYPAD_NUMBER_OF_COLUMNS) + col + 1);
                         #endif
                    #elif (KEYPAD_NUMBER_OF_COLUMNS == 3)
                         #ifdef KEYPAD4x4Eta32mini
					            return ((row*KEYPAD_NUMBER_OF_COLUMNS) + col +1);
                         #else
					            return KEYPAD_4x3_AdjustKeyNumber((row*KEYPAD_NUMBER_OF_COLUMNS) + col + 1);
                         #endif
                    #endif
				}
			}
			GPIO_SetupPinDirection(KEYPAD_ROWS_PORT_ID, KEYPAD_ROW0_PIN_ID+row, INPUT_PIN);
		}
	}
}

#ifndef KEYPAD4x4Eta32mini

#if (KEYPAD_NUMBER_OF_COLUMNS == 3)

/*
 * Description:
 * Adjust the pressed button according to Proteus 4x3 Keypad
 */
static uint8 KEYPAD_4x3_AdjustKeyNumber(uint8 Pressed_Button)
{
	uint8 Button_Number = 0;

	switch (Pressed_Button)
		{
		case 10:
			Button_Number = '*';
			break;
		case 11:
			Button_Number = 0;
			break;
		case 12:
			Button_Number = '#';
			break;
		default:
			Button_Number = Pressed_Button;
			break;
		}
	return Button_Number;
}

#elif (KEYPAD_NUMBER_OF_COLUMNS == 4)

/*
 * Description:
 * Adjust the pressed button according to Proteus 4x4 Keypad
 */
static uint8 KEYPAD_4x4_AdjustKeyNumber(uint8 Pressed_Button)
{
	uint8 Button_Number = 0;

	switch (Pressed_Button)
	{
	case 1:
		Button_Number = 7;
		break;
	case 2:
		Button_Number = 8;
		break;
	case 3:
		Button_Number = 9;
		break;
	case 4:
		Button_Number = '%';
		break;
	case 5:
		Button_Number = 4;
		break;
	case 6:
		Button_Number = 5;
		break;
	case 7:
		Button_Number = 6;
		break;
	case 8:
		Button_Number = '*';
		break;
	case 9:
		Button_Number = 1;
		break;
	case 10:
		Button_Number = 2;
		break;
	case 11:
		Button_Number = 3;
		break;
	case 12:
		Button_Number = '-';
		break;
	case 13:
		Button_Number = 13;
		break;
	case 14:
		Button_Number = 0;
		break;
	case 15:
		Button_Number = '=';
		break;
	case 16:
		Button_Number = '+';
		break;
	default:
		Button_Number = Pressed_Button;
		break;
	}
	return Button_Number;
}

#endif

#endif /* KEYPAD4x4Eta32mini */
