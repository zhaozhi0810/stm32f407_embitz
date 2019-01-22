
/******************************************************************************
 * Copyright (C) 2019, dazhi
 * All rights reserved.
 *
 * �ļ�����: tty.c
 * ժ    Ҫ����ӡ��������

 *
 * ��ǰ�汾: 1.0
 * ��    ��: dazhi
 * �������: 2019-01-15
 *
 ******************************************************************************/

/* Includes ------------------------------------------------------------------*/
#include "tty.h"
//#include "ringbuffer.h"
#include "stm32f4xx.h"
#include <stdarg.h>
#include <stdio.h>
#include <string.h>

static unsigned char rxbuf[TTY_TXBUF_SIZE];         /*���ջ����� ------------*/
static unsigned char txbuf[TTY_RXBUF_SIZE];         /*���ͻ����� ------------*/
static ring_buf_t ringbuf_send, ringbuf_recv;       /*�շ����������� ---------*/


#if TTY_USE_DMA == 1
    static unsigned char dma_tx_buf[TTY_DMA_TX_LEN];/*DMA���ͻ����� ---------*/
    static unsigned char dma_rx_buf[TTY_DMA_RX_LEN];/*DMA���ջ����� ---------*/
#endif

/*******************************************************************************
 * �������ƣ�port_conf
 * ������������ӡ��������(PA9->USART1_TX, PA10->USART1_RX)
 * ���������none
 * �� �� ֵ��none
 * ��    �ߣ�dazhi
 ******************************************************************************/
static void port_conf(void)
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

#ifdef USE_ISR_UART1
    //���������ж�
    USART1->CR1 |= USART_CR1_RXNEIE;

    // �������ȼ�����
    NVIC_SetPriority(USART1_IRQn,NVIC_EncodePriority (7-2, 2,2));
        //NVIC �ж�ʹ��
    NVIC_EnableIRQ(USART1_IRQn);
#endif
}



/*******************************************************************************
 * �������ƣ�DMA_Conf
 * ��������: ����DMA����(DMA2_Channel4_Stream2->USART1_RX,
 *                       DMA2_Channel4_Stream7->USART1_TX)
 * ���������none
 * �� �� ֵ��none
 * ��    �ߣ�dazhi
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
    /*���ô���3������ */
    DMA_Structure.DMA_Channel = DMA_Channel_4;                    /*DMA1ͨ��4*/
    DMA_Structure.DMA_PeripheralBaseAddr = (uint32_t)(&USART3->DR);
    DMA_Structure.DMA_Memory0BaseAddr = (uint32_t)dma_rx_buf;
    DMA_Structure.DMA_DIR = DMA_DIR_PeripheralToMemory;           /*���赽�ڴ�*/
    DMA_Structure.DMA_BufferSize = sizeof(dma_rx_buf);
    DMA_Structure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
    DMA_Structure.DMA_MemoryInc = DMA_MemoryInc_Enable;
    DMA_Structure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;
    DMA_Structure.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;
    DMA_Structure.DMA_Mode = DMA_Mode_Circular;                   /*ѭ��ģʽ*/
    DMA_Structure.DMA_Priority = DMA_Priority_Low;
    DMA_Structure.DMA_FIFOMode = DMA_FIFOMode_Disable;
    DMA_Structure.DMA_FIFOThreshold = DMA_FIFOThreshold_Full;
    DMA_Structure.DMA_MemoryBurst = DMA_MemoryBurst_Single;
    DMA_Structure.DMA_PeripheralBurst = DMA_PeripheralBurst_Single;
    DMA_Init(DMA1_Stream1, &DMA_Structure);

    /*���ô���3������ */
    DMA_Structure.DMA_PeripheralBaseAddr = (uint32_t)(&USART3->DR);
    DMA_Structure.DMA_Memory0BaseAddr = (uint32_t)dma_tx_buf;
    DMA_Structure.DMA_DIR = DMA_DIR_MemoryToPeripheral;            /*�ڴ浽����*/
    DMA_Structure.DMA_BufferSize = sizeof(dma_tx_buf);
    DMA_Structure.DMA_Mode = DMA_Mode_Normal;                      /*����ģʽ -*/
    DMA_Init(DMA1_Stream3, &DMA_Structure);

    /* Enable DMA Stream Transfer Complete interrupt */
    DMA_ITConfig(DMA1_Stream1, DMA_IT_TC, ENABLE);
    //DMA_ITConfig(DMA1_Stream3, DMA_IT_TC, ENABLE);
    /* DMA Stream enable */
    DMA_Cmd(DMA1_Stream1, ENABLE);                                 /*ʹ�ܽ�����*/

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




