#include "hardware_init.h"
#include "bsp_usart.h"
#include "configuration.h"
/**
  ******************************************************************************
  * @author  Louis
  * @version V1.0
  * @date    07-March-2014
  * @brief   STM32F429/43项目硬件外设初始化
  ******************************************************************************
**/

//各个外设初始化
void HardWare_Init(void)
{ 	
	SysInit();
	DBG_InitUart(COM1,115200);
	//串口1初始化
  delay_init();

}




