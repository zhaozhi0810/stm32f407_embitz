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
#include "stm32f4xx_conf.h"
#include "Queue.h"
#include "BSP_Uart.h"

int main(void)
{
    BSP_UartOpen(COM1,115200,8,1,0);
    while(1)
    {
        BSP_UartWrite(COM1,"20190120\n",9);
    }
}
