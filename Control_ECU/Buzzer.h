/****************************************************************************************************************
 * File Name: Buzzer.h
 * Date: 21/8/2023
 * Driver: Buzzer Driver Header File
 * Author: Youssef Zaki
 ****************************************************************************************************************/
#include "Standard_Types.h"

#ifndef BUZZER_H_
#define BUZZER_H_

/******************************************************************************************
 *                                    Macros Definitions                                  *
 ******************************************************************************************/
#define BUZZER_PORT_ID                       PORTA_ID
#define BUZZER_PIN_ID                        PIN0_ID

/*******************************************************************************************
 *                                      Functions Prototypes                               *
 *******************************************************************************************/

/*
 * Description:
 * 1. Setup the direction for the buzzer pin as output pin through the GPIO driver.
 * 2. Turn off the buzzer through the GPIO.
 */
void Buzzer_Init(void);

/*
 * Description:
 * Function to enable the Buzzer through the GPIO.
 */
void Buzzer_ON(void);

/*
 * Description:
 * Function to disable the Buzzer through the GPIO.
 */
void Buzzer_OFF(void);

#endif /* BUZZER_H_ */
