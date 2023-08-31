/*******************************************************************************************************************
 * File Name: TIMER0.c
 * Date: 16/7/2023
 * Driver: ATmega32 TIMER0 Driver Source File
 * Author: Youssef Zaki
 ******************************************************************************************************************/
#include <avr/io.h>
#include "Common_Macros.h"
#include "GPIO.h"
#include "TIMER0.h"

/***************************************************************************************
 *                                         Global Variables                            *
 ***************************************************************************************/

/* Global variables to hold the address of the call back function in the application */
static volatile void (*g_CallBackPtr)(void) = NULL_PTR;

/****************************************************************************************
 *                                      Functions Definitions                           *
 ****************************************************************************************/

/*
 * Description:
 * Initialization of Timer0 (Enable Timer0)
 * 1. Let the TCNT0 Register = The Start value of the timer.
 * 2. Enable FOC0 bit in the TCCR0 Register.
 * 3. Enable CS02:0 bits according to the required pre-scalar
 * 4. Configure the TCCR0 Register according to the Timer0 Mode.
 * 5. Configure the TIMSK Register (Interrupt Mask) according to Timer0 Mode.
 * 6. In CTC Mode Let OCR0 = the compare value (TOP Value)
 */
void Timer0_NonPWM_Mode_Init(const Timer0_ConfigType* Config_Ptr)
{
	TCNT0 = Config_Ptr -> Initial_Value;
	SET_BIT(TCCR0, FOC0);
	TCCR0 = (TCCR0 & 0xF8) | (Config_Ptr -> Prescalar);

	if (Config_Ptr -> Timer_Mode == TIMER0_Normal_0)
	{
		/* Configuration of Normal Mode
		 * WGM01 = 0, WGM00 = 0, COM00 = 0, COM01 = 0
		 */
		TCCR0 = (TCCR0 & 0x87);

		SET_BIT(TIMSK, TOIE0);
	}

	else if (Config_Ptr -> Timer_Mode == TIMER0_CTC_2)
	{
		OCR0 = Config_Ptr -> Compare_Value;

		/* Configuration of CTC Mode
		 * WGM01 = 1, WGM00 = 0, COM00 = 0, COM01 = 0
		 */
		TCCR0 = (TCCR0 & 0x87) | (1 << WGM01);

		SET_BIT(TIMSK, OCIE0);
	}
}

/*
 * Description:
 * The function responsible for trigger the Timer0 with the PWM Mode.
 * 1. Let the TCNT0 Register = The Start value of the timer.
 * 2. Disable FOC0 bit in the TCCR0 Register (FOC0 = 0)
 * 3. Enable CS02:0 bits according to the required pre-scalar.
 * 4. Setup the direction for OC0 as output pin through the GPIO driver.
 * 5. Configuration the PWM Mode (Phase Correct or Fast)
 * 6. Setup the PWM mode with Non-Inverting.
 */
void Timer0_PWM_Mode_Init(const Timer0_ConfigType* Config_Ptr)
{
	TCNT0 = Config_Ptr -> Initial_Value;

	CLEAR_BIT(TCCR0, FOC0);

	TCCR0 = (TCCR0 & 0xF8) | (Config_Ptr -> Prescalar);

	GPIO_SetupPinDirection(PORTB_ID, PIN3_ID, OUTPUT_PIN);

	if (Config_Ptr -> Timer_Mode == TIMER0_PhaseCorrect_PWM_1)
	{
		/* Configuration of Phase Correct Mode (Non-Inverting)
		 * WGM01 = 0, WGM00 = 1, COM00 = 0, COM01 = 1
		 */
		TCCR0 = (TCCR0 & 0x07) | (1 << WGM00) | (1<<COM01);
	}

	else if (Config_Ptr -> Timer_Mode == TIMER0_Fast_PWM_3)
	{
		/* Configuration of Fast PWM Mode (Non-Inverting)
		 * Clear OC0 when match occurs (non inverted mode) COM00=0 & COM01=1
		 * WGM01 = 1, WGM00 = 1, COM00 = 0, COM01 = 1
		 */
		TCCR0 = (TCCR0 & 0x07) | (1 << WGM00) | (1<<WGM01) | (1<<COM01);
	}
}

/*
 * Description:
 * The Speed is passed to this function to calculate the duty cycle and hence get the OCR0 Value.
 */
void TIMER0_PWM_Start(uint8 Duty_Cycle)
{
	OCR0 = ((float)((Duty_Cycle)*255)/100);
}

/*
 * Description:
 * De-initialization of Timer0 (Disable)
 */
void Timer0_DeInit(void)
{
	TCNT0 = 0;
	TCCR0 = 0;
	TIMSK = 0;
}

/*
 * Description:
 * Function to set the Call Back function address.
 */
void Timer0_SetCallBack(void(*a_ptr)(void))
{
	g_CallBackPtr = a_ptr;
}
