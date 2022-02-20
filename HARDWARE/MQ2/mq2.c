#include "mq2.h"
#include "timer.h"
#include "adc_dma.h"
#include "stdio.h"
#include "math.h"
#include "delay.h"

//得到ADC采样内部传感器的值
//取10次,然后平均
vu16 Get_ADCValue_MQ2(void)
{
	u32 val = 0;
	u8 times = 10;
	u8 count;
	for(count = 0; count < times; count++)
	{
		val += ADC_ConvertedValue;//获取DMA通道值
		delay_ms(5);
	}
	return val/times;
}

// MQ2传感器数据处理
float MQ2_GetPPM(void)
{	
    float Vrl = 3.3f * Get_ADCValue_MQ2() / 4096.f;
	float ppm = Vrl*100;
    return  ppm;
}
		 







