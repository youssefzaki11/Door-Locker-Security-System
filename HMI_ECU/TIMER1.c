/*******************************************************************************************************************
 * File Name: TIMER1.c
 * Date: 17/7/2023
 * Driver: ATmega32 TIMER1 Driver Source File
 * Author: Youssef Zaki
 ******************************************************************************************************************/
#include "TIMER1.h"
#include "Common_Macros.h"
#include "GPIO.h"
#include <avr/io.h>
#include <avr/interrupt.h>

/***************************************************************************************
 *                                         Global Variables                            *
 ***************************************************************************************/
/* Global variables to hold the address of the call back function in the application */
static volatile void (*g_callBackPtr)(void) = NULL_PTR;

/*******************************************************************************
 *                       Interrupt Service Routines                            *
 *******************************************************************************/
/* Interrupt for Normal (Overflow) Mode */
ISR(TIMER1_OVF_vect)
{
	if(g_callBackPtr != NULL_PTR)
	{
		/* Call the Call Back function in the application after the edge is detected */
		(*g_callBackPtr)();
	}
}
/* Interrupt for Compare Mode */
ISR(TIMER1_COMPA_vect)
{
	if(g_callBackPtr != NULL_PTR)
	{
		/* Call the Call Back function in the application after the edge is detected */
		(*g_callBackPtr)();
	}
}

/****************************************************************************************
 *                                      Functions Definitions                           *
 ****************************************************************************************/

/*
 * Description:
 * Initialization of Timer1 (Enable Timer1)
 * 1. Let the TCNT1 Register = The Start value of the timer1.
 * 2. Enable FOC1A bit in the TCCR1A Register.
 * 3. Configure TCCR1A & TCCR1B according to the required pre-scalar.
 * 4. Configure the TCCR1B Register according to the Timer1 Mode.
 * 5. In CTC Mode Let OCR1A = the compare value (TOP Value).
 * 6. Set the OCIE1A in TIMSK Register to enable the interrupt of timer1 mode A.
 */
void Timer1_NonPWm_Mode_Init(const Timer1_ConfigType * Config_Ptr)
{

	TCNT1 = Config_Ptr -> initial_value;
	TCCR1A = (1<<FOC1A) | (1<<FOC1B) ;

	/* Set to required pre-scalar Configuration*/
	TCCR1B = (TCCR1B & 0xF8) | (Config_Ptr -> prescaler);

	if (Config_Ptr -> mode == TIMER1_Normal_0)
	{
		/* Configuration for Normal Mode:
		 * COM1A1 = 0, COM1A0 = 0, COM1B1 = 0, COM1B0 = 0
		 */
		TCCR1B = (TCCR1B & 0xF7);
	}
	else if (Config_Ptr -> mode == TIMER1_CTC_4)
	{
		OCR1A = Config_Ptr -> compare_value;
		/*
		 * Configuration for CTC Mode:
		 * WGM10 = 0, WGM11 = 0, WGM12 = 1
		 */
		TCCR1B = (TCCR1B & 0xF7) | (1 << WGM12);
	}

	/* Enable Timer1 Interrupt for mode A */
	TIMSK = (1<<OCIE1A);
}

/*
 * Description:
 * The function responsible for trigger the Timer1 with the PWM Mode.
 * 1. Let the TCNT1A Register = The Start value of the timer.
 * 2. Disable FOC1A & FOC1B bits in the TCCR1A Register (FOC1A = 0, FOC1B = 0)
 * 3. Enable CS12:0 bits according to the required pre-scalar.
 * 4. Setup the direction for OC1A as output pin through the GPIO driver.
 * 5. Configuration the PWM Mode (Phase Correct or Fast)
 * 6. Setup the PWM mode with Non-Inverting.
 */
