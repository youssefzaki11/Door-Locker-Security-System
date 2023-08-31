/*************************************************************************************************************************
 * [File]: Control_ECU.c
 * [Date]: 21/8/2023
 * [Objective]: Developing a system to unlock a door using a password - Control ECU.
 * [Drivers]: GPIO - Timer0 - Timer1 - UART - I2C - DC_Motor - External EEPROM - Buzzer
 * [Author]: Youssef Ahmed Zaki
 *************************************************************************************************************************/
#include <avr/io.h>
#include <util/delay.h>

/* MCAL Layer */
#include "GPIO.h"
#include "TIMER0.h"
#include "TIMER1.h"
#include "UART.h"
#include "I2C.h"

/* HAL Layer */
#include "Buzzer.h"
#include "EEPROM.h"
#include "DC_Motor.h"

#define HMI_READY                              0x10
#define CONTROL_READY                          0x20
#define PASSWORD_SIZE                          5

#define PASSWORDS_UNMATCHED                    0x30
#define PASSWORDS_MATCHED                      0x40

#define PASS_RECEIVED                          0x06
#define DISPLAY_ERROR                          0x0C

#define OPEN_THE_DOOR                          0x03
#define CHANGE_PASSWORD                        0x04

/* Control ECU Cases */
#define RECEIVE_FIRST_PASSWORD                 0x00
#define RECEIVE_AND_CHECK_CONFIRMED_PASSWORD   0x01
#define RECEIVING_MAIN_OPTION                  0x02
#define OPEN_THE_DOOR                          0x03
#define PASSWORD_ERROR                         0x04

/********************************************************************************************************
 *                                                                                                      *
 *                                             * Global Variables *                                     *
 *                                                                                                      *
 ********************************************************************************************************/

uint8 Control_ECU_Sequence = 0;
uint8 *G_Pass1[PASSWORD_SIZE];
uint8 *G_Pass2[PASSWORD_SIZE];
uint8 Counter;
uint8 G_Timer1_Count = 0;

/********************************************************************************************************
 *                                                                                                      *
 *                                           * Control ECU Functions *                                  *
 *                                                                                                      *
 ********************************************************************************************************/

/*
 * Description:
 * Function is responsible to be called when Timer1 interrupt occurs to increment the ticks by one.
 */
void Timer1_CallBack(void)
{
	/* Every three seconds, it increments by one */
	G_Timer1_Count++;
}

/*
 * Description:
 * Function is responsible for receiving the password from HMI ECU.
 */
void ReceivePassword(uint8 *pass)
{
	uint8 i;

	/* Coordinate data transmit with another ECU */
	while (UART_ReceiveByte()!= HMI_READY);
	UART_SendByte(CONTROL_READY);

	/* Receiving the password from the user */
	for(i = 0; i < PASSWORD_SIZE; i++)
	{
		pass[i] = UART_ReceiveByte();
	}

	/* Send to HMI ECU that password is successfully received */
	UART_SendByte(PASS_RECEIVED);
}

/*
 * Description:
 * Function is responsible for checking the entered password with the one save in External EEPROM.
 */
uint8 CheckPassword(uint8 *Pass1_Receive, uint8 *Pass2_Receive)
{
	uint8 i;

	/* Read the EEPROM saved password */
	for (i = 0; i < PASSWORD_SIZE; i++)
	{
		EEPROM_ReadByte((0x0000 + i), &Pass2_Receive[i]);
		_delay_ms(10);
	}

	/* Compare entered password with EEPROM saved password */
	for (i = 0; i < PASSWORD_SIZE; i++)
	{
		if (Pass1_Receive[i] != Pass2_Receive[i])
		{
			/* Send to HMI ECU that two passwords are un-matched */
			return PASSWORDS_UNMATCHED;
		}
	}
	return PASSWORDS_MATCHED;
}

/*
 * Description:
 * Function is responsible for comparing entered password and confirmed password.
 */
uint8 ComparePasswords(uint8 *Pass1_Receive, uint8 *Pass2_Receive)
{
	uint8 i;

	/* Receive the Confirmed Password */
	for (i = 0; i < PASSWORD_SIZE; i++)
	{
		if (Pass1_Receive[i] != Pass2_Receive[i])
		{
			/* Send to HMI ECU that two passwords are un-matched */
			return PASSWORDS_UNMATCHED;
		}
	}
	/* Send to HMI ECU that two passwords are matched */
	return PASSWORDS_MATCHED;
}

/*
 * Description:
 * Function is responsible for saving the password if the entered and confirmed password are matched.
 */
void SavePassword(uint8 *Pass_Receive)
{
	uint8 i;

	/* Store the Password in EEPROM */
	for (i = 0; i < PASSWORD_SIZE; i++)
	{
		EEPROM_WriteByte((0x0000 + i), Pass_Receive[i]);
		_delay_ms(10);
	}
}

