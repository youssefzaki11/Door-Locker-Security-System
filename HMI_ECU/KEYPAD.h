/*****************************************************************************************************************
 * File Name: KEYPAD.h
 * Date: 8/8/2023
 * Driver: Keypad Driver Header File
 * Author: Youssef Zaki
 ****************************************************************************************************************/
#include "Standard_Types.h"

#ifndef KEYPAD_H_
#define KEYPAD_H_

#define KEYPAD_TYPE  KEYPAD4x4

#if ((KEYPAD_TYPE != KEYPAD4x4) || (KEYPAD_TYPE != KEYPAD4x3))

#error "Keypad type should be only 4x4 or 3x4"

#endif

/******************************************************************************************
 *                                    Macros Definitions                                  *
 ******************************************************************************************/

/* Keypad Types */
#define KEYPAD4x4                             0x01
#define KEYPAD4x3                             0x02

/* Define the number of rows and columns */
#define KEYPAD_NUMBER_OF_ROWS                 4
#define KEYPAD_NUMBER_OF_COLUMNS              4



/* Setup Eta32mini Keypad PORTs */
#define KEYPAD_ROWS_PORT_ID                   PORTC_ID
#define KEYPAD_COLUMNS_PORT_ID                PORTC_ID

/* Setup Rows Pins */
#define KEYPAD_ROW0_PIN_ID                    PIN0_ID
#define KEYPAD_ROW1_PIN_ID                    PIN1_ID
#define KEYPAD_ROW2_PIN_ID                    PIN2_ID
#define KEYPAD_ROW3_PIN_ID                    PIN3_ID

/* Setup Columns Pins */
#define KEYPAD_COL0_PIN_ID                    PIN4_ID
#define KEYPAD_COL1_PIN_ID                    PIN5_ID
#define KEYPAD_COL2_PIN_ID                    PIN6_ID
#define KEYPAD_COL3_PIN_ID                    PIN7_ID


/*******************************************************************************************
 *                                      Functions Prototypes                               *
 *******************************************************************************************/
/*
 * Description:
 * function to get the pressed key of the Eta32mini KEYPAD or 4x3 and 4x4 Keypads
 */
uint8 KEYPAD_GetPressedKey(void);

#endif /* KEYPAD_H_ */
