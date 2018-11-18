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



void led_init(void)
{
    //GPIOE 13,14,15 �Ƿֱ�������3��led
    //�͵�ƽ����
	RCC->AHB1ENR |= RCC_AHB1ENR_GPIOEEN;  //GPIOE��ʱ��ʹ��

	//PE13,14,15 ����led
	GPIOE->MODER &= ~(GPIO_MODER_MODER13 | GPIO_MODER_MODER14 |GPIO_MODER_MODER14);//0x15<<26;   //����Ϊ��©���ģʽ��Ƶ��Ϊ2MHZ
	GPIOE->MODER |= GPIO_MODER_MODER13_0 | GPIO_MODER_MODER14_0 | GPIO_MODER_MODER15_0;  //ͨ�����ģʽ

//	GPIOE->OSPEEDR |= GPIO_OSPEEDER_OSPEEDR13_1 | GPIO_OSPEEDER_OSPEEDR14_1 | GPIO_OSPEEDER_OSPEEDR15_1;
	//GPIOx_OTYPER û�����ã���©�����춼û������
	GPIOE->BSRRH = GPIO_BSRR_BS_13 | GPIO_BSRR_BS_14 | GPIO_BSRR_BS_15;    //��13,14,15λ����
}

int main(void)
{
	int i = 0;
	unsigned char rec;

    //�����ж����ȼ���������
    NVIC_SetPriorityGrouping(7-2) ;

    led_init();

	Uart1_init();

	send_string("20181117\n");

	while(1)
	{
		//led��˸
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


		//3.��ѭ���д�����յ�����
		if(revice_ok) //���յ�����
		{
			revice_ok = 0; //���ձ�־����
			send_string(rev_buf);
        }
	}
//	return 0;
}
