#include "stm_delay.h"
#include "stm_sys.h"
#include "bsp_usart.h"
#include "stdio.h"


/**
  ******************************************************************************
  * @author  Louis
  * @version V1.0
  * @date    07-March-2014
  * @brief   STM32F429/43延迟初始化时钟，秒，毫秒，微妙，系统时钟检测
  ******************************************************************************
**/


static u8  fac_us=0;//us延时倍乘数
static u16 fac_ms=0;//ms延时倍乘数


//初始化延迟函数
//当使用ucos的时候,此函数会初始化ucos的时钟节拍
//SYSTICK的时钟固定为HCLK时钟的1/8
//SYSCLK:系统时钟
void delay_init(void)	 
{
    RCC_ClocksTypeDef RCC_Clocks; //系统个部分的时钟结构体
	

	SysTick_CLKSourceConfig(SysTick_CLKSource_HCLK_Div8);	//选择外部时钟  HCLK/8
	//得到系统时钟
  RCC_GetClocksFreq(&RCC_Clocks);

	/*************系统时钟自检**********************/
	DbgPrintf("SYSCLK:%d\n ", RCC_Clocks.SYSCLK_Frequency);
	DbgPrintf("HCLK:%d\n ",   RCC_Clocks.HCLK_Frequency);  
	DbgPrintf("PCLK1:%d\n ",  RCC_Clocks.PCLK1_Frequency);
	DbgPrintf("PCLK2:%d\n ",  RCC_Clocks.PCLK2_Frequency);  
	/*************系统时钟自检**********************/
  fac_us = RCC_Clocks.HCLK_Frequency/4000000;	//为系统时钟的1/8 	

	fac_ms = (u16)fac_us*500;//非ucos下,代表每个ms需要的systick时钟数   

}								    

//延时nus
//nus为要延时的us数.		    								   
void delay_2us(u32 nus)
{		
	u32 temp;	    	 
	SysTick->LOAD=nus*fac_us; //时间加载	  		 
	SysTick->VAL=0x00;        //清空计数器
	SysTick->CTRL|=SysTick_CTRL_ENABLE_Msk ;          //开始倒数	 
	do
	{
		temp=SysTick->CTRL;
	}
	while(temp&0x01&&!(temp&(1<<16)));//等待时间到达   
	SysTick->CTRL&=~SysTick_CTRL_ENABLE_Msk;       //关闭计数器
	SysTick->VAL =0X00;       //清空计数器	 
}
//延时nms
//注意nms的范围
//SysTick->LOAD为24位寄存器,所以,最大延时为:
//nms<=16777215/22.5
//SYSCLK单位为Hz,nms单位为ms
//对180M条件下,nms<=745.654 
void delay_ms(u16 nms)
{	 		  	  
	u32 temp;		   
	SysTick->LOAD=(u32)nms*fac_ms;//时间加载(SysTick->LOAD为24bit)
	SysTick->VAL =0x00;           //清空计数器
	SysTick->CTRL|=SysTick_CTRL_ENABLE_Msk ;          //开始倒数  
	do
	{
		temp=SysTick->CTRL;
	}
	while(temp&0x01&&!(temp&(1<<16)));//等待时间到达   
	SysTick->CTRL&=~SysTick_CTRL_ENABLE_Msk;       //关闭计数器
	SysTick->VAL =0X00;       //清空计数器	  	    
} 
//延时秒
void delay_Second(u8 second)
{
   u8  i = 0;
   for(i=0;i<second;i++)
   {
      delay_ms(500);
		  delay_ms(500);
   }

}





































