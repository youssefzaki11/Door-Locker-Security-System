/****************************************************************************************************************
 * File Name: DC_Motor.h
 * Date: 4/8/2023
 * Driver: DC Motor Driver Header File
 * Author: Youssef Zaki
 ****************************************************************************************************************/
#include "Standard_Types.h"

#ifndef DC_MOTOR_H_
#define DC_MOTOR_H_

/******************************************************************************************
 *                                    Macros Definitions                                  *
 ******************************************************************************************/
#define DC_MOTOR_IN1_PORT_ID                 PORTB_ID
#define DC_MOTOR_IN1_PIN_ID                  PIN0_ID

#define DC_MOTOR_IN2_PORT_ID                 PORTB_ID
#define DC_MOTOR_IN2_PIN_ID                  PIN1_ID

#define DC_MOTOR_EN1_PORT_ID                 PORTB_ID
#define DC_MOTOR_EN1_PIN_ID                  PIN2_ID

#define DC_MOTOR_MAX_SPEED                   100

/******************************************************************************************
 *                                     Types Declaration                                  *
 ******************************************************************************************/

typedef enum {
	STOP,CW,A_CW
}DcMotor_State;

/******************************************************************************************
 *                                      Functions Prototypes                              *
 ******************************************************************************************/

/*
 * DESCRIPTION:
 * The Function responsible for setup the direction for the two motor pins through the GPIO driver.
 * Stop at the DC-Motor at the beginning through the GPIO driver.
 */
void DcMotor_Init(void);

/*
 * DESCRIPTION:
 * The function responsible for rotate the DC Motor CW/ or A-CW or stop the motor based on the state input
 * state value.
 * Send the required duty cycle to the PWM driver based on the required speed value.
 */
void DcMotor_Rotate(DcMotor_State state, uint8 speed);

#endif /* DC_MOTOR_H_ */
