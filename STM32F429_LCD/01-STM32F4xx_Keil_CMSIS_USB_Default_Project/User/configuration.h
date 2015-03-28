#ifndef CONFIGURATION_H
#define CONFIGURATION_H

#include "stm32f4xx.h"
#include "stm32f4xx_tim.h"
#include "stm32f4xx_rcc.h"

void SysInit(void);
void USART1_Configure(void);
void NVIC_Config(void);
#endif
