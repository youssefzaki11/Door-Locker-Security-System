/*****************************************************************************************************************
 * File Name: DC_Motor.c
 * Date: 4/8/2023
 * Driver: DC Motor Driver Source File
 * Author: Youssef Zaki
 ****************************************************************************************************************/
#include "GPIO.h"
#include "DC_Motor.h"
#include "TIMER0.h"

/*
 * DESCRIPTION:
 * The Function responsible for setup the direction for the two motor pins through the GPIO driver.
 * Stop at the DC-Motor at the beginning through the GPIO driver.
 */
void DcMotor_Init(void)
{
	/* Let MCU Pins connected to IN1 & IN2 as output pins */
	GPIO_SetupPinDirection(DC_MOTOR_IN1_PORT_ID,DC_MOTOR_IN1_PIN_ID,OUTPUT_PIN);
	GPIO_SetupPinDirection(DC_MOTOR_IN2_PORT_ID,DC_MOTOR_IN2_PIN_ID,OUTPUT_PIN);

	 /* Stop the Motor at the beginning */
	GPIO_WritePin(DC_MOTOR_IN1_PORT_ID,DC_MOTOR_IN1_PIN_ID,LOGIC_LOW);
	GPIO_WritePin(DC_MOTOR_IN2_PORT_ID,DC_MOTOR_IN2_PIN_ID,LOGIC_LOW);
}

/*
 * DESCRIPTION:
 * The function responsible for rotate the DC Motor CW/ or A-CW or stop the motor based on the state
 * input state value.
 * Send the required duty cycle to the PWM driver based on the required speed value.
 */
void DcMotor_Rotate(DcMotor_State state, uint8 speed)
{
	if (state == STOP)
	{
		/* STOP MODE: A = LOW, B = LOW */
		GPIO_WritePin(DC_MOTOR_IN1_PORT_ID,DC_MOTOR_IN1_PIN_ID,LOGIC_LOW);
		GPIO_WritePin(DC_MOTOR_IN1_PORT_ID,DC_MOTOR_IN2_PIN_ID,LOGIC_LOW);
	}
	else if (state == CW)
	{
		/* CLOCk WISE MODE: A = LOW, B = HIGH */
		GPIO_WritePin(DC_MOTOR_IN1_PORT_ID,DC_MOTOR_IN1_PIN_ID,LOGIC_LOW);
		GPIO_WritePin(DC_MOTOR_IN1_PORT_ID,DC_MOTOR_IN2_PIN_ID,LOGIC_HIGH);
	}
	else if (state == A_CW)
	{
		/* Anti-CLOCk WISE MODE: A = HIGH, B = LOW */
		GPIO_WritePin(DC_MOTOR_IN1_PORT_ID,DC_MOTOR_IN1_PIN_ID,LOGIC_HIGH);
		GPIO_WritePin(DC_MOTOR_IN1_PORT_ID,DC_MOTOR_IN2_PIN_ID,LOGIC_LOW);
	}

	/* Pass the Speed of the Motor to PWM Function to calculate duty cycle and hence Timer0 Compare  Value */
	TIMER0_PWM_Start(speed);

}
