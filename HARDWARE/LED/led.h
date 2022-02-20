#ifndef __LED_H
#define __LED_H

#include "stm32f10x.h"  

#define LED_G_GPIO_PIN           GPIO_Pin_0
#define LED_B_GPIO_PIN           GPIO_Pin_1
#define LED_R_GPIO_PIN           GPIO_Pin_5
#define LED_GPIO_Port            GPIOB
#define LED_GPIO_CLK 	         RCC_APB2Periph_GPIOB

#define ON           1
#define OFF          0
#define LED_1(a)       if(a) \
                	     GPIO_ResetBits(LED_GPIO_Port, LED_G_GPIO_PIN); \
                       else \
						 GPIO_SetBits(LED_GPIO_Port, LED_G_GPIO_PIN);  
#define LED_2(a)       if(a) \
                	     GPIO_ResetBits(LED_GPIO_Port, LED_B_GPIO_PIN); \
                       else \
						 GPIO_SetBits(LED_GPIO_Port, LED_B_GPIO_PIN); 
#define LED_3(a)       if(a) \
                	     GPIO_ResetBits(LED_GPIO_Port, LED_R_GPIO_PIN); \
                       else \
						 GPIO_SetBits(LED_GPIO_Port, LED_R_GPIO_PIN); 
#define LED_ALL(a)     do{if(a) \
						{\
							LED_1(ON);\
							LED_2(ON);\
							LED_3(ON);\
						} \
                       else \
						{\
							LED_1(OFF);\
							LED_2(OFF);\
							LED_3(OFF);\
						}}while(0)

#define LED_GREEN           do{LED_1(ON);LED_2(OFF);LED_3(OFF);}while(0)					
#define LED_BLUE            do{LED_1(OFF);LED_2(ON);LED_3(OFF);}while(0)
#define LED_RED             do{LED_1(OFF);LED_2(OFF);LED_3(ON);}while(0)
#define LED_YELLOW          do{LED_1(ON);LED_2(OFF);LED_3(ON);}while(0)     
#define LED_PURPLE          do{LED_1(OFF);LED_2(ON);LED_3(ON);}while(0)         
#define LED_CYAN            do{LED_1(ON);LED_2(ON);LED_3(OFF);}while(0)     //За
#define LED_WHITE           do{LED_1(ON);LED_2(ON);LED_3(ON);}while(0)

						
void LED_GPIO_Config(void);

#endif
