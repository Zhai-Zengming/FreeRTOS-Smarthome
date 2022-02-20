#ifndef __TIMER_H
#define __TIMER_H

#include "stm32f10x.h"

/************高级定时器TIM参数定义，只限TIM2,3,4,5************/
// 当使用不同的定时器的时候，对应的GPIO是不一样的，这点要注意
// 这里我们使用高级控制定时器TIM3

#define            GENERAL_TIM                   TIM3
#define            GENERAL_TIM_APBxClock_FUN     RCC_APB1PeriphClockCmd
#define            GENERAL_TIM_CLK               RCC_APB1Periph_TIM3
// PWM 信号的频率 F = TIM_CLK/{(ARR+1)*(PSC+1)}
#define            GENERAL_TIM_PERIOD            (200-1)
#define            GENERAL_TIM_PSC               (7200-1)


#define            GENERAL_TIM_IRQ               TIM1_UP_IRQn
#define            GENERAL_TIM_IRQHandler        TIM1_UP_IRQHandler

// TIM3 输出比较通道
#define            GENERAL_TIM_CH1_GPIO_CLK      RCC_APB2Periph_GPIOA
#define            GENERAL_TIM_CH1_PORT          GPIOA
#define            GENERAL_TIM_CH1_PIN           GPIO_Pin_6
#define            GENERAL_TIM_CH1_PULSE         0

#define            GENERAL_TIM_CH2_GPIO_CLK      RCC_APB2Periph_GPIOA
#define            GENERAL_TIM_CH2_PORT          GPIOA
#define            GENERAL_TIM_CH2_PIN           GPIO_Pin_7
#define            GENERAL_TIM_CH2_PULSE         0




void GENERAL_TIM_Init(void);


#endif





