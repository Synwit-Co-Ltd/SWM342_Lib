#include "SWM342.h"


int main(void)
{	
	SystemInit();
	
	GPIO_Init(GPIOA, PIN0, 0, 1, 0, 0);			//输入，上拉使能，接KEY
	
	GPIO_Init(GPIOA, PIN5, 1, 0, 0, 0);			//输出， 接LED
	
	while(1==1)
	{
		if(GPIO_GetBit(GPIOA, PIN0) == 0)		//按键被按下
		{
			GPIO_AtomicSetBit(GPIOA, PIN5);
		}
		else
		{
			GPIO_AtomicClrBit(GPIOA, PIN5);
		}
	}
}

