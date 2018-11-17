/*
**
**                           Main.c
**
**
**********************************************************************/
/*
   Last committed:     $Revision: 00 $
   Last changed by:    $Author: $
   Last changed date:  $Date:  $
   ID:                 $Id:  $

**********************************************************************/
#include "stm32f4xx.h"


//����1�ĳ�ʼ��
void Uart1_init(void)
{
	//1.ʹ��uart1��ʱ���ź� �� GPIOA��ʱ���ź�
	RCC->APB2ENR |= RCC_APB2ENR_USART1EN;
	RCC->AHB1ENR |= RCC_AHB1ENR_GPIOAEN;

	//2. GPIOA �Ķ˿ڹ������� 9,10
	GPIOA->MODER &= ~(GPIO_MODER_MODER10 | GPIO_MODER_MODER9 );//0x15<<26;   //����Ϊ��©���ģʽ��Ƶ��Ϊ2MHZ
	GPIOA->MODER |= GPIO_MODER_MODER10_1 | GPIO_MODER_MODER9_1 ;  //ͨ�����ģʽ
	//p179 ����Ϊ���ڹ��ܸ���ģʽ
	GPIOA->AFR[1] |= 0x77<<4;

	//3. ����λ��ֹͣλ��������
	USART1->CR1 = USART_CR1_UE | USART_CR1_RE | USART_CR1_TE;//0x200c;  //ʹ��ģ�飬���ͽ���ʹ�ܣ��жϹر�
	USART1->CR2 = 0;  //ֹͣλ1  �������ܽ�ֹ
	//���㲨���� ��stm32f4xx���Ĳο��ֲ�.pdf p686��
	//band = apb2clk / (8*(2-over8) * USARTDIV)  ����band ���Ե���9600/115200(����)
	//115200 = 84000000 / (16*x) --> x = 45.5729
	USART1->BRR = 0x2d9;
}



void uart1_send_byte(unsigned char dat)
{
	while((USART1->SR & USART_SR_TC)==0);//�ȴ����ͽ���
	USART1->DR = dat;   //����ȥ
}


void send_string(char * str)
{
	while(*str)
	{
		uart1_send_byte(*str++);
	}
}



int main(void)
{
	int i = 0;
	unsigned char rec;

	RCC->AHB1ENR |= RCC_AHB1ENR_GPIOEEN;  //GPIOE��ʱ��ʹ��

	//PE13,14,15 ����led
	GPIOE->MODER &= ~(GPIO_MODER_MODER13 | GPIO_MODER_MODER14 |GPIO_MODER_MODER14);//0x15<<26;   //����Ϊ��©���ģʽ��Ƶ��Ϊ2MHZ
	GPIOE->MODER |= GPIO_MODER_MODER13_0 | GPIO_MODER_MODER14_0 | GPIO_MODER_MODER15_0;  //ͨ�����ģʽ


//	GPIOE->OSPEEDR |= GPIO_OSPEEDER_OSPEEDR13_1 | GPIO_OSPEEDER_OSPEEDR14_1 | GPIO_OSPEEDER_OSPEEDR15_1;
	//GPIOx_OTYPER û�����ã���©�����춼û������

	GPIOE->BSRRH = GPIO_BSRR_BS_13 | GPIO_BSRR_BS_14 | GPIO_BSRR_BS_15;    //��4λ����

	Uart1_init();

//	USART1->DR = 'a';
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
		if(USART1->SR & USART_SR_RXNE) //���յ�����
		{
			rec = USART1->DR;   //�������յ�����
			if(rec == '2')
			{
				rec += 1;
			}
			uart1_send_byte(rec);
		}

	}
//	return 0;
}
