/*******************************************************************************************************************
 * File Name: GPIO.h
 * Date: 25/6/2023
 * Driver: ATmega32 GPIO Driver Header File
 * Author: Youssef Zaki
 ******************************************************************************************************************/
#include "Standard_Types.h"

#ifndef GPIO_H_
#define GPIO_H_

/****************************************************************************************
 *                                    Macros Definitions                                *
 ****************************************************************************************/

/* Define number of pins and ports at AVR ATmega32 */
#define NUM_OF_PORTS          4
#define NUM_OF_PINS_PER_PORT  8

/* Define of the ATmeg32 PORTs with respect to their ID */
#define PORTA_ID                                   0
#define PORTB_ID                                   1
#define PORTC_ID                                   2
#define PORTD_ID                                   3

/* Define of the ATmega32 PINs with respect to their ID */
#define PIN0_ID                                    0
#define PIN1_ID                                    1
#define PIN2_ID                                    2
#define PIN3_ID                                    3
#define PIN4_ID                                    4
#define PIN5_ID                                    5
#define PIN6_ID                                    6
#define PIN7_ID                                    7

/****************************************************************************************
 *                                      Types Declaration                               *
 ****************************************************************************************/
typedef enum
{
	INPUT_PIN, OUTPUT_PIN
}GPIO_PinDirectionType;

typedef enum
{
	INPUT_PORT, OUTPUT_PORT = 0b11111111
}GPIO_PortDirectionType;
/****************************************************************************************
 *                                      Functions Prototypes                            *
 ****************************************************************************************/

/*
 * Description:
 * Setup the direction of the PIN if it is INPUT (LOGIC LOW) or it is OUTPUT (LOGIC HIGH)
 * If the PORT or PIN numbers are not correct, the function will not handle the request
 */
void GPIO_SetupPinDirection(uint8 Port_num, uint8 Pin_num, uint8 GPIO_PinDirectionType);

/*
 * Description:
 * write the value LOGIC HIGH or LOGIC LOW on the required PIN in the required PORT
 * If the PORT or PIN numbers are not correct, the function will not handle the request
 * If the PIN is INPUT PIN, so the PIN will enable/disable the internal pull-up resistor
 */
void GPIO_WritePin(uint8 Port_num, uint8 Pin_num, uint8 value);

/*
 * Description:
 * read the value LOGIC HIGH or LOGIC LOW on the required PIN in the required PORT
 * If the PORT or PIN numbers are not correct, the function will return LOGIC LOW
 * The function will return LOGIC HIGH or LOGIC LOW
 */
uint8 GPIO_ReadPin(uint8 Port_num, uint8 Pin_num);

/*
 * Description:
 * Setup the direction of the PORT if it is INPUT (LOGIC LOW) or it is OUTPUT (LOGIC HIGH)
 * If the PORT number is not correct, the function will not handle the request
 */
void GPIO_SetupPortDirection(uint8 Port_num, GPIO_PortDirectionType dircetion);

/*
 * Description:
 * write the value on the required PORT
 * If the PORT number is not correct, the function will not handle the request
 */
void GPIO_WritePORT(uint8 Port_num, uint8 value);

/*
 * Description:
 * read the value on the required PORT
 * If the PORT number is not correct, the function will return ZERO
 * The function will return the read value on the PORT
 */
uint8 GPIO_ReadPORT(uint8 Port_num);

#endif /* GPIO_H_ */
