#include "hardware_init.h"
#include "bsp_usart.h"
#include "configuration.h"
/**
  ******************************************************************************
  * @author  Louis
  * @version V1.0
  * @date    07-March-2014
  * @brief   STM32F429/43��ĿӲ�������ʼ��
  ******************************************************************************
**/

//���������ʼ��
void HardWare_Init(void)
{ 	
	SysInit();
	DBG_InitUart(COM1,115200);
	//����1��ʼ��
  delay_init();

}




