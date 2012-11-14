
/**
*   @file Lcd.h
*   @brief This file contains the function prototypes for LCD.
*
*/

#ifndef _LCD_H
#define _LCD_H

//*****************************************************************************
// Include and Header files
//*****************************************************************************

#include <p18F4680.h>
#include <GenericTypeDefs.h>

#include "General.h"

//*****************************************************************************
// Global Knobs
//*****************************************************************************

//*****************************************************************************
// Global Definitions and Equates
//*****************************************************************************

//*****************************************************************************
// Global Variables
//*****************************************************************************

//*****************************************************************************
// Global Constant Strings
//*****************************************************************************

//*****************************************************************************
// Global Variable Strings
//*****************************************************************************

//*****************************************************************************
// Global Function Prototypes
//*****************************************************************************

void LCDInit(void);                             ///< LCD initialization service
void LCDClear(void);                            ///< Screen clearing service
void LCDGoto(UINT8_T Pos,UINT8_T Ln);           ///< Cursor positioning service
void LCDPutChar(UINT8_T Data);                  ///< Outputs ASCII character
void LCDPutByte(UINT8_T Val);                   ///< Outputs binary value
void LCDWriteStr(char  *Str);     ///< Outputs strings to LCD

#endif

