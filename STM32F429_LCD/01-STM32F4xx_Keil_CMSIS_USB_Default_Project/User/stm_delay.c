#include "stm_delay.h"
#include "stm_sys.h"
#include "bsp_usart.h"
#include "stdio.h"


/**
  ******************************************************************************
  * @author  Louis
  * @version V1.0
  * @date    07-March-2014
  * @brief   STM32F429/43�ӳٳ�ʼ��ʱ�ӣ��룬���룬΢�ϵͳʱ�Ӽ��
  ******************************************************************************
**/


static u8  fac_us=0;//us��ʱ������
static u16 fac_ms=0;//ms��ʱ������


//��ʼ���ӳٺ���
//��ʹ��ucos��ʱ��,�˺������ʼ��ucos��ʱ�ӽ���
//SYSTICK��ʱ�ӹ̶�ΪHCLKʱ�ӵ�1/8
//SYSCLK:ϵͳʱ��
void delay_init(void)	 
{
    RCC_ClocksTypeDef RCC_Clocks; //ϵͳ�����ֵ�ʱ�ӽṹ��
	

	SysTick_CLKSourceConfig(SysTick_CLKSource_HCLK_Div8);	//ѡ���ⲿʱ��  HCLK/8
	//�õ�ϵͳʱ��
  RCC_GetClocksFreq(&RCC_Clocks);

	/*************ϵͳʱ���Լ�**********************/
	DbgPrintf("SYSCLK:%d\n ", RCC_Clocks.SYSCLK_Frequency);
	DbgPrintf("HCLK:%d\n ",   RCC_Clocks.HCLK_Frequency);  
	DbgPrintf("PCLK1:%d\n ",  RCC_Clocks.PCLK1_Frequency);
	DbgPrintf("PCLK2:%d\n ",  RCC_Clocks.PCLK2_Frequency);  
	/*************ϵͳʱ���Լ�**********************/
  fac_us = RCC_Clocks.HCLK_Frequency/4000000;	//Ϊϵͳʱ�ӵ�1/8 	

	fac_ms = (u16)fac_us*500;//��ucos��,����ÿ��ms��Ҫ��systickʱ����   

}								    

//��ʱnus
//nusΪҪ��ʱ��us��.		    								   
void delay_2us(u32 nus)
{		
	u32 temp;	    	 
	SysTick->LOAD=nus*fac_us; //ʱ�����	  		 
	SysTick->VAL=0x00;        //��ռ�����
	SysTick->CTRL|=SysTick_CTRL_ENABLE_Msk ;          //��ʼ����	 
	do
	{
		temp=SysTick->CTRL;
	}
	while(temp&0x01&&!(temp&(1<<16)));//�ȴ�ʱ�䵽��   
	SysTick->CTRL&=~SysTick_CTRL_ENABLE_Msk;       //�رռ�����
	SysTick->VAL =0X00;       //��ռ�����	 
}
//��ʱnms
//ע��nms�ķ�Χ
//SysTick->LOADΪ24λ�Ĵ���,����,�����ʱΪ:
//nms<=16777215/22.5
//SYSCLK��λΪHz,nms��λΪms
//��180M������,nms<=745.654 
void delay_ms(u16 nms)
{	 		  	  
	u32 temp;		   
	SysTick->LOAD=(u32)nms*fac_ms;//ʱ�����(SysTick->LOADΪ24bit)
	SysTick->VAL =0x00;           //��ռ�����
	SysTick->CTRL|=SysTick_CTRL_ENABLE_Msk ;          //��ʼ����  
	do
	{
		temp=SysTick->CTRL;
	}
	while(temp&0x01&&!(temp&(1<<16)));//�ȴ�ʱ�䵽��   
	SysTick->CTRL&=~SysTick_CTRL_ENABLE_Msk;       //�رռ�����
	SysTick->VAL =0X00;       //��ռ�����	  	    
} 
//��ʱ��
void delay_Second(u8 second)
{
   u8  i = 0;
   for(i=0;i<second;i++)
   {
      delay_ms(500);
		  delay_ms(500);
   }

}





































