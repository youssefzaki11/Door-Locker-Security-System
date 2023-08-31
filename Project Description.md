Description:
Door Locker Security System is an Embedded Systems Project which aims to unlock a door using password.

1. The User create a password by entering first and confirmed Passwords, and if they are matched so the password will be saved in external EEPROM. 
2. The user has two options to choose between them after creating a password ( Open the Door or Change Password).
3. To select one of this options, the user are asked to enter password, if the password doesn't match with the one saved in external EEPROM then the system will give a wrong message. 
4. If the password entered is matched with the one saved in external EEPROM, so the user can access the option that selected.  
5. If the password entered three times wrongly, then the system will give an error to the user.

Programming Language: C Programming -
Target: AVR ATMega32 Microcontroller

Drivers: 
1. MCAL Drivers: 
{GPIO - TIMER0 - TIMER1 - UART - I2C}
2. HAL Drivers:
{Keypad - LCD - Buzzer - EEPROM - DC Motor}

ECus: {Control ECU - HMI ECU} 
