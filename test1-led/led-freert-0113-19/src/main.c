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
#include "FreeRTOS.h"
#include "task.h"
#include <stdio.h>
/********************************************//**
测试： 创建4个任务

start_task() : 用来创建其他3个任务
led0_task(): 用来控制led0的闪烁
led1_task(): 用来控制led1的闪烁
float_task(): 用来简单的浮点测试

 ***********************************************/

#define START_TASK_PRIO   1    //任务优先级
#define START_STK_SIZE    128  //任务堆栈大小
TaskHandle_t StartTask_Handler;
void Start_task(void* pvParameters);

#define LED0_TASK_PRIO   2    //任务优先级
#define LED0_STK_SIZE    128  //任务堆栈大小
TaskHandle_t Led0Task_Handler;
void Led0_task(void* pvParameters);


#define LED1_TASK_PRIO   3    //任务优先级
#define LED1_STK_SIZE    128  //任务堆栈大小
TaskHandle_t Led1Task_Handler;
void Led1_task(void* pvParameters);

#define FLOAT_TASK_PRIO   4    //任务优先级
#define FLOAT_STK_SIZE    128  //任务堆栈大小
TaskHandle_t FloatTask_Handler;
void Float_task(void* pvParameters);




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


void Led0_task(void* pvParameters)
{
    while(1)
    {
        GPIOE->BSRRL = GPIO_BSRR_BS_13;
        vTaskDelay(5000);
        GPIOE->BSRRH = GPIO_BSRR_BS_13;
        vTaskDelay(5000);
    }
}


void Led1_task(void* pvParameters)
{
    while(1)
    {
        GPIOE->BSRRL = GPIO_BSRR_BS_14;
        vTaskDelay(2000);
        GPIOE->BSRRH = GPIO_BSRR_BS_14;
        vTaskDelay(2000);
    }
}


void Float_task(void* pvParameters)
{
    static float float_num = 0.0;

    while(1)
    {
        float_num += 0.01f;
        printf("float_num = %.4f\n",float_num);
        vTaskDelay(1000);
    }
}


void Start_task(void* pvParameters)
{
    taskENTER_CRITICAL();   //进入临界区

    xTaskCreate((TaskFunction_t) Led0_task,         //任务函数
            (const char*) "led0_task",          //任务名称
            (uint16_t ) LED0_STK_SIZE,          //任务堆栈大小
            (void *) NULL,                       //传递给任务函数的参数
            (UBaseType_t) LED0_TASK_PRIO,       //任务优先级
            (TaskHandle_t *)&Led0Task_Handler   //任务句柄
            );


    xTaskCreate((TaskFunction_t) Led1_task,         //任务函数
            (const char*) "led1_task",          //任务名称
            (uint16_t ) LED1_STK_SIZE,          //任务堆栈大小
            (void *) NULL,                       //传递给任务函数的参数
            (UBaseType_t) LED1_TASK_PRIO,       //任务优先级
            (TaskHandle_t *)&Led1Task_Handler   //任务句柄
            );


    xTaskCreate((TaskFunction_t) Float_task,         //任务函数
            (const char*) "float_task",          //任务名称
            (uint16_t ) FLOAT_STK_SIZE,          //任务堆栈大小
            (void *) NULL,                       //传递给任务函数的参数
            (UBaseType_t) FLOAT_TASK_PRIO,       //任务优先级
            (TaskHandle_t *)&FloatTask_Handler   //任务句柄
            );

    vTaskDelete(StartTask_Handler);
    taskEXIT_CRITICAL();    //退出临界区
}





int main(void)
{
    led_init();

    xTaskCreate((TaskFunction_t) Start_task,         //任务函数
                (const char*) "start_task",          //任务名称
                (uint16_t ) START_STK_SIZE,          //任务堆栈大小
                (void *) NULL,                       //传递给任务函数的参数
                (UBaseType_t) START_TASK_PRIO,       //任务优先级
                (TaskHandle_t *)&StartTask_Handler   //任务句柄
                );

    vTaskStartScheduler();  //开始任务调度
}
