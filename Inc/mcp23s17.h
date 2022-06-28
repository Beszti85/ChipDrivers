/*
 * mcp23s17_spi.h
 *
 *  Created on: 2022. jún. 20.
 *      Author: drCsabesz
 */

#ifndef INC_MCP23S17_H_
#define INC_MCP23S17_H_

#include <stdint.h>
#include <stdbool.h>

void MCP23S17_Init( void );
void MCP23S17_SetIODirectionAB( uint8_t portAValue, uint8_t portBValue );
void MCP23S17_WriteOutputA( uint8_t value );
void MCP23S17_WriteOutputB( uint8_t value );
void MCP23S17_ReadAllRegs( void );

#endif /* INC_MCP23S17_H_ */
