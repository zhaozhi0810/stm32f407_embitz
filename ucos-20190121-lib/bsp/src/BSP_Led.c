/*
********************************************************************************
*
*                                 BSP_Led.c
*
* File          : BSP_Led.c
* Version       : V1.0
* Author        : dazhi
* Mode          : Thumb2
* Toolchain     :
* Description   : LED灯驱动程序
*
* History       :
* Date          : 2019.01.21
*******************************************************************************/

#include "misc.h"
#include "stm32f4xx_gpio.h"
#include "stm32f4xx_rcc.h"
#include "BSP_Led.h"

static const  INT32U  led_arr[] = { (BSP_GPIO_LED1 | BSP_GPIO_LED2 | BSP_GPIO_LED3),BSP_GPIO_LED1,BSP_GPIO_LED2,BSP_GPIO_LED3};




/*
*********************************************************************************************************
*                                           BSP_LED_Init()
*
* Description : Initialize any or all the LEDs on the board.
*
* Argument(s) : led     The ID of the LED to control:
*
*                       0    inialize ALL  LEDs
*                       1    inialize user LED1
*                       2    inialize user LED2
*                       3    inialize user LED3
*                       4    inialize user LED4
*
* Return(s)   : none.
*
* Caller(s)   : Application.
*
* Note(s)     : none.
*********************************************************************************************************
*/
void  BSP_LED_Init(void)
{
    GPIO_InitTypeDef  led_init;

//    BSP_PeriphEn(BSP_PERIPH_ID_GPIOE);                          /* Configure GPIOG for LED1 and LED2                    */
    RCC_AHB1PeriphClockCmd(LED_GPIO_RCC, ENABLE);

    led_init.GPIO_Pin   = BSP_GPIO_LED1 | BSP_GPIO_LED2 | BSP_GPIO_LED3;
    led_init.GPIO_Mode  = GPIO_Mode_OUT;
    led_init.GPIO_OType = GPIO_OType_PP;
    led_init.GPIO_PuPd  = GPIO_PuPd_UP;
    led_init.GPIO_Speed = GPIO_Speed_2MHz;

    GPIO_Init(LED_GPIO, &led_init);
}

/*
*********************************************************************************************************
*                                             BSP_LED_On()
*
* Description : Turn ON any or all the LEDs on the board.
*
* Argument(s) : led     The ID of the LED to control:
*
*                       0    turns ON ALL  LEDs
*                       1    turns ON user LED1
*                       2    turns ON user LED2
*                       3    turns ON user LED3
*                       4    turns ON user LED4
*
* Return(s)   : none.
*
* Caller(s)   : Application.
*
* Note(s)     : none.
*********************************************************************************************************
*/

void  BSP_LED_On (INT8U     led)
{
     if(led< 4)
    {
        GPIO_SetBits(LED_GPIO, led_arr[led]);
    }
}

/*
*********************************************************************************************************
*                                              BSP_LED_Off()
*
* Description : Turn OFF any or all the LEDs on the board.
*
* Argument(s) : led     The ID of the LED to control:
*
*                       0    turns OFF ALL the LEDs
*                       1    turns OFF user LED1
*                       2    turns OFF user LED2
*                       3    turns OFF user LED3
*                       4    turns OFF user LED4
*
* Return(s)   : none.
*
* Caller(s)   : Application.
*
* Note(s)     : none.
*********************************************************************************************************
*/

void  BSP_LED_Off (INT8U led)
{
     if(led< 4)
    {
        GPIO_ResetBits(LED_GPIO, led_arr[led]);
    }
}


/*
*********************************************************************************************************
*                                            BSP_LED_Toggle()
*
* Description : TOGGLE any or all the LEDs on the board.
*
* Argument(s) : led     The ID of the LED to control:
*
*                       0    TOGGLE ALL the LEDs
*                       1    TOGGLE user LED1
*                       2    TOGGLE user LED2
*                       3    TOGGLE user LED3
*                       4    TOGGLE user LED4
*
* Return(s)   : none.
*
* Caller(s)   : Application.
*
* Note(s)     : none.
*********************************************************************************************************
*/

void  BSP_LED_Toggle (INT8U  led)
{
    INT32U  pins;

    if(led< 4)
    {
//        pins  =  GPIO_ReadOutputData(LED_GPIO);
//        pins ^= led_arr[led];
//        GPIO_SetBits( LED_GPIO,   pins  & (led_arr[led]));
//        GPIO_ResetBits(LED_GPIO, (~pins) & (led_arr[led]));
		GPIO_ToggleBits(LED_GPIO, (led_arr[led]));

    }
}




