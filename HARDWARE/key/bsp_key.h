#ifndef __BSP_KEY_H
#define __BSP_KEY_H

#include "stm32f10x.h"

#define KEY1_GPIO_PIN             GPIO_Pin_0
#define KEY1_GPIO_Port            GPIOA
#define KEY1_GPIO_CLK 	          RCC_APB2Periph_GPIOA

#define KEY2_GPIO_PIN             GPIO_Pin_13
#define KEY2_GPIO_Port            GPIOC
#define KEY2_GPIO_CLK 	          RCC_APB2Periph_GPIOC

#define KEY_ON       1
#define KEY_OFF      0

#define KEY1_ON       1
#define KEY1_OFF      0

#define KEY2_ON       2
#define KEY2_OFF      0

void KEY1_GPIO_Config(void);
void KEY2_GPIO_Config(void);
void KEY_GPIO_Init(void);
//uint8_t Key1_Scan(GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin);
//uint8_t Key2_Scan(GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin);
uint8_t Key_Scan(void);

#endif
