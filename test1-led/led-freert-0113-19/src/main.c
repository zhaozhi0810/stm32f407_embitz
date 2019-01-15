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
���ԣ� ����4������

start_task() : ������������3������
led0_task(): ��������led0����˸
led1_task(): ��������led1����˸
float_task(): �����򵥵ĸ������

 ***********************************************/

#define START_TASK_PRIO   1    //�������ȼ�
#define START_STK_SIZE    128  //�����ջ��С
TaskHandle_t StartTask_Handler;
void Start_task(void* pvParameters);

#define LED0_TASK_PRIO   2    //�������ȼ�
#define LED0_STK_SIZE    128  //�����ջ��С
TaskHandle_t Led0Task_Handler;
void Led0_task(void* pvParameters);


#define LED1_TASK_PRIO   3    //�������ȼ�
#define LED1_STK_SIZE    128  //�����ջ��С
TaskHandle_t Led1Task_Handler;
void Led1_task(void* pvParameters);

#define FLOAT_TASK_PRIO   4    //�������ȼ�
#define FLOAT_STK_SIZE    128  //�����ջ��С
TaskHandle_t FloatTask_Handler;
void Float_task(void* pvParameters);




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
    taskENTER_CRITICAL();   //�����ٽ���

    xTaskCreate((TaskFunction_t) Led0_task,         //������
            (const char*) "led0_task",          //��������
            (uint16_t ) LED0_STK_SIZE,          //�����ջ��С
            (void *) NULL,                       //���ݸ��������Ĳ���
            (UBaseType_t) LED0_TASK_PRIO,       //�������ȼ�
            (TaskHandle_t *)&Led0Task_Handler   //������
            );


    xTaskCreate((TaskFunction_t) Led1_task,         //������
            (const char*) "led1_task",          //��������
            (uint16_t ) LED1_STK_SIZE,          //�����ջ��С
            (void *) NULL,                       //���ݸ��������Ĳ���
            (UBaseType_t) LED1_TASK_PRIO,       //�������ȼ�
            (TaskHandle_t *)&Led1Task_Handler   //������
            );


    xTaskCreate((TaskFunction_t) Float_task,         //������
            (const char*) "float_task",          //��������
            (uint16_t ) FLOAT_STK_SIZE,          //�����ջ��С
            (void *) NULL,                       //���ݸ��������Ĳ���
            (UBaseType_t) FLOAT_TASK_PRIO,       //�������ȼ�
            (TaskHandle_t *)&FloatTask_Handler   //������
            );

    vTaskDelete(StartTask_Handler);
    taskEXIT_CRITICAL();    //�˳��ٽ���
}





int main(void)
{
    led_init();

    xTaskCreate((TaskFunction_t) Start_task,         //������
                (const char*) "start_task",          //��������
                (uint16_t ) START_STK_SIZE,          //�����ջ��С
                (void *) NULL,                       //���ݸ��������Ĳ���
                (UBaseType_t) START_TASK_PRIO,       //�������ȼ�
                (TaskHandle_t *)&StartTask_Handler   //������
                );

    vTaskStartScheduler();  //��ʼ�������
}
