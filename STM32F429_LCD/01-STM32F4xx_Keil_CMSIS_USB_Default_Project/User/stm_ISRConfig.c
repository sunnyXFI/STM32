/*
��  �� ˵ ��:  NVIC���ú��ж���ѡ��
����޸�ʱ��:  2013-3-22 15:06:08
��        ��:  Louis    
*/
#include "stm_ISRConfig.h"


/**
  * @��������:  NVIC����
  * @��������:  none
  * @����ֵ  :  none
 	* @˵  ��  :  �ú������������ж��Ӻ�����NVIC����
								����NVIC������˵������
								�١�Cortex-M3�ж�����8������λ���������ж�Դ�����ȼ���STM32ֻ������5bit	���ɷ�Ϊ5������
								�ڡ����䷽ʽ����
								      ��0�飺����4λ����ָ����Ӧ���ȼ�														  NVIC_PriorityGroupConfig(NVIC_PriorityGroup_0);
									  ��1�飺���1λ����ָ����ռʽ���ȼ������3λ����ָ����Ӧ���ȼ� NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1);
									  ��2�飺���2λ����ָ����ռʽ���ȼ������2λ����ָ����Ӧ���ȼ�	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
									  ��3�飺���3λ����ָ����ռʽ���ȼ������1λ����ָ����Ӧ���ȼ�	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_3);
									  ��4�飺����4λ����ָ����ռʽ���ȼ�														NVIC_PriorityGroupConfig(NVIC_PriorityGroup_4);
								�ۡ�ÿ������ֻ������һ��NVIC  Ҳ����ֻ����һ��NVIC_PriorityGroupConfig(NVIC_PriorityGroup_x)
								�ܡ�������������2��5�η� 32���ж�

  */
void bsp_NVIC_Configuration(void)
{
  NVIC_InitTypeDef NVIC_InitStructure; 

  NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
    
/*****   �����õĴ���1�������ȼ�  *****/
  NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;					//���ô���1Ϊ�ж�Դ
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0; 			//����ռ�����ȼ�Ϊ2
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 4;		  			//���ø����ȼ�Ϊ0
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			  			//ʹ�ܴ���2�ж�	 //DISABLE
  NVIC_Init(&NVIC_InitStructure);							  			//���ݲ�����ʼ���жϼĴ���  
  USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);//ʹ�ܴ���1�����ж�
/*****   �����õĴ���1�������ȼ�  *****/


/*****   ��ʱ��NVIC�������ȼ�  *****/	           									
  NVIC_InitStructure.NVIC_IRQChannel = TIM3_IRQn;	  	  //��ʱ��2�ж�
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2;	  //�¼����
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);

 													
}
