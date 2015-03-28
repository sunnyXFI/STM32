#include "configuration.h"
#include "stm32f4xx_usart.h"
#include "stm32f4xx_rcc.h"


void SysInit(void)
{
		//Resets the RCC clock configuration to the default reset state
		RCC_DeInit();
	
		//Configures the External High Speed oscillator (HSE)
		RCC_HSEConfig(RCC_HSE_ON);
	
		//Waits for HSE start-up
		while( RCC_WaitForHSEStartUp() != SUCCESS );
	
		//Configures the main PLL clock source, multiplication and division factors
		//M=8,N=360,P=16,Q=7
		RCC_PLLConfig(RCC_PLLSource_HSE,8,360,2,7);
	
		//Enables or disables the main PLL
		RCC_PLLCmd(ENABLE);
	
		//Configures the system clock (SYSCLK)
		RCC_SYSCLKConfig(RCC_SYSCLKSource_PLLCLK);
		
		//Loop until the PLL become the SYSCLK source
		while(RCC_GetSYSCLKSource() != 0x08)
	
		//Configures the AHB clock (HCLK)
		 RCC_HCLKConfig(RCC_HCLK_Div1);
		 
		 //Configures the Low Speed APB clock (PCLK1)
		 RCC_PCLK1Config(RCC_HCLK_Div4);
		 
		 //Configures the High Speed APB clock (PCLK2).
		 RCC_PCLK2Config(RCC_HCLK_Div2);
	
}

void USART1_Configure(void){
	
	GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStruct;
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE);
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);

	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_AF;
	//GPIO_InitStructure.GPIO_OType=GPIO_OType_PP;//此句话如果不注掉会发生错误
	GPIO_InitStructure.GPIO_PuPd=GPIO_PuPd_UP;
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_9|GPIO_Pin_10;
	
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	GPIO_PinAFConfig(GPIOA, GPIO_PinSource9, GPIO_AF_USART1);
	GPIO_PinAFConfig(GPIOA, GPIO_PinSource10, GPIO_AF_USART1);
	
	USART_InitStruct.USART_BaudRate=115200;
	USART_InitStruct.USART_WordLength=USART_WordLength_8b;
	USART_InitStruct.USART_HardwareFlowControl=USART_HardwareFlowControl_None;
	USART_InitStruct.USART_Mode=USART_Mode_Rx|USART_Mode_Tx;
	USART_InitStruct.USART_Parity=USART_Parity_No;
	USART_InitStruct.USART_StopBits=USART_StopBits_1;
	
	USART_Init(USART1,&USART_InitStruct);

	//USART_ITConfig(USART1,USART_IT_RXNE,ENABLE);
	//USART_ITConfig(USART1,USART_IT_TXE,ENABLE);
	
	USART_Cmd(USART1,ENABLE);
}


void NVIC_Config(){
	
	NVIC_InitTypeDef NVIC_InitStructure;

	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x03;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x02;
	NVIC_InitStructure.NVIC_IRQChannel  = USART1_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	
	NVIC_Init(&NVIC_InitStructure);
}
