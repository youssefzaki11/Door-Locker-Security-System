/*****************************************************************************************************************
 * File Name: EEPROM.h
 * Date: 14/8/2023
 * Driver: EEPROM Driver Header File
 * Author: Youssef Zaki
 ****************************************************************************************************************/
#include "Standard_types.h"

#ifndef EEPROM_H_
#define EEPROM_H_

/******************************************************************************************
 *                                    Macros Definitions                                  *
 ******************************************************************************************/
#define ERROR 0
#define SUCCESS 1

/*******************************************************************************************
 *                                      Functions Prototypes                               *
 *******************************************************************************************/

uint8 EEPROM_WriteByte(uint16 EEPROM_Byte_Address, uint8 EEPROM_Data);

uint8 EEPROM_ReadByte(uint16 EEPROM_Byte_Address, uint8 *EEPROM_Data);

#endif /* EEPROM_H_ */
