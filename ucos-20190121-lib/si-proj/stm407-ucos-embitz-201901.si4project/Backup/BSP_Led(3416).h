/*
 ********************************************************************************
 *
 *                                 BSP_Led.h
 *
 * File          : BSP_Led.h
 * Version       : V1.0
 * Author        : dazhi
 * Mode          : Thumb2
 * Toolchain     :
 * Description   : led驱动头文件
 *
 * History       :
 * Date          : 2019.01.21
 *******************************************************************************/

#include "stm32f4xx_gpio.h"

 #ifndef _BSP_LED_H_
 #define _BSP_LED_H_

#define LED1_ON() 		GPIO_ResetBits(GPIOE , GPIO_Pin_13)
#define LED2_ON() 		GPIO_ResetBits(GPIOE , GPIO_Pin_14)
#define LED3_ON() 		GPIO_ResetBits(GPIOE , GPIO_Pin_15)

#define LED1_OFF() 		GPIO_SetBits(GPIOE , GPIO_Pin_13)
#define LED2_OFF() 		GPIO_SetBits(GPIOE , GPIO_Pin_14)
#define LED3_OFF() 		GPIO_SetBits(GPIOE , GPIO_Pin_15)


//led初始化函数
void BSP_LedOpen(void);

 #endif


