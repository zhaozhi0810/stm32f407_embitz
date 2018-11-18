
#include <stm32f4xx.h>

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




