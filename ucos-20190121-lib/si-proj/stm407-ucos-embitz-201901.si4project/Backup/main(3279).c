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
#include "BSP_Led.h"
#include "BSP_I2c.h"
#include "bsp.h"
#include "bsp_os.h"
#include "stm32f4xx.h"
#include "ucos_ii.h"
#include "app_cfg.h"
#include "stdio.h"

#include <string.h>


static  OS_STK App_TaskStartStk[APP_TASK_START_STK_SIZE];	            //开始任务对应的堆栈
static  OS_STK App_Task1Stk[APP_TASK_1_STK_SIZE];			            //第一个任务对应的堆栈
static  OS_STK Led2_TaskStk[LED2_TASK_STK_SIZE];			            //第一个任务对应的堆栈
static  OS_STK Float_TaskStk[APP_TASK_Printf_STK_SIZE];			            //第一个任务对应的堆栈
static  OS_STK Tty_TaskStk[TTY_TASK_STK_SIZE];			            //第一个任务对应的堆栈
static  OS_STK Print_TaskStk[PRINT_TASK_STK_SIZE];			            //第一个任务对应的堆栈

static  void  App_TaskStart(void *p_arg);                               //开始任务
static  void  App_TaskCreate(void);							            //创建子任务函数
static  void  App_Task1(void *p_arg);						            //第一个任务
static  void  Led2_Task(void *p_arg);						            //第一个任务
static  void  Float_print_Task(void *p_arg);                            //浮点数打印任务
static  void  I2C_Task(void *p_arg);


//打印任务，打印缓存中的数据
static  void  print_Task(void *p_arg);



static  void  tty_Task(void *p_arg);                            //浮点数打印任务

/*********************************************************************************
*描述：读取CPU的寄存器来确定CPU时钟频率的芯片
*
*返回值：CPU的时钟频率，单位为Hz。
*********************************************************************************
INT32U  BSP_CPU_ClkFreq (void)
{
    RCC_ClocksTypeDef  rcc_clocks;

    RCC_GetClocksFreq(&rcc_clocks);

    return ((INT32U)rcc_clocks.HCLK_Frequency);
}
*/




/*************************************************************************************
  * 函数名称：main()
  * 参数    ：void
  * 返回值  ：void
  * 描述    ：程序主入口main函数
**************************************************************************************/
int main(void)
{
//    BSP_LedOpen();
    BSP_Init();


    OSInit();                                                           //初始化OS内核
    OSTaskCreate((void (*)(void *)) App_TaskStart,     				    //建立开始任务
                             (void          * ) 0,
                             (OS_STK        * )&App_TaskStartStk[APP_TASK_START_STK_SIZE - 1],
                             (INT8U           ) APP_TASK_START_PRIO);
#if (OS_TASK_NAME_SIZE >= 11)
    OSTaskNameSet(APP_TASK_START_PRIO, (INT8U *)"Start Task", &os_err);
#endif
    OSStart();                                                  //开始任务调度
    return (0);
}
//开始任务函数
static  void  App_TaskStart (void *p_arg)
{
    (void)p_arg;                                                //Initialize BSP functions.
	//初始化系统节拍时钟
    OS_CPU_SysTickInit();
	//统计任务
#if (OS_TASK_STAT_EN > 0)
    OSStatInit();                                               // Determine CPU capacity.   算出最大的空闲计数值
                                                                     //其他任务都没有创建，只有空闲任务和统计任务
#endif

	/* 在此可创建其他任务 */
    App_TaskCreate();	                                        //建立第一个任务
//	BSP_Init();	                                                //外设的初始化
    while (1)
	{
		OSTaskSuspend(OS_PRIO_SELF);  // 挂起任务
    }
}

/*************************************************************************************
  * 函数名称：App_TaskCreate()
  * 参数    ：void
  * 返回值  ：void
  * 描述    ：创建任务
***************************************************************************************/
static  void App_TaskCreate(void)
{
    OSTaskCreate((void (*)(void *)) App_Task1,         //Create the start task
                             (void          * ) 0,
                             (OS_STK        * )&App_Task1Stk[APP_TASK_1_STK_SIZE - 1],
                             (INT8U           ) APP_TASK_1_PRIO);


    OSTaskCreate((void (*)(void *)) I2C_Task,         //Create the start task
                         (void          * ) 0,
                         (OS_STK        * )&Led2_TaskStk[LED2_TASK_STK_SIZE - 1],
                         (INT8U           ) LED2_TASK_PRIO);

    OSTaskCreate((void (*)(void *)) Float_print_Task,         //Create float print task
                         (void          * ) 0,
                         (OS_STK        * )&Float_TaskStk[APP_TASK_Printf_STK_SIZE- 1],
                         (INT8U           ) APP_TASK_Printf_PRIO);


    OSTaskCreate((void (*)(void *)) tty_Task,         //Create 串口终端接收处理
                         (void          * ) 0,
                         (OS_STK        * )&Tty_TaskStk[TTY_TASK_STK_SIZE- 1],
                         (INT8U           ) TTY_TASK_PRIO);



    OSTaskCreate((void (*)(void *)) print_Task,         //Create 串口终端接收处理
                         (void          * ) 0,
                         (OS_STK        * )&Print_TaskStk[PRINT_TASK_STK_SIZE- 1],
                         (INT8U           ) PRINT_TASK_PRIO);


}
/*************************************************************************************
  * 函数名称：App_Task1()
  * 参数    ：void
  * 返回值  ：void
  * 描述    ：第一个任务执行程序
***************************************************************************************/
static  void  App_Task1(void *p_arg)
{
	(void)p_arg;

	while(1)
    {
        GPIOE->BSRRL = GPIO_BSRR_BS_13;
        OSTimeDly(OS_TICKS_PER_SEC / 2u);
        GPIOE->BSRRH = GPIO_BSRR_BS_13;
        OSTimeDly(OS_TICKS_PER_SEC / 2u);
        printf("App_Task1-0121pm\n\r");
//        BSP_UartWrite(COM1,(uint8_t *)"App_Task1\n",10);
    }
}


