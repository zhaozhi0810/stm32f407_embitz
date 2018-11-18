



#ifndef __UART1_H__
#define __UART1_H__

#define USE_ISR_UART1

void Uart1_init(void);
void uart1_send_byte(unsigned char dat);
void send_string(char * str);

#ifdef USE_ISR_UART1
#define REC_BUF_SIZE 32
    extern u8 rev_buf[REC_BUF_SIZE]; // 接收缓冲区
    extern u8 rec_cont;     // 地址偏移量
    extern u8 revice_ok; // 接收完成标志


    //接收中断函数
    void USART1_IRQHandler(void);
#endif


#endif

