#include "hc_sr04.h"
#include "delay.h"

void US015_UserConfiguer(void){
	
	GPIO_InitTypeDef GPIO_InitStructure;
	TIM_TimeBaseInitTypeDef TIM_InitStructure;
	
	RCC_APB2PeriphClockCmd(RCC_APBxPeriph_GPIOx,ENABLE); 
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM6,ENABLE);
	
	GPIO_InitStructure.GPIO_Pin   = TRIG;               //触发信号
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_10MHz;
	GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_Out_PP;
	GPIO_Init(GPIO_US015,&GPIO_InitStructure);
	TRIG_Low;
	
	GPIO_InitStructure.GPIO_Pin   = ECHO;               //信号回响
	GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_IPD;
	GPIO_Init(GPIO_US015,&GPIO_InitStructure);
	ECHO_Low;
	
	TIM_InitStructure.TIM_Period  = 65535; //1us
	TIM_InitStructure.TIM_Prescaler = 72-1; //72000000/72 = 1000000 1Mhz
	TIM_InitStructure.TIM_ClockDivision = TIM_CKD_DIV1;
	TIM_InitStructure.TIM_RepetitionCounter = DISABLE;
	TIM_InitStructure.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_TimeBaseInit(TIM6,&TIM_InitStructure);
	TIM_Cmd(TIM6,DISABLE);
}
void US015_Read_Data(float *cmA){
	
	while(GPIO_ReadInputDataBit(GPIO_US015,ECHO) == 1);
	
	TRIG_High;   //15US  //10US
	delay_us(15);
	TRIG_Low;
	
	while(GPIO_ReadInputDataBit(GPIO_US015,ECHO) == 0);
	TIM_SetCounter(TIM6,0);
	TIM_Cmd(TIM6,ENABLE);
	while(GPIO_ReadInputDataBit(GPIO_US015,ECHO) == 1);
	TIM_Cmd(TIM6,DISABLE);
	
	delay_ms(60); //两次测量不能低于60ms
	
	*cmA = TIM_GetCounter(TIM6)*340/1000000.0/2*100; //CM
//	*cmB = TIM_GetCounter(TIM6)/58.0; //CM
//	*cmC = TIM_GetCounter(TIM6)/58; //CM
}
