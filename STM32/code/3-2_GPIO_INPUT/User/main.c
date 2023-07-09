#include "stm32f10x.h"                  // Device header
#include "Delay.h"
#include "GPIO_Init.h"
#include "Key.h"

uint8_t KeyNum;

int main(void)
{
	
	LED_Init();
	Key_Init();
	//LED_Red_ON();
	//LED_Blue_ON();
	//LED_Greed_ON();
	
	while(1)
	{
		KeyNum = Key_GetNum();
		
		if (KeyNum) {
			LED_Turn();
		}
		else {
			LED_Turn();
		}
		
	}
}
