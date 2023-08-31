/*************************************************************************************************************************
 * [File]: HMI_ECU.c
 * [Date]: 21/8/2023
 * [Objective]: Developing a system to unlock a door using a password - HMI ECU.
 * [Drivers]: GPIO - Timer1 - UART - Keypad - LCD
 * [Author]: Youssef Ahmed Zaki
 *************************************************************************************************************************/
#include <avr/io.h>
#include <util/delay.h>

/* MCAL Layer */
#include "GPIO.h"
#include "TIMER1.h"
#include "UART.h"

/* HAL Layer */
#include "Keypad.h"
#include "LCD.h"

#define HMI_READY               0x10
#define CONTROL_READY           0x20
#define PASSWORD_SIZE           5

#define PASSWORDS_UNMATCHED     0x30
#define PASSWORDS_MATCHED       0x40

#define PASS_RECEIVED           0x06
#define DISPLAY_ERROR           0x0C

#define OPEN_THE_DOOR           0x03
#define CHANGE_PASSWORD         0x04

/* HMI ECU Cases */
#define ENTER_PASSWORD          0x00
#define CONFIRM_PASSWORD        0x01
#define MAIN_OPTIONS_DISPLAY    0x02
#define MAIN_OPTION_SELECTION   0x03
#define OPENING_DOOR            0x04
#define PASSWORD_ERROR          0x05

/********************************************************************************************************
 *                                                                                                      *
 *                                             * Global Variables *                                     *
 *                                                                                                      *
 ********************************************************************************************************/

uint8 HMI_ECU_Sequence = 0;
uint8 Key_Pressed = 0;
uint8 G_Timer1_Count = 0;
uint8 Counter;
uint8 PassArr1_Send[PASSWORD_SIZE];
uint8 PassArr2_Send[PASSWORD_SIZE];

/********************************************************************************************************
 *                                                                                                      *
 *                                             * HMI ECU Functions *                                    *
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
 * Function is responsible for sending the entered password to Control ECU.
 */
void SendPassword(uint8 *pass_Ptr1)
{
	uint8 i;

	/* Coordinate data transmit with another ECU */
	UART_SendByte(HMI_READY);
	while (UART_ReceiveByte()!= CONTROL_READY);

	/* Send the password to Control_ECU number by number */
	for(i = 0; i < PASSWORD_SIZE; i++)
	{
		UART_SendByte(pass_Ptr1[i]);
	}

	/* wait until the byte received  Control_ECU */
	while(UART_ReceiveByte() != PASS_RECEIVED);
}

/*
 * Description:
 * This function is responsible for Entering the password of five integers.
 */
void EnterPassword(void)
{
	/* Clear anything on the LCD Screen */
	LCD_ClearString();

	/* Display this message to let the user enter the password */
	LCD_DisplayString("Plz Enter Pass: ");

	/* Move Cursor to the next row to write the password */
	LCD_MoveCursor(1,0);

	/* Let the user enter only 5 numbers & store the password in the array of integers */
	for (Counter = 0; Counter < PASSWORD_SIZE; Counter++)
	{
		Key_Pressed = KEYPAD_GetPressedKey();

		if (Key_Pressed >= 0 && Key_Pressed <= 9)
		{
			LCD_DisplayCharacter('*');
			PassArr1_Send[Counter] = Key_Pressed;
		}
		else
		{
			/*
			 * The counter decreases by one then increases by one to be the same to ensure that exactly five
			 * numbers are entered.
			 */
			Counter--;
		}
	}

	/* The application waits the user to enter '=' (Enter Key) to continue */
	while (Key_Pressed != '=')
	{
		/* Let the user press enter '=' to save the password */
		Key_Pressed = KEYPAD_GetPressedKey();
	}

	/* Send the First Password to Control_ECU */
	SendPassword(PassArr1_Send);
}

/*
 * Description:
 * This function is responsible for entering confirmed password and sending it to Control ECU.
 */
void ConfirmPassword(void)
{
	/* Clear anything on the LCD Screen */
	LCD_ClearString();

	/* Once the user entered the enter press '=', display the following on the LCD Screen */
	LCD_DisplayString("Plz re-enter the");
	LCD_MoveCursor(1,0);
	LCD_DisplayString("Same Pass: ");
	LCD_MoveCursor(1,11);

	/* Let the user enter only 5 numbers & store the password in the array of integers */
	for (Counter = 0; Counter < PASSWORD_SIZE; Counter++)
	{
		Key_Pressed = KEYPAD_GetPressedKey();

		if (Key_Pressed >= 0 && Key_Pressed <= 9)
		{
			LCD_DisplayCharacter('*');
			PassArr2_Send[Counter] = Key_Pressed;
		}
		else
		{
			/*
			 * if any key pressed other than [0 : 9] -> the counter decreases by one then increases by one to be the same
			 * to ensure that exactly five digits are entered.
			 */
			Counter--;
		}
	}

	/* The application waits the user to enter '=' (Enter Key) to continue */
	while (Key_Pressed != '=')
	{
		/* Let the user press enter '=' to save the password */
		Key_Pressed = KEYPAD_GetPressedKey();
	}

	/* Send the Second Password to Control_ECU */
	SendPassword(PassArr2_Send);
}

