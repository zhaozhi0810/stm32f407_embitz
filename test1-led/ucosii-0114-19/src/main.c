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

static  OS_STK App_TaskStartStk[APP_TASK_START_STK_SIZE];	            //��ʼ�����Ӧ�Ķ�ջ
static  OS_STK App_Task1Stk[APP_TASK_1_STK_SIZE];			            //��һ�������Ӧ�Ķ�ջ
static  OS_STK Led2_TaskStk[LED2_TASK_STK_SIZE];			            //��һ�������Ӧ�Ķ�ջ


static  void  App_TaskStart(void *p_arg);                               //��ʼ����
static  void  App_TaskCreate(void);							            //������������
static  void  App_Task1(void *p_arg);						            //��һ������
static  void  Led2_Task(void *p_arg);						            //��һ������








/*********************************************************************************
*��������ȡCPU�ļĴ�����ȷ��CPUʱ��Ƶ�ʵ�оƬ
*
*����ֵ��CPU��ʱ��Ƶ�ʣ���λΪHz��
**********************************************************************************/
INT32U  BSP_CPU_ClkFreq (void)
{
//    RCC_ClocksTypeDef  rcc_clocks;


//    RCC_GetClocksFreq(&rcc_clocks);

//    return ((INT32U)rcc_clocks.HCLK_Frequency);
    return 168000000;
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


void led_init(void)
{
    //GPIOE 13,14,15 �Ƿֱ�������3��led
    //�͵�ƽ����
	RCC->AHB1ENR |= RCC_AHB1ENR_GPIOEEN;  //GPIOE��ʱ��ʹ��

	//PE13,14,15 ����led
	GPIOE->MODER &= ~(GPIO_MODER_MODER13 | GPIO_MODER_MODER14 |GPIO_MODER_MODER14);//0x15<<26;   //����Ϊ��©���ģʽ��Ƶ��Ϊ2MHZ
	GPIOE->MODER |= GPIO_MODER_MODER13_0 | GPIO_MODER_MODER14_0 | GPIO_MODER_MODER15_0;  //ͨ�����ģʽ

//	GPIOE->OSPEEDR |= GPIO_OSPEEDER_OSPEEDR13_1 | GPIO_OSPEEDER_OSPEEDR14_1 | GPIO_OSPEEDER_OSPEEDR15_1;
	//GPIOx_OTYPER û�����ã���©�����춼û������
	GPIOE->BSRRL = GPIO_BSRR_BS_13 | GPIO_BSRR_BS_14 | GPIO_BSRR_BS_15;    //��13,14,15λ����
}


/*************************************************************************************
  * �������ƣ�main()
  * ����    ��void
  * ����ֵ  ��void
  * ����    �����������main����
**************************************************************************************/
int main(void)
{
    led_init();
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
        printf("12345\n");
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
    }
}