/*************************************************************************************
  * 函数名称：Led2_Task()
  * 参数    ：void
  * 返回值  ：void
  * 描述    ：第一个任务执行程序
***************************************************************************************/
static  void  Led2_Task(void *p_arg)
{
	(void)p_arg;
	int i = 0;

	while(1)
    {
//        GPIOE->BSRRL = GPIO_BSRR_BS_14;
//        OSTimeDly(OS_TICKS_PER_SEC / 10u);
//        GPIOE->BSRRH = GPIO_BSRR_BS_14;
        BSP_LED_Toggle(LED2);
        OSTimeDly(OS_TICKS_PER_SEC / 10u);
 //       i++;
        printf("Led2_Task---0121\r");
        printf("i = %d\r",i++);
        printf("Led2_Task---0131\r");

 //       BSP_UartWrite(COM1,(uint8_t *)"Led2_Task\n",10);
    }
}





/*************************************************************************************
  * 函数名称：Led2_Task()
  * 参数    ：void
  * 返回值  ：void
  * 描述    ：第一个任务执行程序
***************************************************************************************/
static  void  Float_print_Task(void *p_arg)
{
    float num = 1.11f;
//    OS_CPU_SR cpu_sr;
    int x = 0;
	(void)p_arg;

	while(1)
    {
        num += 0.1f;
        x += 1;
        printf("num = %f\n\r",x+num);

        OSTimeDly(OS_TICKS_PER_SEC *4);
    }
}



/*************************************************************************************
  * 函数名称：串口终端接收处理函数()
  * 参数    ：void
  * 返回值  ：void
  * 描述    ：第一个任务执行程序
***************************************************************************************/
static  void  tty_Task(void *p_arg)
{
    char rec_buf[128];
    int ret;

	(void)p_arg;

	while(1)
    {
        memset(rec_buf,0,128);
        ret =  uart1_read (rec_buf,128);

        if(ret)
        {
            //忽略空回车
            if(strcmp(rec_buf,"\n\r") == 0 || strcmp(rec_buf,"\r\n") == 0 ||
                strcmp(rec_buf,"\r") == 0 || strcmp(rec_buf,"\n") == 0)
            {
                OSTimeDly(OS_TICKS_PER_SEC / 2u);
                continue;
            }
//            printf("ret = %d\n",ret);
            printf("buf = %s\n",rec_buf);   //接收的命令打印出来
        }
        OSTimeDly(OS_TICKS_PER_SEC / 2u);
    }
}






//打印任务，打印缓存中的数据
static  void  print_Task(void *p_arg)
{
    (void)p_arg;

	while(1)
    {
        send_uart1();
        OSTimeDly(OS_TICKS_PER_SEC / 10u);
    }
}


void print_charbuf(INT8U arr[],int len)
{
    int i;

    for(i=0;i<len;i++)
    {
        printf("%x ",arr[i]);
    }
    printf("\r");
}


/*************************************************************************************
  * 函数名称：I2C_Task()   24c02
  * 参数    ：void
  * 返回值  ：void
  * 描述    ：24c02程序
***************************************************************************************/
static  void  I2C_Task(void *p_arg)
{
	(void)p_arg;
	int ret ;

    INT8U buf[16];

    //1.初始化I2C接口
    BSP_I2cOpen(I2C_1,100000);

    ret = BSP_I2cWrite(I2C_1,"20190202--zhaozhi",0,10,17);
	printf("i2c write  ret = %d\r",ret);


	while(1)
    {
        memset(buf,0,16);
        ret = BSP_I2cRead(I2C_1,buf,0,10,16);
		printf("i2c read  ret = %d\r",ret);
        print_charbuf(buf,16);
        BSP_OS_TimeDlyMs(1000);

    }
}




