/*
**
**                           Main.c
**
**
**********************************************************************/
/*
   Last committed:     $Revision: 1.0 $
   Last changed by:    $Author: zhaozhi$
   Last changed date:  $Date:  20181118$
   ID:                 $Id:  00001$

**********************************************************************/
#include <stm32f4xx.h>
#include "uart1.h"





int main(void)
{
	int i = 0;
	unsigned char rec;

	RCC->AHB1ENR |= RCC_AHB1ENR_GPIOEEN;  //GPIOE的时钟使能

	//PE13,14,15 三个led
	GPIOE->MODER &= ~(GPIO_MODER_MODER13 | GPIO_MODER_MODER14 |GPIO_MODER_MODER14);//0x15<<26;   //配置为开漏输出模式，频率为2MHZ
	GPIOE->MODER |= GPIO_MODER_MODER13_0 | GPIO_MODER_MODER14_0 | GPIO_MODER_MODER15_0;  //通用输出模式


//	GPIOE->OSPEEDR |= GPIO_OSPEEDER_OSPEEDR13_1 | GPIO_OSPEEDER_OSPEEDR14_1 | GPIO_OSPEEDER_OSPEEDR15_1;
	//GPIOx_OTYPER 没有设置，开漏和推挽都没有问题

	GPIOE->BSRRH = GPIO_BSRR_BS_13 | GPIO_BSRR_BS_14 | GPIO_BSRR_BS_15;    //第4位清零

	Uart1_init();

//	USART1->DR = 'a';
	send_string("20181117\n");

	while(1)
	{
		//led闪烁
		i++;
		if(i<500000)
		{
			GPIOE->BSRRL = GPIO_BSRR_BS_13 | GPIO_BSRR_BS_14 | GPIO_BSRR_BS_15;
		}
		else if(i<1000000)
		{
			GPIOE->BSRRH = GPIO_BSRR_BS_13 | GPIO_BSRR_BS_14 | GPIO_BSRR_BS_15;
		}
		else
		{
			i = 0;
		}


		//3.在循环中处理接收的数据
		if(USART1->SR & USART_SR_RXNE) //接收到数据
		{
			rec = USART1->DR;   //读出接收的数据
			if(rec == '2')
			{
				rec += 1;
			}
            uart1_send_byte(rec);
        }
	}
//	return 0;
}
