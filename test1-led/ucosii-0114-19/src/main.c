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
#include "stm32f4xx.h"
#include "ucos_ii.h"
#include "app_cfg.h"
#include "stdio.h"

static  OS_STK App_TaskStartStk[APP_TASK_START_STK_SIZE];	            //开始任务对应的堆栈
static  OS_STK App_Task1Stk[APP_TASK_1_STK_SIZE];			            //第一个任务对应的堆栈
static  OS_STK Led2_TaskStk[LED2_TASK_STK_SIZE];			            //第一个任务对应的堆栈


static  void  App_TaskStart(void *p_arg);                               //开始任务
static  void  App_TaskCreate(void);							            //创建子任务函数
static  void  App_Task1(void *p_arg);						            //第一个任务
static  void  Led2_Task(void *p_arg);						            //第一个任务








/*********************************************************************************
*描述：读取CPU的寄存器来确定CPU时钟频率的芯片
*
*返回值：CPU的时钟频率，单位为Hz。
**********************************************************************************/
INT32U  BSP_CPU_ClkFreq (void)
{
//    RCC_ClocksTypeDef  rcc_clocks;


//    RCC_GetClocksFreq(&rcc_clocks);

//    return ((INT32U)rcc_clocks.HCLK_Frequency);
    return 168000000;
}

/*********************************************************************************
*描述：系统滴答的时钟频率
*
*返回值：时钟频率（系统时钟周期）。
**********************************************************************************/
INT32U  OS_CPU_SysTickClkFreq (void)
{
    INT32U  freq;


    freq = BSP_CPU_ClkFreq();
    return (freq);
}


void led_init(void)
{
    //GPIOE 13,14,15 是分别连接了3个led
    //低电平点亮
	RCC->AHB1ENR |= RCC_AHB1ENR_GPIOEEN;  //GPIOE的时钟使能

	//PE13,14,15 三个led
	GPIOE->MODER &= ~(GPIO_MODER_MODER13 | GPIO_MODER_MODER14 |GPIO_MODER_MODER14);//0x15<<26;   //配置为开漏输出模式，频率为2MHZ
	GPIOE->MODER |= GPIO_MODER_MODER13_0 | GPIO_MODER_MODER14_0 | GPIO_MODER_MODER15_0;  //通用输出模式

//	GPIOE->OSPEEDR |= GPIO_OSPEEDER_OSPEEDR13_1 | GPIO_OSPEEDER_OSPEEDR14_1 | GPIO_OSPEEDER_OSPEEDR15_1;
	//GPIOx_OTYPER 没有设置，开漏和推挽都没有问题
	GPIOE->BSRRL = GPIO_BSRR_BS_13 | GPIO_BSRR_BS_14 | GPIO_BSRR_BS_15;    //第13,14,15位清零
}


/*************************************************************************************
  * 函数名称：main()
  * 参数    ：void
  * 返回值  ：void
  * 描述    ：程序主入口main函数
**************************************************************************************/
int main(void)
{
    led_init();
    OSInit();                                                           //初始化OS内核
    OSTaskCreate((void (*)(void *)) App_TaskStart,     				    //建立开始任务
                             (void          * ) 0,
                             (OS_STK        * )&App_TaskStartStk[APP_TASK_START_STK_SIZE - 1],
                             (INT8U           ) APP_TASK_START_PRIO);
#if (OS_TASK_NAME_SIZE >= 11)
    OSTaskNameSet(APP_TASK_START_PRIO, (CPU_INT08U *)"Start Task", &os_err);
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
    OSStatInit();                                               // Determine CPU capacity.
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


    OSTaskCreate((void (*)(void *)) Led2_Task,         //Create the start task
                         (void          * ) 0,
                         (OS_STK        * )&Led2_TaskStk[LED2_TASK_STK_SIZE - 1],
                         (INT8U           ) LED2_TASK_PRIO);
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
        OSTimeDly(500);
        GPIOE->BSRRH = GPIO_BSRR_BS_13;
        OSTimeDly(500);
        printf("12345\n");
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

	while(1)
    {
        GPIOE->BSRRL = GPIO_BSRR_BS_14;
        OSTimeDly(100);
        GPIOE->BSRRH = GPIO_BSRR_BS_14;
        OSTimeDly(100);
    }
}


