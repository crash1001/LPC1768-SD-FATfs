

#ifndef _UART_CONSOLE_H
#define _UART_CONSOLE_H

#include <stdint.h>
//uart functions
void init_UART0(uint32_t baudrate);
void LPC17xx_UART_PutChar (uint8_t);
uint8_t LPC17xx_UART_GetChar (void);

#endif

