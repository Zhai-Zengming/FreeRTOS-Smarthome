#include "led.h"
#include "delay.h"
#include "sys.h"
#include "FreeRTOS.h"
#include "task.h"
#include "bsp_basictime.h"
#include "bsp_key.h"
#include "event_groups.h"
#include "hc_sr04.h"
#include "adc_dma.h"
#include "timer.h"
#include "beep.h"
#include "bh1750.h"
#include "dht11.h"
#include "oled.h"
#include "mq2.h"

/**********************************************************************
@date:  2022/01/23
@brief: FreeRTOS���ܼҾ�
	
**********************************************************************/

//�������ȼ�
#define START_TASK_PRIO		1
//�����ջ��С	
#define START_STK_SIZE 		256  
//������
TaskHandle_t StartTask_Handler;
//������
void start_task(void *pvParameters);

//�������ȼ�
#define HARD_TASK_PRIO		2
//�����ջ��С	
#define HARD_STK_SIZE 		128
//������
TaskHandle_t HardTask_Handler;
//������
void hard_task(void *pvParameters);

//�������ȼ�
#define SG90_TASK_PRIO		3
//�����ջ��С	
#define SG90_STK_SIZE 		128
//������
TaskHandle_t Sg90Task_Handler;
//������
void sg90_task(void *pvParameters);

//�������ȼ�
#define MQ2_TASK_PRIO		4
//�����ջ��С	
#define MQ2_STK_SIZE 		128
//������
TaskHandle_t Mq2Task_Handler;
//������
void mq2_task(void *pvParameters);

float light_value;
DHT11_Data_TypeDef DHT11_Data;
float hc_sr04value;
float MQ2_value;

int main(void)
{
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_4);//����ϵͳ�ж����ȼ�����4	 	 
	delay_init();	    				//��ʱ������ʼ��	  
	uart_init(115200);					//��ʼ������
	LED_GPIO_Config();		  					//��ʼ��LED
	KEY_GPIO_Init();
	GENERAL_TIM_Init();
	US015_UserConfiguer();
	ADCx_Init();
	BEEP_Init();
	BH1750_Init();
	DHT11_GPIO_Config();
	OLED_Init();
	OLED_Clear();
	
	//������ʼ����
    xTaskCreate((TaskFunction_t )start_task,            //������
                (const char*    )"start_task",          //��������
                (uint16_t       )START_STK_SIZE,        //�����ջ��С
                (void*          )NULL,                  //���ݸ��������Ĳ���
                (UBaseType_t    )START_TASK_PRIO,       //�������ȼ�
                (TaskHandle_t*  )&StartTask_Handler);   //������              
    vTaskStartScheduler();          //�����������
}

//��ʼ����������
void start_task(void *pvParameters)
{
    taskENTER_CRITICAL();           //�����ٽ���

	
    //����hard_task����
    xTaskCreate((TaskFunction_t )hard_task,     	
                (const char*    )"hard_task",   	
                (uint16_t       )HARD_STK_SIZE, 
                (void*          )NULL,				
                (UBaseType_t    )HARD_TASK_PRIO,	
                (TaskHandle_t*  )&HardTask_Handler);
	//����sg90_task����
    xTaskCreate((TaskFunction_t )sg90_task,     	
                (const char*    )"sg90_task",   	
                (uint16_t       )SG90_STK_SIZE, 
                (void*          )NULL,				
                (UBaseType_t    )SG90_TASK_PRIO,	
                (TaskHandle_t*  )&Sg90Task_Handler);
	//����mq2_task����
    xTaskCreate((TaskFunction_t )mq2_task,     	
                (const char*    )"mq2_task",   	
                (uint16_t       )MQ2_STK_SIZE, 
                (void*          )NULL,				
                (UBaseType_t    )MQ2_TASK_PRIO,	
                (TaskHandle_t*  )&Mq2Task_Handler);
    
    vTaskDelete(StartTask_Handler); //ɾ����ʼ����
    taskEXIT_CRITICAL();            //�˳��ٽ���
}

//hard_task������ 
void hard_task(void *pvParameters)
{
	char sprintf_str[] = "";

    while(1)
    {
//		printf("hard_task running\r\n");
		/* ��ȡ����ֵ */
		light_value = LIght_Intensity();
		sprintf(sprintf_str, "light: %d lx   ", (int)light_value);
		OLED_ShowString(0,0,(u8*)sprintf_str,12);
		
		/* ��ȡ�¶�ʪ��ֵ */
		if( Read_DHT11(&DHT11_Data)==SUCCESS )
		{
			sprintf(sprintf_str, "tempe: %d.%d",DHT11_Data.temp_int,DHT11_Data.temp_deci);
			OLED_ShowString(0,1,(u8*)sprintf_str,12);
			sprintf(sprintf_str, "humi:  %d.%d %",DHT11_Data.humi_int,DHT11_Data.humi_deci);
			OLED_ShowString(0,2,(u8*)sprintf_str,12);
		}
		else
		{
			DHT11_Data.temp_int = 0;
			DHT11_Data.temp_deci = 0;
			
			OLED_ShowString(0,1,"tempe: 000",12);
			OLED_ShowString(0,2,"humi : 000",12);
		}
		
		/* ��������� */
		US015_Read_Data(&hc_sr04value);
		sprintf(sprintf_str, "dist : %.2f cm  ", hc_sr04value);
		OLED_ShowString(0,3,(u8*)sprintf_str,12);
		
		/* COŨ�� */
		MQ2_value = MQ2_GetPPM();
		
		printf("MQ2_value = %f\r\n",MQ2_value);
		printf("ADC_ConvertedValue = %d\r\n",ADC_ConvertedValue);
		sprintf(sprintf_str, "CO   : %d ppm   ", (int)MQ2_value);
		OLED_ShowString(0,4,(u8*)sprintf_str,12);
		
		vTaskDelay(100);
    }
}   

/* ���������:�¶ȴ���30���ת�� */
void sg90_task(void *pvParameters)
{
	u16 pulse;
	while(1)                //ÿ������Ҫ��һ��while(1)ѭ�����������Ῠ��
	{
		while(DHT11_Data.temp_int > 10)
		{
			printf("sg90_task running\r\n");
			for(pulse=5; pulse<=25; pulse+=5)
			{
				TIM_SetCompare1(GENERAL_TIM,pulse);          //ͨ��1
				delay_ms(1000);
			}
			for(pulse=25; pulse<=5; pulse-=5)
			{
				TIM_SetCompare1(GENERAL_TIM,pulse);
				delay_ms(1000);
			}
		}
		vTaskDelay(10);
	}
}

/* mq2_task������: COŨ�ȳ����趨���� */
void mq2_task(void *pvParameters)
{
	while(1)
	{
		while(MQ2_value > 200)
		{
			printf("mq2_task running\r\n");
			BEEP_SetOnOff(1);
			LED_RED;
			delay_ms(300);
			BEEP_SetOnOff(0);
			LED_ALL(OFF);
			delay_ms(300);
		}
		vTaskDelay(10);
	}
}





