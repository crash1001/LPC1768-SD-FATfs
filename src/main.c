#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "LPC17xx.h"

#include "lpc17xx_uart.h"
#include "lpc17xx_pinsel.h"
#include "lpc17xx_gpio.h"
#include "lpc17xx_rtc.h"

#include "uart_console.h"
#include "monitor.h"

#include "sd_lpc17xx_spi.h"

#include "ff.h"
#include "integer.h"
#include "diskio.h"
#include "ffconf.h"

/**************************************************PREDEFINED CONSOLE MESSAGES******************************************************************/
uint8_t printMenu[] = "Starting SD Card interface test!!! \r\n";
uint8_t uartInitMsg[] = "UART0 initialized for console \r\n";
uint8_t spiInitMsg[] = "SPI0 initialized for sd card interface \r\n";
uint8_t sysTickMsg[] = "sysTick Timer Started \r\n";
char textBuff[] = "Hello I am Writing Now\r\n";
/**************************************************************************************************************************************/

static volatile uint32_t SysTickCount = 0;
//static char Line[128];				/* Console input buffer */
FATFS Fatfs[_VOLUMES];		/* File system object for each logical drive */

//Function for initializing 
static void prvHardwareSetup(void);
/**************************************************************************************************************************************/

//Systick functions
void SysTick_Handler(void);
void _delayMs(uint32_t delayMs);
/**************************************************************************************************************************************/

DWORD get_fattime ();

/**************************************************MAIN FUNCTION***********************************************************************/
int main(void) {
	char temp_Buff[128];
	DWORD sss;
	uint8_t i;
	DSTATUS errd;
	FRESULT errf;
	char *ptr;
	UINT s2;
	char *Buff_ptr;
	
	FIL file1;
	
	Buff_ptr = &temp_Buff[0];

	prvHardwareSetup();				/* Setup/initialize all configurations and hardware */	
	_delayMs(500);
	xputs("\nHelo World\n");
		xprintf("rc=%d\n", disk_initialize(0));
		xputs("\nDisk Initialized\n");
	_delayMs(500);
		errf = f_mount(0, &Fatfs[0]);
		xprintf("\nDisk Mounted, result = %d\n", errf);
//	
		ptr = "logdata.txt";
	
		errf = f_open(&file1, ptr, FA_OPEN_ALWAYS);
		xprintf("\nFile opened, error = %d\n", errf);
		
		f_lseek(&file1, 110);
		for(i=0; i<5; i++) {
			sprintf(Buff_ptr, "Count = %d\r\n", i);
			errf = f_write(&file1, Buff_ptr, strlen(Buff_ptr), &s2);
			xprintf("\nCounting again  = %d", i);
			_delayMs(500);
		}
		
//		sss = f_size(&file1);
			xprintf("\nsize = %d", sss);
		SPI_CS_High ();
		SPI_RecvByte();
		_delayMs(500);
		f_close(&file1);
		
		_delayMs(500);
		SPI_CS_Low();
		errf = f_open(&file1, ptr, FA_READ);
		xprintf("\nFile opened, error = %d\n", errf);
		f_lseek(&file1, 0);
		errf = f_read(&file1, Buff_ptr, sss, &s2);
		xprintf("\nRead Data : \n%s\n", Buff_ptr);
		_delayMs(500);
		f_close(&file1);
}
/*****************************************************************************************************************************************/


/**************************************************INITIALIZING FUNCTION******************************************************************/
static void prvHardwareSetup(void) {
	
	SystemInit();
	SysTick_Config(SystemCoreClock/1000 - 1); /* Generate interrupt each 1 ms   */
	
	init_UART0(115200);												/* Setup/initialize all configurations and hardware */	
	xprintf("\r\n%s", uartInitMsg);		

	
	
}
/****************************************************************************************************************************************/

/****************************************************SYSTICK FUNCTIONS******************************************************************/
void SysTick_Handler(void) {
	static uint32_t div10;
	SysTickCount++;
	if(++div10 >=10)
	{
		div10 = 0;
		disk_timerproc();
	}
}

void _delayMs(uint32_t delayMs) {
	uint32_t currentTicks;
	
	currentTicks = SysTickCount;
	while((SysTickCount- currentTicks) < delayMs);
}
/***************************************************************************************************************************************/


/********************************************************************************************************************/		
DWORD get_fattime ()
{
//	RTCTime rtc;

//	/* Get local time */
//	rtc_gettime(&rtc);

//	/* Pack date and time into a DWORD variable */
//	return	  ((DWORD)(rtc.year - 1980) << 25)
//			| ((DWORD)rtc.mon << 21)
//			| ((DWORD)rtc.mday << 16)
//			| ((DWORD)rtc.hour << 11)
//			| ((DWORD)rtc.min << 5)
//			| ((DWORD)rtc.sec >> 1);
	return 2;
}
