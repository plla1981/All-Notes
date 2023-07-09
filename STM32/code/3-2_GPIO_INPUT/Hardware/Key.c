#include "stm32f10x.h"                  // Device header
#include "delay.h"

void Key_Init(void)
{
	//开启外设时钟（GPIO）
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);
	
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_13;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	
	GPIO_Init(GPIOC, &GPIO_InitStructure);
	
}

uint8_t Key_GetNum(void)
{
	uint8_t KeyNum = 0;
	
	//读取gpio的值
	if (!GPIO_ReadInputDataBit(GPIOC, GPIO_Pin_13)){
		Delay_ms(10);
		while (!GPIO_ReadInputDataBit(GPIOC, GPIO_Pin_13));
		Delay_ms(10);
		KeyNum = 1;
	}
	
	return KeyNum;
}
