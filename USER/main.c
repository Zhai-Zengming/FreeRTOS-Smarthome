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
@brief: FreeRTOS智能家居
	
**********************************************************************/

//任务优先级
#define START_TASK_PRIO		1
//任务堆栈大小	
#define START_STK_SIZE 		256  
//任务句柄
TaskHandle_t StartTask_Handler;
//任务函数
void start_task(void *pvParameters);

//任务优先级
#define HARD_TASK_PRIO		2
//任务堆栈大小	
#define HARD_STK_SIZE 		128
//任务句柄
TaskHandle_t HardTask_Handler;
//任务函数
void hard_task(void *pvParameters);

//任务优先级
#define SG90_TASK_PRIO		3
//任务堆栈大小	
#define SG90_STK_SIZE 		128
//任务句柄
TaskHandle_t Sg90Task_Handler;
//任务函数
void sg90_task(void *pvParameters);

//任务优先级
#define MQ2_TASK_PRIO		4
//任务堆栈大小	
#define MQ2_STK_SIZE 		128
//任务句柄
TaskHandle_t Mq2Task_Handler;
//任务函数
void mq2_task(void *pvParameters);

float light_value;
DHT11_Data_TypeDef DHT11_Data;
float hc_sr04value;
float MQ2_value;

int main(void)
{
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_4);//设置系统中断优先级分组4	 	 
	delay_init();	    				//延时函数初始化	  
	uart_init(115200);					//初始化串口
	LED_GPIO_Config();		  					//初始化LED
	KEY_GPIO_Init();
	GENERAL_TIM_Init();
	US015_UserConfiguer();
	ADCx_Init();
	BEEP_Init();
	BH1750_Init();
	DHT11_GPIO_Config();
	OLED_Init();
	OLED_Clear();
	
	//创建开始任务
    xTaskCreate((TaskFunction_t )start_task,            //任务函数
                (const char*    )"start_task",          //任务名称
                (uint16_t       )START_STK_SIZE,        //任务堆栈大小
                (void*          )NULL,                  //传递给任务函数的参数
                (UBaseType_t    )START_TASK_PRIO,       //任务优先级
                (TaskHandle_t*  )&StartTask_Handler);   //任务句柄              
    vTaskStartScheduler();          //开启任务调度
}

//开始任务任务函数
void start_task(void *pvParameters)
{
    taskENTER_CRITICAL();           //进入临界区

	
    //创建hard_task任务
    xTaskCreate((TaskFunction_t )hard_task,     	
                (const char*    )"hard_task",   	
                (uint16_t       )HARD_STK_SIZE, 
                (void*          )NULL,				
                (UBaseType_t    )HARD_TASK_PRIO,	
                (TaskHandle_t*  )&HardTask_Handler);
	//创建sg90_task任务
    xTaskCreate((TaskFunction_t )sg90_task,     	
                (const char*    )"sg90_task",   	
                (uint16_t       )SG90_STK_SIZE, 
                (void*          )NULL,				
                (UBaseType_t    )SG90_TASK_PRIO,	
                (TaskHandle_t*  )&Sg90Task_Handler);
	//创建mq2_task任务
    xTaskCreate((TaskFunction_t )mq2_task,     	
                (const char*    )"mq2_task",   	
                (uint16_t       )MQ2_STK_SIZE, 
                (void*          )NULL,				
                (UBaseType_t    )MQ2_TASK_PRIO,	
                (TaskHandle_t*  )&Mq2Task_Handler);
    
    vTaskDelete(StartTask_Handler); //删除开始任务
    taskEXIT_CRITICAL();            //退出临界区
}

//hard_task任务函数 
void hard_task(void *pvParameters)
{
	char sprintf_str[] = "";

    while(1)
    {
//		printf("hard_task running\r\n");
		/* 获取光照值 */
		light_value = LIght_Intensity();
		sprintf(sprintf_str, "light: %d lx   ", (int)light_value);
		OLED_ShowString(0,0,(u8*)sprintf_str,12);
		
		/* 获取温度湿度值 */
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
		
		/* 超声波测距 */
		US015_Read_Data(&hc_sr04value);
		sprintf(sprintf_str, "dist : %.2f cm  ", hc_sr04value);
		OLED_ShowString(0,3,(u8*)sprintf_str,12);
		
		/* CO浓度 */
		MQ2_value = MQ2_GetPPM();
		
		printf("MQ2_value = %f\r\n",MQ2_value);
		printf("ADC_ConvertedValue = %d\r\n",ADC_ConvertedValue);
		sprintf(sprintf_str, "CO   : %d ppm   ", (int)MQ2_value);
		OLED_ShowString(0,4,(u8*)sprintf_str,12);
		
		vTaskDelay(100);
    }
}   

/* 舵机任务函数:温度大于30舵机转动 */
void sg90_task(void *pvParameters)
{
	u16 pulse;
	while(1)                //每个任务都要有一个while(1)循环，否则程序会卡死
	{
		while(DHT11_Data.temp_int > 10)
		{
			printf("sg90_task running\r\n");
			for(pulse=5; pulse<=25; pulse+=5)
			{
				TIM_SetCompare1(GENERAL_TIM,pulse);          //通道1
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

/* mq2_task任务函数: CO浓度超过设定报警 */
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





