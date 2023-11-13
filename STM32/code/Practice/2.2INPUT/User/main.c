#include "stm32f10x.h"                  // Device header
#include "Delay.h"
#include "LED.h"
#include "stm32f10x_adc.h"

int main(void)
{
	LED_Init();
	
	while(1)
	{
		LED_BLUE_ON();
	}
}
