/*******************************************************************************************************************
 * File Name: GPIO.c
 * Date: 25/6/2023
 * Driver: ATmega32 GPIO Driver Source File
 * Author: Youssef Zaki
 ******************************************************************************************************************/
#include <avr/io.h>
#include "GPIO.h"
#include "Common_Macros.h"

/****************************************************************************************
 *                                      Functions Definitions                           *
 ****************************************************************************************/

/*
 * Description:
 * Setup the direction of the PIN if it is INPUT (LOGIC LOW) or it is OUTPUT (LOGIC HIGH)
 * If the PORT or PIN numbers are not correct, the function will not handle the request
 */
void GPIO_SetupPinDirection(uint8 Port_Num, uint8 Pin_Num, uint8 GPIO_PinDirectionType)
{
	/* CHECK IF THE CORRECT NUMBER OF PORT AND NUMBER OF PIN ARE ENTERED */
	if ((Port_Num >= PORTA_ID && Port_Num <= PORTD_ID) && (Pin_Num >= PIN0_ID && Pin_Num <= PIN7_ID))
	{
		switch (Port_Num)
		{
		case PORTA_ID:
			if (GPIO_PinDirectionType == OUTPUT_PIN)
			{
				SET_BIT(DDRA, Pin_Num);
			}
			else
			{
				CLEAR_BIT(DDRA, Pin_Num);
			}
			break;

		case PORTB_ID:
			if (GPIO_PinDirectionType == OUTPUT_PIN)
			{
				SET_BIT(DDRB, Pin_Num);
			}
			else
			{
				CLEAR_BIT(DDRB, Pin_Num);
			}
			break;

		case PORTC_ID:
			if (GPIO_PinDirectionType == OUTPUT_PIN)
			{
				SET_BIT(DDRC, Pin_Num);
			}
			else
			{
				CLEAR_BIT(DDRC, Pin_Num);
			}
			break;

		case PORTD_ID:
			if (GPIO_PinDirectionType == OUTPUT_PIN)
			{
				SET_BIT(DDRD, Pin_Num);
			}
			else
			{
				CLEAR_BIT(DDRD, Pin_Num);
			}
			break;
		}
	}
	else
	{
		/* Do Nothing if the wrong number of port or number of pin are entered */
	}
}

/*
 * Description:
 * write the value LOGIC HIGH or LOGIC LOW on the required PIN in the required PORT
 * If the PORT or PIN numbers are not correct, the function will not handle the request
 * If the PIN is INPUT PIN, so the PIN will enable/disable the internal pull-up resistor
 */
void GPIO_WritePin(uint8 Port_Num, uint8 Pin_Num, uint8 value)
{
	/* CHECK IF THE CORRECT NUMBER OF PORT AND NUMBER OF PIN ARE ENTERED */
	if ((Port_Num >= PORTA_ID && Port_Num <= PORTD_ID) && (Pin_Num >= PIN0_ID && Pin_Num <= PIN7_ID))
	{
		switch(Port_Num)
		{
		case PORTA_ID:
			if (value == LOGIC_HIGH)
			{
				SET_BIT(PORTA, Pin_Num);
			}
			else
			{
				CLEAR_BIT(PORTA, Pin_Num);
			}
			break;

		case PORTB_ID:
			if (value == LOGIC_HIGH)
			{
				SET_BIT(PORTB, Pin_Num);
			}
			else
			{
				CLEAR_BIT(PORTB, Pin_Num);
			}
			break;

		case PORTC_ID:
			if (value == LOGIC_HIGH)
			{
				SET_BIT(PORTC, Pin_Num);
			}
			else
			{
				CLEAR_BIT(PORTC, Pin_Num);
			}
			break;

		case PORTD_ID:
			if (value == LOGIC_HIGH)
			{
				SET_BIT(PORTD, Pin_Num);
			}
			else
			{
				CLEAR_BIT(PORTD, Pin_Num);
			}
			break;
		}
	}
	else
	{
		/* Do Nothing */
	}
}

/*
 * Description:
 * read the value LOGIC HIGH or LOGIC LOW on the required PIN in the required PORT
 * If the PORT or PIN numbers are not correct, the function will return LOGIC LOW
 * The function will return LOGIC HIGH or LOGIC LOW
 */