/*
 * Description:
 * This function is responsible for
 * 1. Displaying the main options (+ : Open Door or - : Change Pass)
 * 2. Let the user select one option.
 */
void MainOptions(void)
{
	/* Clear anything on the LCD Screen */
	LCD_ClearString();

	/* The Main Options should be displayed on the LCD Screen as following: */
	LCD_DisplayString("+ : Open Door ");
	LCD_MoveCursor(1,0);
	LCD_DisplayString("- : Change Pass ");

	/* if '+' or '-' buttons are not pressed, then nothing happens until any of them pressed by user */
	while ((Key_Pressed != '+') && (Key_Pressed != '-'))
	{
		/* Press '+' to open the door or Press '-' to change pass */
		Key_Pressed = KEYPAD_GetPressedKey();
	}
}

/********************************************************************************************************
 *                                                                                                      *
 *                                             * HMI Main Function *                                    *
 *                                                                                                      *
 ********************************************************************************************************/

int main(void)
{
	uint8 Result;
	uint8 i;

	/********************************************************************************************************
	 *                                                                                                      *
	 *                                         * Drivers Configurations *                                   *
	 *                                                                                                      *
	 ********************************************************************************************************/

	/*
	 * Timer1 Normal Mode Configuration:
	 * 1. TCNT1 = 0 -> Starting Value of Timer is Zero.
	 * 2. OCR1A = 24000 -> Every 24000 ticks, the timer counts "three seconds".
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

	/********************************************************************************************************
	 *                                                                                                      *
	 *                                           * Drivers Initialization *                                 *
	 *                                                                                                      *
	 ********************************************************************************************************/

	/* MCAL Drivers Initialization */
	UART_Init(&UART_Config);

	/* HAL Drivers initialization */
	LCD_Init();

	/* Activation of Global Interrupt enable bit (I-bit) to enable the interrupts */
	SREG |= (1<<7);

	/* Wait until Control_ECU is ready to receive the data */
	while(UART_ReceiveByte() != CONTROL_READY){}

	/********************************************************************************************************
	 *                                                                                                      *
	 *                                           * HMI Application Sequence *                               *
	 *                                                                                                      *
	 ********************************************************************************************************/

	while (1)
	{
		switch(HMI_ECU_Sequence)
		{

		/* Let the user enter the first password */
		case ENTER_PASSWORD:

			/* Start the Application by letting the user enter the password */
			EnterPassword();

			/* Jump to the next step */
			HMI_ECU_Sequence++;
			break;

			/* Let the user enter the confirmed password and save it if it matched with first one */
		case CONFIRM_PASSWORD:

			/* Let the user confirm the password */
			ConfirmPassword();

			/* Coordinate data transmit with another ECU */
			while (UART_ReceiveByte()!= CONTROL_READY);
			UART_SendByte(HMI_READY);

			if (UART_ReceiveByte() == PASSWORDS_MATCHED)
			{
				/* Clear anything on the LCD Screen */
				LCD_ClearString();

				/* Print that the passwords are matched */
				LCD_DisplayString("Done!");
				LCD_MoveCursor(1,0);
				LCD_DisplayString("Password Saved");
				_delay_ms(2000);

				/* Jump to the next step */
				HMI_ECU_Sequence++;
			}
			else
			{
				/* Clear anything on the LCD Screen */
				LCD_ClearString();

				/* Print that the passwords are not matched */
				LCD_DisplayString("ERROR! Passwords");
				LCD_MoveCursor(1,0);
				LCD_DisplayString("Not Matched");
				_delay_ms(2000);

				/* return to first step */
				HMI_ECU_Sequence = ENTER_PASSWORD;
			}
			break;

			/* Display the main options and let user choose between open the door or change password */
		case MAIN_OPTIONS_DISPLAY:

			/* Display Main Options */
			MainOptions();

			/* Coordinate data transmit with another ECU */
			UART_SendByte(HMI_READY);
			while (UART_ReceiveByte()!= CONTROL_READY);

			/* Send the pressed key whether is '+' or '-' to Control ECU */
			UART_SendByte(Key_Pressed);

			if (Key_Pressed == '+')
			{
				/* Let the user enter the password */
				EnterPassword();

				/* Coordinate data transmit with another ECU */
				while (UART_ReceiveByte()!= CONTROL_READY);
				UART_SendByte(HMI_READY);

				if (UART_ReceiveByte() == PASSWORDS_MATCHED)
				{
					/* Clear anything on the LCD Screen */
					LCD_ClearString();

					/* Print that the password are successfully entered */
					LCD_DisplayString("Successful!");
					_delay_ms(2000);

					/* Jump to Opening Door step */
					HMI_ECU_Sequence = OPENING_DOOR;
				}
				else
				{
					/* Clear anything on the LCD Screen */
					LCD_ClearString();

					/* Print that the passwords are not matched */
					LCD_DisplayString("ERROR!");
					LCD_MoveCursor(1,0);
					LCD_DisplayString("Wrong Password");
					_delay_ms(2000);

					/* increment of the wrong attempts */
					i++;

					/* return to main options display step */
					HMI_ECU_Sequence = MAIN_OPTIONS_DISPLAY;
				}
			}
			else
			{
				/* Let the user enter the password */
				EnterPassword();

				/* Coordinate data transmit with another ECU */
				while (UART_ReceiveByte()!= CONTROL_READY);
				UART_SendByte(HMI_READY);

				if (UART_ReceiveByte() == PASSWORDS_MATCHED)
				{
					/* Clear anything on the LCD Screen */
					LCD_ClearString();

					/* Print that the password are successfully entered */
					LCD_DisplayString("Successful!");
					_delay_ms(2000);

					/* Jump to Changing Password step */
					HMI_ECU_Sequence = ENTER_PASSWORD;
				}
				else
				{
					/* Clear anything on the LCD Screen */
					LCD_ClearString();

					/* Print that the passwords are not matched */
					LCD_DisplayString("ERROR!");
					LCD_MoveCursor(1,0);
					LCD_DisplayString("Wrong Password");
					_delay_ms(2000);

					/* increment of the wrong attempts */
					i++;

					/* return to main options display step */
					HMI_ECU_Sequence = MAIN_OPTIONS_DISPLAY;
				}
			}

			/* take an action if the wrong attempts reach 3 attempts */
			if (i == 3)
			{
				/* Jump to Password Error Step */
				HMI_ECU_Sequence = PASSWORD_ERROR;
				i = 0;
			}
			break;

			/* Display the messages of door locking/unlocking on lCD screen */
		case OPENING_DOOR:

			/* Coordinate data transmit with another ECU */
			UART_SendByte(HMI_READY);
			while (UART_ReceiveByte()!= CONTROL_READY);

			/* Send to Control ECU to open the door */
			UART_SendByte(OPEN_THE_DOOR);

			/* Start the timer */
			Timer1_NonPWm_Mode_Init(&Timer1_Config);
			Timer1_SetCallBack(Timer1_CallBack);

			/* Clear anything on the LCD Screen */
			LCD_ClearString();

			/* Print that the passwords are not matched */
			LCD_DisplayString("Door is");
			LCD_MoveCursor(1,0);
			LCD_DisplayString("Unlocking...");

			/* every tick = 3 seconds, so:
			 * 1. Opening the door time = 3*5 = 15 seconds
			 * 2. Holding the door time = 3*1 = 3 seconds
			 * Total time = 18 seconds which is G_Timer1_Count = 18/3 = 6 ticks */
			while (G_Timer1_Count < 6);

			/* Start count again from 0 */
			G_Timer1_Count = 0;

			/* Clear anything on the LCD Screen */
			LCD_ClearString();

			/* Print that the passwords are not matched */
			LCD_DisplayString("Door is");
			LCD_MoveCursor(1,0);
			LCD_DisplayString("Locking...");

			/* Wait another 15 seconds (3 seconds * 5 ticks) */
			while (G_Timer1_Count < 5);

			/* Stop the timer */
			G_Timer1_Count = 0;
			Timer1_DeInit();

			/* return to main options display step */
			HMI_ECU_Sequence = MAIN_OPTIONS_DISPLAY;
			break;

			/* Display message of Error on LCD screen if the user entered the password three failed attempts */
		case PASSWORD_ERROR:

			/* Coordinate data transmit with another ECU */
			while (UART_ReceiveByte()!= CONTROL_READY);
			UART_SendByte(HMI_READY);

			/* Wait until Control ECU Sends to display Error message on LCD Screen */
			while (UART_ReceiveByte()!= DISPLAY_ERROR);

			/* Start the timer */
			Timer1_NonPWm_Mode_Init(&Timer1_Config);
			Timer1_SetCallBack(Timer1_CallBack);

			/* Clear anything on the LCD Screen */
			LCD_ClearString();

			/* Print that the passwords are not matched */
			LCD_DisplayString("ERROR Happened!");
			LCD_MoveCursor(1,0);
			LCD_DisplayString("Try Again Later");

			/* wait one minute (60 seconds = 3 seconds * 20 ticks ) */
			while (G_Timer1_Count < 20);

			/* Stop the timer */
			G_Timer1_Count = 0;
			Timer1_DeInit();

			/* return to main options display step */
			HMI_ECU_Sequence = MAIN_OPTIONS_DISPLAY;
			break;

			/* if any thing entered except the sequence steps of application -> display error message */
		default:

			/* Clear anything on the LCD Screen */
			LCD_ClearString();

			/* Print that the passwords are not matched */
			LCD_DisplayString("ERROR Happened!");
			LCD_MoveCursor(1,0);
			LCD_DisplayString("Try Again Later");
			while (1);
		}
	}
}