void Timer1_PWM_Mode_Init(const Timer1_ConfigType * Config_Ptr)
{

	TCNT1 = Config_Ptr -> initial_value;
	CLEAR_BIT(TCCR1A, FOC1A);
	CLEAR_BIT(TCCR1A, FOC1B);

	/* Set to required pre-scalar Configuration*/
	TCCR1B = (TCCR1B & 0xF8) | (Config_Ptr -> prescaler);

	/* Configure the OC1A pin as Output Pin */
	GPIO_SetupPinDirection(PORTD_ID, PIN4_ID, OUTPUT_PIN);

	/* Preparing TCCR1A & TCCR1B Registers to configuration according to required Mode */
	TCCR1A &= 0x3C;      /* TCCR1A & 0011 1100 */
	TCCR1B &= 0xE7;      /* TCCR1B & 1110 0111 */

	switch(Config_Ptr -> mode)
	{
	case TIMER1_PWM_Phase_Correct_8_Bit_1:
		/* Configuration for Phase Correct PWM 8-bit Mode:
		 * WGM10 = 1, WGM11 = 0, WGM12 = 0, WGM13 = 0, COM1A1 = 1, COM1A0 = 0
		 */
		SET_BIT(TCCR1A, WGM10);
		SET_BIT(TCCR1A, COM1A1);
		break;

	case TIMER1_PWM_Phase_Correct_9_Bit_2:
		/* Configuration for Phase Correct PWM 9-bit Mode:
		 * WGM10 = 0, WGM11 = 1, WGM12 = 0, WGM13 = 0, COM1A1 = 1, COM1A0 = 0
		 */
		SET_BIT(TCCR1A, WGM11);
		SET_BIT(TCCR1A, COM1A1);
		break;

	case TIMER1_PWM_Phase_Correct_10_Bit_3:
		/* Configuration for Phase Correct PWM 10-bit Mode:
		 * WGM10 = 1, WGM11 = 1, WGM12 = 0, WGM13 = 0, COM1A1 = 1, COM1A0 = 0
		 */
		SET_BIT(TCCR1A, WGM10);
		SET_BIT(TCCR1A, WGM11);
		SET_BIT(TCCR1A, COM1A1);
		break;

	case TIMER1_Fast_Pwm_8_Bit_5:
		/* Configuration for Fast PWM 8-bit Mode:
		 * WGM10 = 1, WGM11 = 0, WGM12 = 1, WGM13 = 0, COM1A1 = 1, COM1A0 = 0
		 */
		SET_BIT(TCCR1A, WGM10);
		SET_BIT(TCCR1B, WGM12);
		SET_BIT(TCCR1A, COM1A1);
		break;

	case TIMER1_Fast_Pwm_9_Bit_6:
		/* Configuration for Fast PWM 9-bit Mode:
		 * WGM10 = 0, WGM11 = 1, WGM12 = 1, WGM13 = 0, COM1A1 = 1, COM1A0 = 0
		 */
		SET_BIT(TCCR1A, WGM11);
		SET_BIT(TCCR1B, WGM12);
		SET_BIT(TCCR1A, COM1A1);
		break;

	case TIMER1_Fast_Pwm_10_Bit_7:
		/* Configuration for Fast PWM 10-bit Mode:
		 * WGM10 = 1, WGM11 = 1, WGM12 = 1, WGM13 = 0, COM1A1 = 1, COM1A0 = 0
		 */
		SET_BIT(TCCR1A, WGM10);
		SET_BIT(TCCR1A, WGM11);
		SET_BIT(TCCR1B, WGM12);
		SET_BIT(TCCR1A, COM1A1);
		break;

	case TIMER1_PWM_Phase_and_Frequency_Correct_8:
		/* Configuration for Phase Correct and Frequency PWM (ICR1) Mode:
		 * WGM10 = 0, WGM11 = 0, WGM12 = 0, WGM13 = 1, COM1A1 = 1, COM1A0 = 0
		 */
		SET_BIT(TCCR1B, WGM13);
		SET_BIT(TCCR1A, COM1A1);
		break;

	case TIMER1_PWM_Phase_and_Frequency_Correct_9:
		/* Configuration for Phase Correct and Frequency PWM (OCR1A) Mode:
		 * WGM10 = 1, WGM11 = 0, WGM12 = 0, WGM13 = 1, COM1A1 = 1, COM1A0 = 0
		 */
		SET_BIT(TCCR1A, WGM10);
		SET_BIT(TCCR1B, WGM13);
		SET_BIT(TCCR1A, COM1A1);
		break;

	case TIMER1_PWM_Phase_Correct_10:
		/* Configuration for Phase Correct PWM (ICR1) Mode:
		 * WGM10 = 0, WGM11 = 1, WGM12 = 0, WGM13 = 1, COM1A1 = 1, COM1A0 = 0
		 */
		SET_BIT(TCCR1A, WGM11);
		SET_BIT(TCCR1B, WGM13);
		SET_BIT(TCCR1A, COM1A1);
		break;

	case TIMER1_PWM_Phase_Correct_11:
		/* Configuration for Phase Correct PWM (OCR1A) Mode:
		 * WGM10 = 1, WGM11 = 1, WGM12 = 0, WGM13 = 1, COM1A1 = 1, COM1A0 = 0
		 */
		SET_BIT(TCCR1A, WGM10);
		SET_BIT(TCCR1A, WGM11);
		SET_BIT(TCCR1B, WGM13);
		SET_BIT(TCCR1A, COM1A1);
		break;

	case TIMER1_Fast_PWM_14:
		/* Configuration for Fast PWM (ICR1) Mode:
		 * WGM10 = 0, WGM11 = 1, WGM12 = 1, WGM13 = 1, COM1A1 = 1, COM1A0 = 0
		 */
		SET_BIT(TCCR1A, WGM11);
		SET_BIT(TCCR1B, WGM12);
		SET_BIT(TCCR1B, WGM13);
		SET_BIT(TCCR1A, COM1A1);
		break;

	case TIMER1_Fast_PWM_15:
		/* Configuration for Fast PWM (OCR1A) Mode:
		 * WGM10 = 1, WGM11 = 1, WGM12 = 1, WGM13 = 1, COM1A1 = 1, COM1A0 = 0
		 */
		SET_BIT(TCCR1A, WGM10);
		SET_BIT(TCCR1A, WGM11);
		SET_BIT(TCCR1B, WGM12);
		SET_BIT(TCCR1B, WGM13);
		SET_BIT(TCCR1A, COM1A1);
		break;
	}
}

/*
 * Description:
 * Function to
 * 1. calculate the OCR1A Value from required Duty Cycle.
 * 2. Put Compare and TOP values of (ICR1 or OCR1A) registers.
 */
void TIMER1_PWM_Start (uint16 Duty_Cycle)
{
	ICR1 = 2499;
	OCR1A = Duty_Cycle;
}
/*
 * Description:
 * Function to disable the Timer1.
 */
void Timer1_DeInit(void)
{
	TCCR1A = 0;
	TCCR1B = 0;
	TIMSK = 0;
}

/*
 * Description:
 * Function to set the Call Back function address.
 */
void Timer1_SetCallBack(void(*a_ptr)(void))
{
	g_callBackPtr = a_ptr;
}

