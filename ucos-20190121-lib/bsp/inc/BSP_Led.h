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
#include "os_cpu.h"



 #ifndef _BSP_LED_H_
 #define _BSP_LED_H_


/*
*********************************************************************************************************
*                                              LED SERVICES
*********************************************************************************************************
*/


#define  BSP_GPIO_LED1                        GPIO_Pin_13
#define  BSP_GPIO_LED2                        GPIO_Pin_14
#define  BSP_GPIO_LED3                        GPIO_Pin_15


#define  LED_ALL   0
#define  LED1       1
#define  LED2       2
#define  LED3       3





#define LED_GPIO_RCC RCC_AHB1Periph_GPIOE
#define LED_GPIO    GPIOE





void  BSP_LED_Init(void);
void  BSP_LED_On (INT8U	 led);
void  BSP_LED_Off (INT8U led);
void  BSP_LED_Toggle (INT8U  led);



 #endif


