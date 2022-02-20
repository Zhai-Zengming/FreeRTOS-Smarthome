//�˶�ʱ�����ڲ���PWM���ƶ��


#include "timer.h"

static void GENERAL_TIM_GPIO_Config(void) 
{
    GPIO_InitTypeDef GPIO_InitStructure;

    // ����Ƚ�ͨ�� GPIO ��ʼ��
	RCC_APB2PeriphClockCmd(GENERAL_TIM_CH1_GPIO_CLK, ENABLE);
    GPIO_InitStructure.GPIO_Pin =  GENERAL_TIM_CH1_PIN;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GENERAL_TIM_CH1_PORT, &GPIO_InitStructure);

	RCC_APB2PeriphClockCmd(GENERAL_TIM_CH2_GPIO_CLK, ENABLE);
    GPIO_InitStructure.GPIO_Pin =  GENERAL_TIM_CH2_PIN;
    GPIO_Init(GENERAL_TIM_CH2_PORT, &GPIO_InitStructure);

}

static void GENERAL_TIM_Mode_Config(void)
{
	TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
	TIM_OCInitTypeDef TIM_OCInitStructure;
	/* ��ʱ�� */
	GENERAL_TIM_APBxClock_FUN(GENERAL_TIM_CLK, ENABLE);
	
	/* ʱ���ṹ�� */
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_TimeBaseStructure.TIM_Period = GENERAL_TIM_PERIOD;
	TIM_TimeBaseStructure.TIM_Prescaler = GENERAL_TIM_PSC;
	TIM_TimeBaseStructure.TIM_RepetitionCounter = 0;
	TIM_TimeBaseInit(GENERAL_TIM, &TIM_TimeBaseStructure);

	/* ����ȽϽṹ�� */
	TIM_OCInitStructure.TIM_OCIdleState = TIM_OCIdleState_Set;
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;
	TIM_OCInitStructure.TIM_OCNIdleState = TIM_OCNIdleState_Reset;
	TIM_OCInitStructure.TIM_OCNPolarity = TIM_OCNPolarity_High;
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;
	TIM_OCInitStructure.TIM_OutputNState = TIM_OutputNState_Enable;
	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
	/* ͨ��1 */
	TIM_OCInitStructure.TIM_Pulse = GENERAL_TIM_CH1_PULSE;
	TIM_OC1Init(GENERAL_TIM, &TIM_OCInitStructure);
	TIM_OC1PreloadConfig(GENERAL_TIM, TIM_OCPreload_Enable);
	
	/* ͨ��2 */
	TIM_OCInitStructure.TIM_Pulse = GENERAL_TIM_CH2_PULSE;
	TIM_OC2Init(GENERAL_TIM, &TIM_OCInitStructure);
	TIM_OC2PreloadConfig(GENERAL_TIM, TIM_OCPreload_Enable);
	
	// ʹ�ܼ�����
	TIM_Cmd(GENERAL_TIM, ENABLE);	
}

void GENERAL_TIM_Init(void)
{
	GENERAL_TIM_GPIO_Config();
	GENERAL_TIM_Mode_Config();
}

/*********************************************END OF FILE**********************/
