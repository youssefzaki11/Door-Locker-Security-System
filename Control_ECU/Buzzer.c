/****************************************************************************************************************
 * File Name: Buzzer.c
 * Date: 21/8/2023
 * Driver: Buzzer Driver Source File
 * Author: Youssef Zaki
 ****************************************************************************************************************/
#include "GPIO.h"
#include "Buzzer.h"

/****************************************************************************************
 *                                     Functions Definitions                            *
 ****************************************************************************************/

/*
 * Description:
 * 1. Setup the direction for the buzzer pin as output pin through the GPIO driver.
 * 2. Turn off the buzzer through the GPIO.
 */
void Buzzer_Init(void)
{
	GPIO_SetupPinDirection(BUZZER_PORT_ID, BUZZER_PIN_ID, OUTPUT_PIN);
	GPIO_WritePin(BUZZER_PORT_ID, BUZZER_PIN_ID, LOGIC_LOW);
}

/*
 * Description:
 * Function to enable the Buzzer through the GPIO.
 */
void Buzzer_ON(void)
{
	GPIO_WritePin(BUZZER_PORT_ID, BUZZER_PIN_ID, LOGIC_HIGH);
}

/*
 * Description:
 * Function to disable the Buzzer through the GPIO.
 */
void Buzzer_OFF(void)
{
	GPIO_WritePin(BUZZER_PORT_ID, BUZZER_PIN_ID, LOGIC_LOW);
}
