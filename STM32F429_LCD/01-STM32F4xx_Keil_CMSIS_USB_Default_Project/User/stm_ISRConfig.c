/*
文  件 说 明:  NVIC配置和中断线选择
最后修改时间:  2013-3-22 15:06:08
作        者:  Louis    
*/
#include "stm_ISRConfig.h"


/**
  * @函数功能:  NVIC配置
  * @函数参数:  none
  * @返回值  :  none
 	* @说  明  :  该函数包含所有中断子函数的NVIC配置
								关于NVIC的配置说明几点
								①、Cortex-M3中定义了8个比特位用于设置中断源的优先级但STM32只定义了5bit	共可分为5组配置
								②、分配方式如下
								      第0组：所有4位用于指定响应优先级														  NVIC_PriorityGroupConfig(NVIC_PriorityGroup_0);
									  第1组：最高1位用于指定抢占式优先级，最低3位用于指定响应优先级 NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1);
									  第2组：最高2位用于指定抢占式优先级，最低2位用于指定响应优先级	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
									  第3组：最高3位用于指定抢占式优先级，最低1位用于指定响应优先级	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_3);
									  第4组：所有4位用于指定抢占式优先级														NVIC_PriorityGroupConfig(NVIC_PriorityGroup_4);
								③、每个工程只能配置一组NVIC  也就是只能用一次NVIC_PriorityGroupConfig(NVIC_PriorityGroup_x)
								④、很明显能配置2的5次方 32级中断

  */
void bsp_NVIC_Configuration(void)
{
  NVIC_InitTypeDef NVIC_InitStructure; 

  NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
    
/*****   调试用的串口1配置优先级  *****/
  NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;					//配置串口1为中断源
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0; 			//设置占先优先级为2
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 4;		  			//设置副优先级为0
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			  			//使能串口2中断	 //DISABLE
  NVIC_Init(&NVIC_InitStructure);							  			//根据参数初始化中断寄存器  
  USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);//使能串口1接收中断
/*****   调试用的串口1配置优先级  *****/


/*****   定时器NVIC配置优先级  *****/	           									
  NVIC_InitStructure.NVIC_IRQChannel = TIM3_IRQn;	  	  //定时器2中断
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2;	  //事件检测
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);

 													
}