uint8 GPIO_ReadPin(uint8 Port_Num, uint8 Pin_Num)
{
	uint8 Pin_Value = LOGIC_LOW;

	/* CHECK IF THE CORRECT NUMBER OF PORT AND NUMBER OF PIN ARE ENTERED */
	if ( (Port_Num >= PORTA_ID && Port_Num <= PORTD_ID) && (Pin_Num >= PIN0_ID && Pin_Num <= PIN7_ID) )
	{
		switch(Port_Num)
		{
		case PORTA_ID:
			if (BIT_IS_SET (PINA, Pin_Num))
			{
				Pin_Value = LOGIC_HIGH;
			}
			else
			{
				Pin_Value = LOGIC_LOW;
			}
			break;

		case PORTB_ID:
			if (BIT_IS_SET (PINB, Pin_Num))
			{
				Pin_Value = LOGIC_HIGH;
			}
			else
			{
				Pin_Value = LOGIC_LOW;
			}
			break;

		case PORTC_ID:
			if (BIT_IS_SET (PINC, Pin_Num))
			{
				Pin_Value = LOGIC_HIGH;
			}
			else
			{
				Pin_Value = LOGIC_LOW;
			}
			break;

		case PORTD_ID:
			if (BIT_IS_SET (PIND, Pin_Num))
			{
				Pin_Value = LOGIC_HIGH;
			}
			else
			{
				Pin_Value = LOGIC_LOW;
			}
			break;
		}
	}
	else
	{
		/* Function will return ZERO */
	}
	return Pin_Value;
}

/*
 * Description:
 * Setup the direction of the PORT if it is INPUT (LOGIC LOW) or it is OUTPUT (LOGIC HIGH)
 * If the PORT number is not correct, the function will not handle the request
 */
void GPIO_SetupPortDirection(uint8 Port_Num, GPIO_PortDirectionType direction)
{
	/* CHECK IF THE CORRECT NUMBER OF PORT IS ENTERED */
	if ((Port_Num >= PORTA_ID && Port_Num <= PORTD_ID))
	{
		/* "direction" VALUE FROM 0x00 to 0xFF */
		switch (Port_Num)
		{
		case PORTA_ID:
			DDRA = direction;
			break;
		case PORTB_ID:
			DDRB = direction;
			break;
		case PORTC_ID:
			DDRC = direction;
			break;
		case PORTD_ID:
			DDRD = direction;
			break;
		}
	}
	else
	{
		/* Do nothing if the entered port number is not correct */
	}
}

/*
 * Description:
 * write the value on the required PORT
 * If the PORT number is not correct, the function will not handle the request
 */
void GPIO_WritePORT(uint8 Port_Num, uint8 value)
{
	/* CHECK IF THE CORRECT NUMBER OF PORT IS ENTERED */
	if ((Port_Num >= PORTA_ID && Port_Num <= PORTD_ID))
	{
		switch (Port_Num)
		{
		case PORTA_ID:
			PORTA = value;
			break;
		case PORTB_ID:
			PORTB = value;
			break;
		case PORTC_ID:
			PORTC = value;
			break;
		case PORTD_ID:
			PORTD = value;
			break;
		}
	}
	else
	{
		/* Do nothing if the entered port number is not correct */
	}
}

/*
 * Description:
 * read the value on the required PORT
 * If the PORT number is not correct, the function will return ZERO
 * The function will return the read value on the PORT
 */
uint8 GPIO_ReadPORT(uint8 Port_Num)
{
	uint8 Port_Value = 0x00;

	/* CHECK IF THE CORRECT NUMBER OF PORT IS ENTERED */
	if ((Port_Num >= PORTA_ID && Port_Num <= PORTD_ID))
	{
		switch (Port_Num)
		{
		case PORTA_ID:
			Port_Value = PINA;
			break;
		case PORTB_ID:
			Port_Value = PINB;
			break;
		case PORTC_ID:
			Port_Value = PINC;
			break;
		case PORTD_ID:
			Port_Value = PIND;
			break;
		}
	}
	else
	{
		/* function will return ZERO */
	}
	return Port_Value;
}
