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

#include <stm32f4xx.h>
#include "uart1.h"

#ifdef USE_ISR_UART1

    extern u8 rev_buf[REC_BUF_SIZE]; // 接收缓冲区
#endif


int main(void)
{
//	int i = 0;
//	unsigned char rec;

    //设置中断优先级分组配置
    NVIC_SetPriorityGrouping(7-2) ;

   	Uart1_init();

	send_string((u8*)"20190115\n");

    while(1)
    {
#ifdef USE_ISR_UART1
        //3.在循环中处理接收的数据
		if(revice_ok) //接收到数据
		{
			revice_ok = 0; //接收标志清零

			send_string(rev_buf);

        }
#endif
    }

}
