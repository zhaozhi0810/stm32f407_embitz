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

    extern u8 rev_buf[REC_BUF_SIZE]; // ���ջ�����
#endif


int main(void)
{
//	int i = 0;
//	unsigned char rec;

    //�����ж����ȼ���������
    NVIC_SetPriorityGrouping(7-2) ;

   	Uart1_init();

	send_string((u8*)"20190115\n");

    while(1)
    {
#ifdef USE_ISR_UART1
        //3.��ѭ���д�����յ�����
		if(revice_ok) //���յ�����
		{
			revice_ok = 0; //���ձ�־����

			send_string(rev_buf);

        }
#endif
    }

}
