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
#include "configuration.h"
#include "bsp_usart.h"
#include "hardware_init.h"
#include "stm32f429i_discovery_lcd.h"

#define 	MESSAGE1    	"  ECG Waveform  "
#define 	MESSAGE1_1  	"    Display      "
#define 	MESSAGE2    	"ADC Value:"
#define 	MESSAGE5   		" ADC3=%d.%1d V "
#define 	LINENUM       0x15

#define ADC3_DR_ADDRESS     ((uint32_t)0x4001224C)

__IO uint16_t uhADC3ConvertedValue = 0;
__IO uint32_t uwADC3ConvertedVoltage = 0;

static uint16_t Xpos=6,Ypos1=249,Ypos2=249,Xpos1=6,Xpos2=6;
static uint16_t DrawData;
static uint16_t Counter=0;

//------------------------------------------------------------------------------
//ADC3通道13 DMA配置
//------------------------------------------------------------------------------
static void ADC3_CH13_DMA_Config(void)
{
  ADC_InitTypeDef       ADC_InitStructure;
  ADC_CommonInitTypeDef ADC_CommonInitStructure;
  DMA_InitTypeDef       DMA_InitStructure;
  GPIO_InitTypeDef      GPIO_InitStructure;

	//打开ADC3，DMA2，GPIO时钟
  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_DMA2 | RCC_AHB1Periph_GPIOC, ENABLE);
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC3, ENABLE);

  DMA_InitStructure.DMA_Channel = DMA_Channel_2;  
  DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t)ADC3_DR_ADDRESS;
  DMA_InitStructure.DMA_Memory0BaseAddr = (uint32_t)&uhADC3ConvertedValue;
  DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralToMemory;
  DMA_InitStructure.DMA_BufferSize = 1;
  DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
  DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Disable;
  DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_HalfWord;
  DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_HalfWord;
  DMA_InitStructure.DMA_Mode = DMA_Mode_Circular;
  DMA_InitStructure.DMA_Priority = DMA_Priority_High;
  DMA_InitStructure.DMA_FIFOMode = DMA_FIFOMode_Disable;         
  DMA_InitStructure.DMA_FIFOThreshold = DMA_FIFOThreshold_HalfFull;
  DMA_InitStructure.DMA_MemoryBurst = DMA_MemoryBurst_Single;
  DMA_InitStructure.DMA_PeripheralBurst = DMA_PeripheralBurst_Single;
  DMA_Init(DMA2_Stream0, &DMA_InitStructure);
  DMA_Cmd(DMA2_Stream0, ENABLE);

  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AN;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL ;
  GPIO_Init(GPIOC, &GPIO_InitStructure);

  ADC_CommonInitStructure.ADC_Mode = ADC_Mode_Independent;
  ADC_CommonInitStructure.ADC_Prescaler = ADC_Prescaler_Div2;
  ADC_CommonInitStructure.ADC_DMAAccessMode = ADC_DMAAccessMode_Disabled;
  ADC_CommonInitStructure.ADC_TwoSamplingDelay = ADC_TwoSamplingDelay_5Cycles;
  ADC_CommonInit(&ADC_CommonInitStructure);

  ADC_InitStructure.ADC_Resolution = ADC_Resolution_12b;
  ADC_InitStructure.ADC_ScanConvMode = DISABLE;
  ADC_InitStructure.ADC_ContinuousConvMode = ENABLE;
  ADC_InitStructure.ADC_ExternalTrigConvEdge = ADC_ExternalTrigConvEdge_None;
  ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_T1_CC1;	
  ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;
  ADC_InitStructure.ADC_NbrOfConversion = 1;
  ADC_Init(ADC3, &ADC_InitStructure);

  ADC_RegularChannelConfig(ADC3, ADC_Channel_13, 1, ADC_SampleTime_3Cycles);

  ADC_DMARequestAfterLastTransferCmd(ADC3, ENABLE);

  ADC_DMACmd(ADC3, ENABLE);

  ADC_Cmd(ADC3, ENABLE);
}

//------------------------------------------------------------------------------
//LCD显示电压值主循环
//------------------------------------------------------------------------------
static void ADCValueDisplay_MainLoop(void)
{
  uint32_t uwVoltage =0, uwMVoltage=0;
  uint8_t aTextBuffer[50];

	//把ADC采样值转换成电压值
  uwADC3ConvertedVoltage = uhADC3ConvertedValue *3000/0xFFF;
	
	LCD_SetLayer(LCD_BACKGROUND_LAYER);
  
  LCD_SetBackColor(((220>>3)<<11)|((220>>2)<<5)|(220>>3));
  LCD_SetTextColor( LCD_COLOR_BLUE );
	
  uwVoltage = (uwADC3ConvertedVoltage)/1000;
  uwMVoltage = (uwADC3ConvertedVoltage%1000)/100;

  sprintf((char*)aTextBuffer, MESSAGE5, uwVoltage, uwMVoltage);
  LCD_DisplayStringLine(LCD_LINE_12, (uint8_t*)aTextBuffer);
	
	LCD_SetLayer(LCD_FOREGROUND_LAYER);
}

