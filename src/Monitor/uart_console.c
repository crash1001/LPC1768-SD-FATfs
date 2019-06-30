
#include "uart_console.h"
#include "lpc17xx_uart.h"
#include "lpc17xx.h"
#include "lpc17xx_pinsel.h"
#include "monitor.h"

void init_UART0(uint32_t baudrate) {
	
		
	// UART Configuration structure variable
	UART_CFG_Type UARTConfigStruct;
	// UART FIFO configuration Struct variable
	UART_FIFO_CFG_Type UARTFIFOConfigStruct;
	// Pin configuration for UART0
	PINSEL_CFG_Type PinCfg;
	
	/*
	 * Initialize UART0 pin configuration
	 */
	PinCfg.Funcnum = 1;
	PinCfg.OpenDrain = 0;
	PinCfg.Pinmode = 0;
	PinCfg.Pinnum = 2;
	PinCfg.Portnum = 0;
	PINSEL_ConfigPin(&PinCfg);
	PinCfg.Pinnum = 3;
	PINSEL_ConfigPin(&PinCfg);
	
	/* Initialize UART Configuration parameter structure to default state:
	 * Baudrate = 9600bps
	 * 8 data bit
	 * 1 Stop bit
	 * None parity
	 */
	UART_ConfigStructInit(&UARTConfigStruct);
	UARTConfigStruct.Baud_rate = 115200;

	// Initialize UART0 peripheral with given to corresponding parameter
	UART_Init((LPC_UART_TypeDef *)LPC_UART0, &UARTConfigStruct);


	/* Initialize FIFOConfigStruct to default state:
	 * 				- FIFO_DMAMode = DISABLE
	 * 				- FIFO_Level = UART_FIFO_TRGLEV0
	 * 				- FIFO_ResetRxBuf = ENABLE
	 * 				- FIFO_ResetTxBuf = ENABLE
	 * 				- FIFO_State = ENABLE
	 */
	UART_FIFOConfigStructInit(&UARTFIFOConfigStruct);

	// Initialize FIFO for UART0 peripheral
	UART_FIFOConfig((LPC_UART_TypeDef *)LPC_UART0, &UARTFIFOConfigStruct);


	// Enable UART Transmit
	UART_TxCmd((LPC_UART_TypeDef *)LPC_UART0, ENABLE);

//    /* Enable UART Rx interrupt */
//	UART_IntConfig((LPC_UART_TypeDef *)LPC_UART0, UART_INTCFG_RBR, ENABLE);
	/* Enable UART line status interrupt */
//	UART_IntConfig((LPC_UART_TypeDef *)LPC_UART0, UART_INTCFG_RLS, ENABLE);	

	xfunc_out = LPC17xx_UART_PutChar;
	xfunc_in  = LPC17xx_UART_GetChar; 


}	

void LPC17xx_UART_PutChar (uint8_t ch) {
	while (!(LPC_UART0->LSR & 0x20));    	
	LPC_UART0->THR = ch;	
}

uint8_t LPC17xx_UART_GetChar (void) 
{
	while (!(LPC_UART0->LSR & 0x01));
	return (LPC_UART0->RBR);	
}
