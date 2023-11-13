#include "stm32f10x.h"                  // Device header

void LED_Init(void){
	//1.初始化GPIO外设时钟
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
	
	//2.初始引脚
	//定义结构体，存放引脚，模式，速度
	GPIO_InitTypeDef GPIO_InitStructure;
	//定义模式
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	//定义引脚
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_2;
	//定义速度
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	//初始化
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	//设置引脚为高电平，因为引脚默认为低电平
	GPIO_SetBits(GPIOA, GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_2);
}

//点亮对应的LED灯
void LED_RED_ON(void){
	GPIO_ResetBits(GPIOA, GPIO_Pin_0);
}

void LED_RED_OFF(void){
	GPIO_SetBits(GPIOA, GPIO_Pin_0);
}

void LED_GREED_ON(void){
	GPIO_ResetBits(GPIOA, GPIO_Pin_1);
}

void LED_GREED_OFF(void){
	GPIO_SetBits(GPIOA, GPIO_Pin_1);
}

void LED_BLUE_ON(void){
	GPIO_ResetBits(GPIOA, GPIO_Pin_2);
}

void LED_BLUE_OFF(void){
	GPIO_SetBits(GPIOA, GPIO_Pin_2);
}
