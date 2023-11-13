#include "stm32f10x.h"                  // Device header
#include "Delay.h"

int main(void)
{
	//使能外设GPIO时钟
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
	//定义结构体
	GPIO_InitTypeDef GPIO_InitStrucrure;
	//定义引脚，模式
	GPIO_InitStrucrure.GPIO_Mode = GPIO_Mode_IPU;
	//定义引脚，引脚
	GPIO_InitStrucrure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_2;
	//定义引脚，速度
	GPIO_InitStrucrure.GPIO_Speed = GPIO_Speed_50MHz;
	//初始化
	GPIO_Init(GPIOA, &GPIO_InitStrucrure);
	
	while(1)
	{
		GPIO_ResetBits(GPIOA, GPIO_Pin_0);
		Delay_ms(500);
		GPIO_SetBits(GPIOA, GPIO_Pin_0);
		Delay_ms(500);
		GPIO_ResetBits(GPIOA, GPIO_Pin_1);
		Delay_ms(500);
		GPIO_SetBits(GPIOA, GPIO_Pin_1);
		Delay_ms(500);
		GPIO_ResetBits(GPIOA, GPIO_Pin_2);
		Delay_ms(500);
		GPIO_SetBits(GPIOA, GPIO_Pin_2);
		Delay_ms(500);
	}
}