int main(void)
{
	uint8 Check;
	uint8 Pass_Check;
	uint8 Result;
	uint8 i = 0;

	/*********************************************************************************************************
	 *                                                                                                       *
	 *                                        * Drivers Configurations *                                     *
	 *                                                                                                       *
	 *********************************************************************************************************/

	/*
	 * Timer0 PWM Mode Configuration:
	 * 1. TCNT0 = 0 -> Starting Value of Timer is Zero.
	 * 2. OCR0 = 0 -> It is based on the Duty_Cycle of the PWM Signal.
	 * 3. Pre-scalar = F_CPU/8 -> To control DC motor using 500Hz PWM Signal.
	 * 4. Timer0 Mode -> Fast PWM Mode.
	 */
	Timer0_ConfigType Timer0_Config = {0, 0, TIMER0_Prescaler_8, TIMER0_Fast_PWM_3};

	/*
	 * Timer1 Normal Mode Configuration:
	 * 1. TCNT1 = 0 -> Starting Value of Timer is Zero.
	 * 2. OCR1A = 24000 -> Every 24000 ticks, the timer counts three seconds.
	 * 3. Pre-scalar = F_CPU/1024 -> (Tick time = 0.125 msec, so (Timer Time / Tick Time = No. of ticks)
	 * 4. Timer1 Mode -> CTC Mode.
	 */
	Timer1_ConfigType Timer1_Config = {0, 24000, TIMER1_Prescaler_1024, TIMER1_CTC_4};

	/*
	 * UART Configuration:
	 * 1. UART Mode -> Asynchronous Mode.
	 * 2. Data Transmission Speed Mode -> Double Speed Mode.
	 * 3. Parity Bit State -> Disabled.
	 * 4. Data Size in Bits -> Eight bits.
	 * 5. Baud Rate -> 9600.
	 */
	UART_ConfigType UART_Config = {Asynchronous, Double_Speed, Disabled, 0, Eight_Bit_3, 9600};

	/*
	 * Description:
	 * 1. Set 0x01 as a device address in case of device to be a slave device.
	 * 2. let Bit Rate: 400.000 kbps by giving the TWBR register value = 2 according to I2C Bit rate formula.
	 * 3. Set the pre-scaler to be one = F_CPU
	 */
	TWI_ConfigType TWI_Config = {1, 2, Prescaler_1};

	/*********************************************************************************************************
	 *                                                                                                       *
	 *                                           * Drivers Initialization *                                  *
	 *                                                                                                       *
	 *********************************************************************************************************/

	/* MCAL Drivers Initialization */
	UART_Init(&UART_Config);
	TWI_Init(&TWI_Config);

	/* HAL Drivers initialization */
	Buzzer_Init();
	DcMotor_Init();

	/* Global Interrupt Enable bit (I-bit) Activation to activate the all interrupts */
	SREG |= (1<<7);

	/* Send this byte to HMI_ECU to let the HMI ECU sends the password */
	UART_SendByte(CONTROL_READY);

	/*********************************************************************************************************
	 *                                                                                                       *
	 *                                            * Control Application Sequence *                           *
	 *                                                                                                       *
	 *********************************************************************************************************/

	while (1)
	{
		switch(Control_ECU_Sequence)
		{

		/* Receiving the first password from the user */
		case RECEIVE_FIRST_PASSWORD:

			/* receive the first password from HMI ECU */
			ReceivePassword(G_Pass1);

			/* Jump to the next step */
			Control_ECU_Sequence++;
			break;

			/* Receiving the second password from the user and check if it is matched with the first one */
		case RECEIVE_AND_CHECK_CONFIRMED_PASSWORD:

			/* receive the confirmed password from HMI ECU */
			ReceivePassword(G_Pass2);

			/* Check if the two password are matched or not */
			Check = ComparePasswords(G_Pass1, G_Pass2);

			/* Coordinate data transmit with another ECU */
			UART_SendByte(CONTROL_READY);
			while (UART_ReceiveByte()!= HMI_READY);

			if (Check == PASSWORDS_MATCHED)
			{
				/* Send to HMI ECU that passwords are matched */
				UART_SendByte(PASSWORDS_MATCHED);

				/* Save Password in the External EEPROM */
				SavePassword(G_Pass1);

				/* Jump to the next step */
				Control_ECU_Sequence++;
			}
			else
			{
				/* Send to HMI ECU that passwords are un-matched */
				UART_SendByte(PASSWORDS_UNMATCHED);

				/* return to first step */
				Control_ECU_Sequence = RECEIVE_FIRST_PASSWORD;
			}
			break;

			/* Receiving the user selection from the main options and take action according to this selection */
		case RECEIVING_MAIN_OPTION:

			/* Coordinate data transmit with another ECU */
			while (UART_ReceiveByte()!= HMI_READY);
			UART_SendByte(CONTROL_READY);

			/* Receive the result of option selection by user (+ : Open the Door or - : Change Password) */
			Result = UART_ReceiveByte();

			/* Check if the password matched or not and send the result to HMI ECU */
			if ( Result == '+')
			{
				/* receive the first password from HMI ECU */
				ReceivePassword(G_Pass1);

				/* Check the received password if it is matched with one saved in the External EEPROM */
				Pass_Check = CheckPassword(G_Pass1, G_Pass2);

				/* Coordinate data transmit with another ECU */
				UART_SendByte(CONTROL_READY);
				while (UART_ReceiveByte()!= HMI_READY);

				if (Pass_Check == PASSWORDS_MATCHED)
				{
					/* Send to HMI ECU that passwords are matched */
					UART_SendByte(PASSWORDS_MATCHED);

					/* Jump to open the door step */
					Control_ECU_Sequence = OPEN_THE_DOOR;
				}
				else
				{
					/* Send to HMI ECU that passwords are un-matched */
					UART_SendByte(PASSWORDS_UNMATCHED);

					/* increment of the wrong attempts */
					i++;

					/* Stay at this step to take the password again from the user */
					Control_ECU_Sequence = RECEIVING_MAIN_OPTION;
				}
			}
			else
			{
				/* receive the first password from HMI ECU */
				ReceivePassword(G_Pass1);

				/* Check the received password if it is matched with one saved in EEPROM */
				Pass_Check = CheckPassword(G_Pass1, G_Pass2);

				/* Coordinate data transmit with another ECU */
				UART_SendByte(CONTROL_READY);
				while (UART_ReceiveByte()!= HMI_READY);

				if (Pass_Check == PASSWORDS_MATCHED)
				{
					/* Send to HMI ECU that passwords are matched */
					UART_SendByte(PASSWORDS_MATCHED);

					/* Jump to Change Password step */
					Control_ECU_Sequence = RECEIVE_FIRST_PASSWORD;
				}
				else
				{
					/* Send to HMI ECU that passwords are un-matched */
					UART_SendByte(PASSWORDS_UNMATCHED);

					/* increment of the wrong attempts */
					i++;

					/* Stay at this step to take the password again from the user */
					Control_ECU_Sequence = RECEIVING_MAIN_OPTION;
				}
			}

			/* take an action if the wrong attempts reach 3 attempts */
			if (i == 3)
			{
				/* Jump to Password Error Step */
				Control_ECU_Sequence = PASSWORD_ERROR;
				i = 0;
			}
			break;

			/* Open the door by rotating the motor cw for 15 seconds then A_cw for 15 seconds to open/close the door */
		case OPEN_THE_DOOR:

			/* Coordinate data transmit with another ECU */
			while (UART_ReceiveByte()!= HMI_READY);
			UART_SendByte(CONTROL_READY);

			/* Wait until HMI ECU sends Open the Door */
			while (UART_ReceiveByte() != OPEN_THE_DOOR);

			/* Start Timers */
			Timer0_PWM_Mode_Init(&Timer0_Config);
			Timer1_NonPWm_Mode_Init(&Timer1_Config);
			Timer1_SetCallBack(Timer1_CallBack);

			/* Open the Door by rotating the DC Motor Clockwise at max speed */
			DcMotor_Rotate(CW, 100);

			/* Wait 15 seconds until the door is open (3 seconds * 5 ticks )*/
			while (G_Timer1_Count < 5);

			/* Start from zero again */
			G_Timer1_Count = 0;

			/* Stop rotating the motor after opening the door */
			DcMotor_Rotate(STOP, 0);

			/* Wait three seconds (holding the door)*/
			while (G_Timer1_Count < 1);

			/* Start from zero again */
			G_Timer1_Count = 0;

			/* Close the Door by rotating the DC Motor Anti-Clockwise at max speed */
			DcMotor_Rotate(A_CW, 100);

			/* Wait 15 seconds until door is closed (3 seconds * 5 ticks ) */
			while (G_Timer1_Count < 5);

			/* Stop rotating the motor after closing the door */
			DcMotor_Rotate(STOP, 0);

			/* Stop the timers */
			G_Timer1_Count = 0;
			Timer1_DeInit();
			Timer0_DeInit();

			/* return to receiving the main option from user */
			Control_ECU_Sequence = RECEIVING_MAIN_OPTION;
			break;

		case PASSWORD_ERROR:

			/* Coordinate data transmit with another ECU */
			UART_SendByte(CONTROL_READY);
			while (UART_ReceiveByte()!= HMI_READY);

			/* Send to HMI ECU to display error on LCD Screen */
			UART_SendByte(DISPLAY_ERROR);

			/* Start the timer */
			Timer1_NonPWm_Mode_Init(&Timer1_Config);
			Timer1_SetCallBack(Timer1_CallBack);

			/* Turn on the buzzer when three failed attempts of password are entered */
			Buzzer_ON();

			/* wait one minute (60 seconds = 3 seconds * 20 ticks ) */
			while (G_Timer1_Count < 20);

			/* Turn off the buzzer */
			Buzzer_OFF();

			/* Stop the timer */
			G_Timer1_Count = 0;
			Timer1_DeInit();

			/* return to receiving the main option from user */
			Control_ECU_Sequence = RECEIVING_MAIN_OPTION;
			break;
		}
	}
}
