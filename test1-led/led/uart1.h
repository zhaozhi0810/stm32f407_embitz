



#ifndef __UART1_H__
#define __UART1_H__


#define USE_ISR_UART1   //串口1使用中断

void Uart1_init(void);
void uart1_send_byte(unsigned char dat);
void send_string(char * str);






#endif

