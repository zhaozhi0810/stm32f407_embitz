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

#define LED1_ON() 		GPIO_ResetBits(GPIOE , GPIO_Pin_13)
#define LED2_ON() 		GPIO_ResetBits(GPIOE , GPIO_Pin_14)
#define LED3_ON() 		GPIO_ResetBits(GPIOE , GPIO_Pin_15)

#define LED1_OFF() 		GPIO_SetBits(GPIOE , GPIO_Pin_13)
#define LED2_OFF() 		GPIO_SetBits(GPIOE , GPIO_Pin_14)
#define LED3_OFF() 		GPIO_SetBits(GPIOE , GPIO_Pin_15)



/*******************************************************************************
* Function Name :BSP_LedOpen
* Description   :led引脚初始化
* Input         :  void
* Output        :  void
* Other         :  led对应的是GPIOE 13，14，15三个引脚
* Date          :2019.01.21
*******************************************************************************/
void BSP_LedOpen(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;
    
        /* Enable GPIO clock */
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOE, ENABLE);
    
        /* Configure USART Tx as alternate function push-pull */
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_13|GPIO_Pin_14|GPIO_Pin_15;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;  //io的速度不需要太快，增加功耗和噪声
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;   //无上拉下拉
    GPIO_Init(GPIOE, &GPIO_InitStructure);
    
    return ;
}


/*******************************************************************************
* Function Name : void BSP_LedClose(void)
* Description   : led关闭
* Input         : void
* Output        : 
* Other         : GPIO的时钟不能随便关闭，会影响其他功能
* Date          : 2019.01.21
*******************************************************************************/
void BSP_LedClose(void)
{
    return ;   //to do nothing  
    /* DISABLE GPIO clock */
//    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOE, DISABLE);
}


/*******************************************************************************
* Function Name : uint32_t BSP_LedWrite(uint8_t COMx, uint8_t *buffter, uint32_t len)
* Description   : 串口发送数据
* Input         :   COMx:通道x
* Output        : 
* Other         : 暂时没有功能
* Date          : 2019.01.21
*******************************************************************************/
uint32_t BSP_LedWrite(uint8_t ledn, uint8_t stat)
{

    return 0;
}

/*******************************************************************************
* Function Name : uint32_t BSP_LedRead(uint8_t ledn)
* Description   : led读回状态
* Input         : ledn:led1，led2，led3
* Output        : 
* Other         : 暂时没有功能
* Date          : 2019.01.21
*******************************************************************************/
uint32_t BSP_LedRead(uint8_t ledn)
{
    return 0;
}





