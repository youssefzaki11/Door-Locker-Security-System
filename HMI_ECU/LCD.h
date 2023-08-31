/*****************************************************************************************************************
 * File Name: LCD.h
 * Date: 3/8/2023
 * Driver: LCD Driver Header File
 * Author: Youssef Zaki
 ****************************************************************************************************************/
#include "Standard_Types.h"

#ifndef LCD_H_
#define LCD_H_

#define LCD_BIT_MODE                               8

#if ((LCD_BIT_MODE != 4) && (LCD_BIT_MODE != 8))

#error "LCD Mode should be 4 or 8"

#endif

/******************************************************************************************
 *                                    Macros Definitions                                  *
 ******************************************************************************************/

/* LCD Modes */
#define LCD_ONE_LINE_FOUR_BIT_MODE                 0x20
#define LCD_TWO_LINES_FOUR_BIT_MODE                0x28
#define LCD_ONE_LINE_EIGHT_BIT_MODE                0x30
#define LCD_TWO_LINES_EIGHT_BIT_MODE               0x38

#define LCD_TWO_LINES_FOUR_BITS_MODE_INIT1         0x33
#define LCD_TWO_LINES_FOUR_BITS_MODE_INIT2         0x32

/* LCD Commands */
#define CLEAR_DISPLAY_SCREEN                       0x01
#define RETURN_HOME                                0x02
#define SHIFT_CURSOR_TO_LEFT                       0x04
#define SHIFT_DISPLAY_RIGHT                        0x05
#define SHIFT_CURSOR_TO_RIGHT                      0x06
#define SHIFT_DISPLAY_LEFT                         0x07
#define DISPLAY_OFF_CURSOR_OFF                     0x08
#define DISPLAY_OFF_CURSOR_ON                      0x0A
#define DISPLAY_ON_CURSOR_OFF                      0x0C
#define DISPLAY_ON_CURSOR_ON                       0x0E
#define DISPLAY_ON_CURSOR_BLINKING                 0x0F
#define SHIFT_CURSOR_POSITION_TO_LEFT              0x10
#define SHIFT_CURSOR_POSITION_TO_RIGHT             0x14
#define SHIFT_ENTIRE_DISPLAY_TO_LEFT               0x18
#define SHIFT_ENTIRE_DISPLAY_TO_RIGHT              0x1C
#define SET_CURSOR_POSITION                        0x80
#define FORCE_CURSOR_BEGINNING_OF_SECOND_LINE      0xC0

/* Control Pins Setup */
#define LCD_RS_PORT                               PORTB_ID
#define LCD_RS_PIN                                PIN0_ID

#define LCD_E_PORT                                PORTB_ID
#define LCD_E_PIN                                 PIN2_ID

/* Data Pins Setup */
#if (LCD_BIT_MODE == 4)

#define LCD_DATA_PORT                             PORTA_ID

#define LCD_DB4_PIN_ID                            PIN3_ID
#define LCD_DB5_PIN_ID                            PIN4_ID
#define LCD_DB6_PIN_ID                            PIN5_ID
#define LCD_DB7_PIN_ID                            PIN6_ID

#elif (LCD_BIT_MODE == 8)

#define LCD_DATA_PORT                             PORTA_ID

#endif

/*******************************************************************************************
 *                                      Functions Prototypes                               *
 *******************************************************************************************/

/*
 * Description:
 * 1. Setup the LCD pins directions by GPIO Driver
 * 2. Setup the LCD Data Mode 4-bits or 8-bits.
 */
void LCD_Init(void);


/*
 * Description:
 * Send Command to LCD
 */
void LCD_SendCommand(uint8 Command);


/*
 * Description:
 * Display Character on LCD
 */
void LCD_DisplayCharacter(uint8 Data);

/*
 * Description:
 * Display String on LCD
 */
void LCD_DisplayString(const char *Str);

/*
 * Description:
 * Move cursor to the required position on LCD
 */
void LCD_MoveCursor(uint8 row,uint8 col);

/*
 * Description:
 * 1. Move cursor to the required position on LCD
 * 2. Display string from the required position determined by cursor
 */
void LCD_DisplayStringRowColumn(uint8 row, uint8 col, const char *Str);

/*
 * Description:
 * Clear string on LCD
 */
void LCD_ClearString(void);

/*
 * Description:
 * Display the required decimal value on the screen
 * Data variable type is "integer" because uint8 max value is only 255.
 */
void LCD_IntegerToString(int Data);

#endif /* LCD_H_ */
