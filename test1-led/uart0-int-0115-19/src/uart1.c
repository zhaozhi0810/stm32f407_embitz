
#include <stm32f4xx.h>
#include "uart1.h"


//串口1的初始化
void Uart1_init(void)
{
	//1.使能uart1的时钟信号 和 GPIOA的时钟信号
	RCC->APB2ENR |= RCC_APB2ENR_USART1EN;
	RCC->AHB1ENR |= RCC_AHB1ENR_GPIOAEN;

	//2. GPIOA 的端口功能设置 9,10
	GPIOA->MODER &= ~(GPIO_MODER_MODER10 | GPIO_MODER_MODER9 );//0x15<<26;   //配置为开漏输出模式，频率为2MHZ
	GPIOA->MODER |= GPIO_MODER_MODER10_1 | GPIO_MODER_MODER9_1 ;  //通用输出模式
	//p179 设置为串口功能复用模式
	GPIOA->AFR[1] |= 0x77<<4;

	//3. 数据位，停止位，波特率
	USART1->CR1 = USART_CR1_UE | USART_CR1_RE | USART_CR1_TE;//0x200c;  //使能模块，发送接收使能，中断关闭


	USART1->CR2 = 0;  //停止位1  其他功能禁止



	//计算波特率 《stm32f4xx中文参考手册.pdf p686》
	//band = apb2clk / (8*(2-over8) * USARTDIV)  其中band 可以等于9600/115200(常用)
	//115200 = 84000000 / (16*x) --> x = 45.5729
	USART1->BRR = 0x2d9;

#ifdef USE_ISR_UART1
    //开启接收中断
    USART1->CR1 |= USART_CR1_RXNEIE;

    // 设置优先级分组
    NVIC_SetPriority(USART1_IRQn,NVIC_EncodePriority (7-2, 2,2));
        //NVIC 中断使能
    NVIC_EnableIRQ(USART1_IRQn);
#endif

}



#ifdef USE_ISR_UART1

u8 rev_buf[REC_BUF_SIZE]; // 接收缓冲区
u8 rec_cont=0;     // 地址偏移量
u8 revice_ok=0; // 接收完成标志


//接收中断函数
void USART1_IRQHandler(void)
{
    u8 data;

    data=USART1->DR;// 读取同时会清除标志
    if(data=='\n')// 接收完成
    {
            rev_buf[rec_cont]='\0';
            rec_cont=0;
            revice_ok=1;
    }
    else// 正常接收
    {
            rev_buf[rec_cont++]=data;
            if(rec_cont > REC_BUF_SIZE-1) //防止计数越界
            {
                rec_cont=0;
            }
    }
}
#endif




void uart1_send_byte(u8 dat)
{
	while((USART1->SR & USART_SR_TC)==0);//等待发送结束
	USART1->DR = dat;   //发出去
}


void send_string(u8 * str)
{
	while(*str)
	{
		uart1_send_byte(*str++);
	}
}




