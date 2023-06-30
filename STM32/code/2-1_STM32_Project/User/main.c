#include "stm32f10x.h"                  // Device header

int main(void)
{
	//1.寄存器操作点灯
	//配置外设时钟
	//RCC->APB2ENR = 0x00000004;
	//GPIOA->CRL = 0x00000333;
	//GPIOA->ODR = 0x00000000;
	
	//2.标准库函数操作点灯
	//配置外设时钟
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
	//配置端口模式
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	//GPIO_SetBits(GPIOA, GPIO_Pin_0);
	GPIO_ResetBits(GPIOA, GPIO_Pin_0);
	while (1)
	{
		
	}
}
