#include "bsp_key.h"

void Delay(__IO uint32_t nCount)	 //简单的延时函数
{
	for(; nCount != 0; nCount--);
}

void KEY1_GPIO_Config(void)
{
	GPIO_InitTypeDef KEY_GPIO_Init_Structure;
	
	RCC_APB2PeriphClockCmd(KEY1_GPIO_CLK, ENABLE);
	
	KEY_GPIO_Init_Structure.GPIO_Pin = KEY1_GPIO_PIN;
	KEY_GPIO_Init_Structure.GPIO_Mode = GPIO_Mode_IN_FLOATING; 
	
	GPIO_Init(KEY1_GPIO_Port, &KEY_GPIO_Init_Structure);
}

void KEY2_GPIO_Config(void)
{
	GPIO_InitTypeDef KEY_GPIO_Init_Structure;
	
	RCC_APB2PeriphClockCmd(KEY2_GPIO_CLK, ENABLE);
	
	KEY_GPIO_Init_Structure.GPIO_Pin = KEY2_GPIO_PIN;
	KEY_GPIO_Init_Structure.GPIO_Mode = GPIO_Mode_IN_FLOATING; 
	
	GPIO_Init(KEY2_GPIO_Port, &KEY_GPIO_Init_Structure);
}

void KEY_GPIO_Init(void)
{
	KEY1_GPIO_Config();
	KEY2_GPIO_Config();
}

//uint8_t Key1_Scan(GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin)
//{
//	if(GPIO_ReadInputDataBit(GPIOx, GPIO_Pin) == KEY_ON)
//	{
//		while(GPIO_ReadInputDataBit(GPIOx, GPIO_Pin) == KEY_ON);
//		return KEY1_ON;
//	}
//	else
//		return KEY1_OFF;
//}

//uint8_t Key2_Scan(GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin)
//{
//	if(GPIO_ReadInputDataBit(GPIOx, GPIO_Pin) == KEY_ON)
//	{
//		while(GPIO_ReadInputDataBit(GPIOx, GPIO_Pin) == KEY_ON);
//		return KEY2_ON;
//	}
//	else
//		return KEY2_OFF;
//}

uint8_t Key_Scan(void)
{
	if(GPIO_ReadInputDataBit(KEY1_GPIO_Port, KEY1_GPIO_PIN) == KEY_ON)
	{
		while(GPIO_ReadInputDataBit(KEY1_GPIO_Port, KEY1_GPIO_PIN) == KEY_ON);
		return KEY1_ON;
	}
	
	if(GPIO_ReadInputDataBit(KEY2_GPIO_Port, KEY2_GPIO_PIN) == KEY_ON)
	{
		while(GPIO_ReadInputDataBit(KEY2_GPIO_Port, KEY2_GPIO_PIN) == KEY_ON);
		return KEY2_ON;
	}
	
	else
		return KEY_OFF;
}
