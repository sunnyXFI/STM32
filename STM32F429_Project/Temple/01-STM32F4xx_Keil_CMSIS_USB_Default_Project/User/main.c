/**
 *	Keil project template
 *
 *	Before you start, select your target, on the right of the "Load" button
 *
 *	@author		Tilen Majerle
 *	@email		tilen@majerle.eu
 *	@website	http://stm32f4-discovery.com
 *	@ide		Keil uVision 5
 *	@packs		STM32F4xx Keil packs version 2.2.0 or greater required
 *	@stdperiph	STM32F4xx Standard peripheral drivers version 1.4.0 or greater required
 */
/* Include core modules */
#include "stm32f4xx.h"
/* Include my libraries here */
#include "defines.h"
#include "tm_stm32f4_delay.h"
#include "tm_stm32f4_usart.h"
#include "tm_stm32f4_disco.h"

int main(void) {
	
		uint8_t c;
    
    //Initialize system
    SystemInit();
 
    TM_DELAY_Init();
 
    //Initialize USART1 at 9600 baud, TX: PA9, RX: PA10
    TM_USART_Init(USART1, TM_USART_PinsPack_1, 9600);
    //Put string to terminal
    TM_USART_Puts(USART1, "Hello world\n\r");
 
    while (1) {
        //Get character from internal buffer
        c = TM_USART_Getc(USART1);
        if (c) {
            //If anything received, put it back to terminal
            TM_USART_Putc(USART1, c);
        }
    }
}