//------------------------------------------------------------------------------
//电压波形显示主循环
//------------------------------------------------------------------------------
static void ADCWaveformDisp_MainLoop(void)
{
	DrawData = uhADC3ConvertedValue*(178.0/0xFFF);   

	if( ++Counter == 4 )
	{
		Counter = 0;
		LCD_SetTextColor( LCD_COLOR_BLACK );
		LCD_DrawFullRect( Xpos+1, 50, 2, 218);
		LCD_SetTextColor( LCD_COLOR_GREEN );
		Xpos=Xpos+1;
		if(Xpos>232)
		{
			Xpos=6; 
			LCD_SetTextColor( LCD_COLOR_BLACK );
			LCD_DrawLine( Xpos, 50, 218, LCD_DIR_VERTICAL);
			LCD_SetTextColor( LCD_COLOR_GREEN );
		}
		Xpos2=Xpos2+1;
		Ypos2=249-DrawData;
		if(Xpos2>232)
		{
			Xpos2=6;Xpos1=6;
		}	
		LCD_DrawUniLine( Xpos1, Ypos1, Xpos2, Ypos2);
		Xpos1=Xpos2;
		Ypos1=Ypos2;
	}	
}

//------------------------------------------------------------------------------
//界面初始化显示
//------------------------------------------------------------------------------
static void LCD_GUIDisplay(void)
{
	uint16_t i;
	
  LCD_SetLayer(LCD_BACKGROUND_LAYER);		//设置为背景层
  LCD_SetTransparency(100);  						//背景层透明度
  LCD_Clear( ((220>>3)<<11)|((220>>2)<<5)|(220>>3) );	//把LCD清除为指定颜色

	LCD_SetBackColor(LCD_COLOR_BLUE);
  LCD_SetTextColor(LCD_COLOR_BLACK);
		
	LCD_DrawFullRect(5, 49, 229, 219);

	LCD_SetTextColor( ((180>>3)<<11)|((180>>2)<<5)|(180>>3) );
	LCD_DrawRect( 5, 49, 219, 229);
	
	LCD_SetTextColor( ((180>>3)<<11)|((180>>2)<<5)|(180>>3) );
	LCD_DrawUniLine( 4, 48, 234, 48);
	LCD_DrawUniLine( 4, 48, 4, 268);
	
	LCD_SetTextColor( ((128>>3)<<11)|((128>>2)<<5)|(128>>3) );
	LCD_DrawUniLine( 5, 49, 233, 49);
	LCD_DrawUniLine( 5, 49, 5, 267);

	LCD_SetTextColor( 0XFFFF );
	LCD_DrawUniLine( 235, 49, 235, 269);
	LCD_DrawUniLine( 5, 269, 235, 269);

	LCD_SetTextColor( ((30>>3)<<11)|((30>>2)<<5)|(30>>3) );
	for( i=69; i<269; i+=20 )
		LCD_DrawUniLine( 6, i, 233, i);
	for( i=44; i<233; i+=38 )
		LCD_DrawUniLine( i, 50, i, 267);
	
  LCD_SetBackColor(((220>>3)<<11)|((220>>2)<<5)|(220>>3));
  LCD_SetTextColor(LCD_COLOR_BLUE);
	LCD_SetFont(&Font16x24);
	LCD_DisplayStringLine(LCD_LINE_0, (uint8_t*)MESSAGE1);
	LCD_DisplayStringLine(LCD_LINE_1, (uint8_t*)MESSAGE1_1);

	LCD_SetLayer(LCD_FOREGROUND_LAYER);			//把LCD设置为前景显示层
	LCD_SetTransparency(120);								//前景层透明度
	LCD_Clear( LCD_COLOR_BLACK );	
}

int main(void){
	

	SysInit();
	
	LCD_Init();											//LCD初始化
	
  LCD_LayerInit();								//LCD层次显示初始化  
	
	LTDC_Cmd(ENABLE);								//打开LTDC
	
	LCD_GUIDisplay();
	
	ADC3_CH13_DMA_Config();					//配置ADC3 
  ADC_SoftwareStartConv(ADC3);		//开始ADC转换
	
		while(1)
	{    
    ADCValueDisplay_MainLoop();		//显示电压值
		ADCWaveformDisp_MainLoop(); 	//波形显示			
	}
	
}
