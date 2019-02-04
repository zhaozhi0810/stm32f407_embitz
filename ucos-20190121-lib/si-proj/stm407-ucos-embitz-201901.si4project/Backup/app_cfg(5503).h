/*
*********************************************************************************************************
*                                              EXAMPLE CODE
*
*                          (c) Copyright 2003-2006; Micrium, Inc.; Weston, FL
*
*               All rights reserved.  Protected by international copyright laws.
*               Knowledge of the source code may NOT be used to develop a similar product.
*               Please help us continue to provide the Embedded community with the finest
*               software available.  Your honesty is greatly appreciated.
*********************************************************************************************************
*/

/*
*********************************************************************************************************
*
*                                      APPLICATION CONFIGURATION
*
*                                     ST Microelectronics STM32
*                                              with the
*                                   STM3210E-EVAL Evaluation Board
*
* Filename      : app_cfg.h
* Version       : V1.00
* Programmer(s) : BAN
*********************************************************************************************************
*/

#ifndef  __APP_CFG_H__
#define  __APP_CFG_H__

#include <stdio.h>

/*
*********************************************************************************************************
*                                       MODULE ENABLE / DISABLE
*********************************************************************************************************
*/

#define  APP_OS_PROBE_EN                         DEF_ENABLED
#define  APP_PROBE_COM_EN                        DEF_ENABLED

/*
*********************************************************************************************************
*                                              TASKS NAMES
*********************************************************************************************************
*/


/*
*********************************************************************************************************
*                                            TASK PRIORITIES
*********************************************************************************************************
*/

#define  APP_TASK_START_PRIO                               3
#define  USART1_MUTEX_PRIO                                 6
#define  APP_TASK_1_PRIO                                   5
#define  LED2_TASK_PRIO                                   7
#define  APP_TASK_Printf_PRIO                              4
#define  TTY_TASK_PRIO                              8
#define  PRINT_TASK_PRIO                    (OS_LOWEST_PRIO - 3)     //比较低的优先级
#define  OS_TASK_TMR_PRIO                (OS_LOWEST_PRIO - 2)

/*
*********************************************************************************************************
*                                            TASK STACK SIZES
*                             Size of the task stacks (# of OS_STK entries)
*********************************************************************************************************
*/

#define  APP_TASK_START_STK_SIZE            128
#define  APP_TASK_1_STK_SIZE							 128
#define  LED2_TASK_STK_SIZE							 128
#define  APP_TASK_Printf_STK_SIZE							 512
#define TTY_TASK_STK_SIZE                              512
#define PRINT_TASK_STK_SIZE                         256

/*
*********************************************************************************************************
*                                                  LIB
*********************************************************************************************************
*/
#define DEBUG

#ifdef DEBUG
#define Debug_Messege()  \
        printf("%s %d\r",__FUNCTION__,__LINE__)
#else
#define Debug_Messege()

#endif


#endif
