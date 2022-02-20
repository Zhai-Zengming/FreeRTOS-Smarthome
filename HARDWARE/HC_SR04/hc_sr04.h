#ifndef _HC_SR04_H_
#define _HC_SR04_H_

#include "stm32f10x.h"  

#define RCC_APBxPeriph_GPIOx        RCC_APB2Periph_GPIOD

#define TRIG              GPIO_Pin_0 //触发信号
#define ECHO              GPIO_Pin_1 //信号回响
#define GPIO_US015        GPIOD

#define TRIG_Low          GPIO_ResetBits(GPIO_US015,TRIG)
#define TRIG_High         GPIO_SetBits(GPIO_US015,TRIG)
#define ECHO_Low          GPIO_ResetBits(GPIO_US015,ECHO)
#define ECHO_High         GPIO_SetBits(GPIO_US015,ECHO)

void US015_UserConfiguer(void);
void US015_Read_Data(float *cmA);

#endif
