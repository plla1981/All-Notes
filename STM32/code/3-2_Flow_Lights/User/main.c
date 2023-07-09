#include "stm32f10x.h"                  // Device header
#include "Delay.h"

int main(void)
{
	//GPIO初始化
	//开启外设时钟
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
	//结构体，局部变量
	GPIO_InitTypeDef GPIO_InitStructure;
	
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_2;
	
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	while(1)
	{
		GPIO_Write(GPIOA, ~0x0001);
		Delay_ms(500);
		GPIO_Write(GPIOA, ~0x0002);
		Delay_ms(500);
		GPIO_Write(GPIOA, ~0x0004);
		Delay_ms(500);
	}
}
