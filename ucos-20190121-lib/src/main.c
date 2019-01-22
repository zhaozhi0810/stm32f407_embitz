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

#include "stm32f4xx.h"
#include "ucos_ii.h"
#include "app_cfg.h"
#include "stdio.h"
#include "print_float.h"


static  OS_STK App_TaskStartStk[APP_TASK_START_STK_SIZE];	            //��ʼ�����Ӧ�Ķ�ջ
static  OS_STK App_Task1Stk[APP_TASK_1_STK_SIZE];			            //��һ�������Ӧ�Ķ�ջ
static  OS_STK Led2_TaskStk[LED2_TASK_STK_SIZE];			            //��һ�������Ӧ�Ķ�ջ
static  OS_STK Float_TaskStk[APP_TASK_Printf_STK_SIZE];			            //��һ�������Ӧ�Ķ�ջ

static  void  App_TaskStart(void *p_arg);                               //��ʼ����
static  void  App_TaskCreate(void);							            //������������
static  void  App_Task1(void *p_arg);						            //��һ������
static  void  Led2_Task(void *p_arg);						            //��һ������
static  void  Float_print_Task(void *p_arg);                            //��������ӡ����

/*********************************************************************************
*��������ȡCPU�ļĴ�����ȷ��CPUʱ��Ƶ�ʵ�оƬ
*
*����ֵ��CPU��ʱ��Ƶ�ʣ���λΪHz��
**********************************************************************************/
INT32U  BSP_CPU_ClkFreq (void)
{
    RCC_ClocksTypeDef  rcc_clocks;

    RCC_GetClocksFreq(&rcc_clocks);

    return ((INT32U)rcc_clocks.HCLK_Frequency);
}

/*********************************************************************************
*������ϵͳ�δ��ʱ��Ƶ��
*
*����ֵ��ʱ��Ƶ�ʣ�ϵͳʱ�����ڣ���
**********************************************************************************/
INT32U  OS_CPU_SysTickClkFreq (void)
{
    INT32U  freq;

    freq = BSP_CPU_ClkFreq();
    return (freq);
}



/*************************************************************************************
  * �������ƣ�main()
  * ����    ��void
  * ����ֵ  ��void
  * ����    �����������main����
**************************************************************************************/
int main(void)
{
    BSP_LedOpen();
    BSP_UartOpen(COM1,  115200, 8, 1, 0);

    OSInit();                                                           //��ʼ��OS�ں�
    OSTaskCreate((void (*)(void *)) App_TaskStart,     				    //������ʼ����
                             (void          * ) 0,
                             (OS_STK        * )&App_TaskStartStk[APP_TASK_START_STK_SIZE - 1],
                             (INT8U           ) APP_TASK_START_PRIO);
#if (OS_TASK_NAME_SIZE >= 11)
    OSTaskNameSet(APP_TASK_START_PRIO, (CPU_INT08U *)"Start Task", &os_err);
#endif
    OSStart();                                                  //��ʼ�������
    return (0);
}
//��ʼ������
static  void  App_TaskStart (void *p_arg)
{
    (void)p_arg;                                                //Initialize BSP functions.
	//��ʼ��ϵͳ����ʱ��
    OS_CPU_SysTickInit();
	//ͳ������
#if (OS_TASK_STAT_EN > 0)
    OSStatInit();                                               // Determine CPU capacity.
#endif

	/* �ڴ˿ɴ����������� */
    App_TaskCreate();	                                        //������һ������
//	BSP_Init();	                                                //����ĳ�ʼ��
    while (1)
	{
		OSTaskSuspend(OS_PRIO_SELF);  // ��������
    }
}

/*************************************************************************************
  * �������ƣ�App_TaskCreate()
  * ����    ��void
  * ����ֵ  ��void
  * ����    ����������
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

    OSTaskCreate((void (*)(void *)) Float_print_Task,         //Create float print task
                         (void          * ) 0,
                         (OS_STK        * )&Float_TaskStk[APP_TASK_Printf_STK_SIZE- 1],
                         (INT8U           ) APP_TASK_Printf_PRIO);

}
/*************************************************************************************
  * �������ƣ�App_Task1()
  * ����    ��void
  * ����ֵ  ��void
  * ����    ����һ������ִ�г���
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
        printf("App_Task1-0121pm\n\r");
//        BSP_UartWrite(COM1,(uint8_t *)"App_Task1\n",10);
    }
}


/*************************************************************************************
  * �������ƣ�Led2_Task()
  * ����    ��void
  * ����ֵ  ��void
  * ����    ����һ������ִ�г���
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
        printf("Led2_Task---0121\n\r");
 //       BSP_UartWrite(COM1,(uint8_t *)"Led2_Task\n",10);
    }
}





/*************************************************************************************
  * �������ƣ�Led2_Task()
  * ����    ��void
  * ����ֵ  ��void
  * ����    ����һ������ִ�г���
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
//        PrintFloat(num);

        OSTimeDly(400);
    }
}


