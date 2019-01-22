
/******************************************************************************
 * Copyright (C) 2019, dazhi
 * All rights reserved.
 *
 * 文件名称: tty.c
 * 摘    要：打印串口驱动

 *
 * 当前版本: 1.0
 * 作    者: dazhi
 * 完成日期: 2019-01-15
 *
 ******************************************************************************/

/* Includes ------------------------------------------------------------------*/
#include "tty.h"
//#include "ringbuffer.h"
#include "stm32f4xx.h"
#include <stdarg.h>
#include <stdio.h>
#include <string.h>

static unsigned char rxbuf[TTY_TXBUF_SIZE];         /*接收缓冲区 ------------*/
static unsigned char txbuf[TTY_RXBUF_SIZE];         /*发送缓冲区 ------------*/
static ring_buf_t ringbuf_send, ringbuf_recv;       /*收发缓冲区管理 ---------*/


#if TTY_USE_DMA == 1
    static unsigned char dma_tx_buf[TTY_DMA_TX_LEN];/*DMA发送缓冲区 ---------*/
    static unsigned char dma_rx_buf[TTY_DMA_RX_LEN];/*DMA接收缓冲区 ---------*/
#endif

/*******************************************************************************
 * 函数名称：port_conf
 * 功能描述：打印串口配置(PA9->USART1_TX, PA10->USART1_RX)
 * 输入参数：none
 * 返 回 值：none
 * 作    者：dazhi
 ******************************************************************************/
static void port_conf(void)
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



/*******************************************************************************
 * 函数名称：DMA_Conf
 * 功能描述: 串口DMA配置(DMA2_Channel4_Stream2->USART1_RX,
 *                       DMA2_Channel4_Stream7->USART1_TX)
 * 输入参数：none
 * 返 回 值：none
 * 作    者：dazhi
 ******************************************************************************/
#if TTY_USE_DMA == 1
static void DMA_Conf(void)
{
    DMA_InitTypeDef DMA_Structure;
    NVIC_InitTypeDef NVIC_InitStructure;

    /* Enable DMA clock */
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_DMA1, ENABLE);
    DMA_DeInit(DMA1_Stream1);
    DMA_DeInit(DMA1_Stream3);
    while (DMA_GetCmdStatus(DMA1_Stream1) != DISABLE){}
    while (DMA_GetCmdStatus(DMA1_Stream3) != DISABLE){}
    /*配置串口3接收流 */
    DMA_Structure.DMA_Channel = DMA_Channel_4;                    /*DMA1通道4*/
    DMA_Structure.DMA_PeripheralBaseAddr = (uint32_t)(&USART3->DR);
    DMA_Structure.DMA_Memory0BaseAddr = (uint32_t)dma_rx_buf;
    DMA_Structure.DMA_DIR = DMA_DIR_PeripheralToMemory;           /*外设到内存*/
    DMA_Structure.DMA_BufferSize = sizeof(dma_rx_buf);
    DMA_Structure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
    DMA_Structure.DMA_MemoryInc = DMA_MemoryInc_Enable;
    DMA_Structure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;
    DMA_Structure.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;
    DMA_Structure.DMA_Mode = DMA_Mode_Circular;                   /*循环模式*/
    DMA_Structure.DMA_Priority = DMA_Priority_Low;
    DMA_Structure.DMA_FIFOMode = DMA_FIFOMode_Disable;
    DMA_Structure.DMA_FIFOThreshold = DMA_FIFOThreshold_Full;
    DMA_Structure.DMA_MemoryBurst = DMA_MemoryBurst_Single;
    DMA_Structure.DMA_PeripheralBurst = DMA_PeripheralBurst_Single;
    DMA_Init(DMA1_Stream1, &DMA_Structure);

    /*配置串口3发送流 */
    DMA_Structure.DMA_PeripheralBaseAddr = (uint32_t)(&USART3->DR);
    DMA_Structure.DMA_Memory0BaseAddr = (uint32_t)dma_tx_buf;
    DMA_Structure.DMA_DIR = DMA_DIR_MemoryToPeripheral;            /*内存到外设*/
    DMA_Structure.DMA_BufferSize = sizeof(dma_tx_buf);
    DMA_Structure.DMA_Mode = DMA_Mode_Normal;                      /*正常模式 -*/
    DMA_Init(DMA1_Stream3, &DMA_Structure);

    /* Enable DMA Stream Transfer Complete interrupt */
    DMA_ITConfig(DMA1_Stream1, DMA_IT_TC, ENABLE);
    //DMA_ITConfig(DMA1_Stream3, DMA_IT_TC, ENABLE);
    /* DMA Stream enable */
    DMA_Cmd(DMA1_Stream1, ENABLE);                                 /*使能接收流*/

    /* Enable the DMA Stream IRQ Channel */
    NVIC_InitStructure.NVIC_IRQChannel = DMA1_Stream1_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannel = DMA1_Stream3_IRQn;
    NVIC_Init(&NVIC_InitStructure);
}
#endif




