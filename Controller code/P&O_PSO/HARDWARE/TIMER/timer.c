#include "timer.h"
#include "led.h"


TIM_HandleTypeDef TIM3_Handler;         //定时器句柄 
TIM_HandleTypeDef TIM2_Handler;         //定时器句柄 
TIM_HandleTypeDef TIM7_Handler;         //定时器7句柄


u8 TIM2_FLAG;
u8 TIM3_FLAG;


//通用定时器3中断初始化
//arr：自动重装值。
//psc：时钟预分频数
//定时器溢出时间计算方法:Tout=((arr+1)*(psc+1))/Ft us. Tout = ((5000)*10800)/108 = 500,000us = 500ms = 0.5s = 2Hz
//Ft=定时器工作频率,单位:Mhz
//这里使用的是定时器3!(定时器3挂在APB1上，时钟为HCLK/2)
void TIM3_Init(u16 arr,u16 psc)
{  
    TIM3_Handler.Instance=TIM3;                          //通用定时器3 = 16位
    TIM3_Handler.Init.Prescaler=psc;                     //分频
    TIM3_Handler.Init.CounterMode=TIM_COUNTERMODE_UP;    //向上计数器
    TIM3_Handler.Init.Period=arr;                        //自动装载值
    TIM3_Handler.Init.ClockDivision=TIM_CLOCKDIVISION_DIV1;//时钟分频因子
	
    HAL_TIM_Base_Init(&TIM3_Handler);
    HAL_TIM_Base_Start_IT(&TIM3_Handler); //使能定时器3和定时器3更新中断：TIM_IT_UPDATE    
}

 void TIM2_Init(u16 arr,u16 psc)
{  
    TIM2_Handler.Instance=TIM2;                          //通用定时器2 = 32位
    TIM2_Handler.Init.Prescaler=psc;                     //分频
    TIM2_Handler.Init.CounterMode=TIM_COUNTERMODE_UP;    //向上计数器
    TIM2_Handler.Init.Period=arr;                        //自动装载值
    TIM2_Handler.Init.ClockDivision=TIM_CLOCKDIVISION_DIV1;//时钟分频因子
	
    HAL_TIM_Base_Init(&TIM2_Handler);
    HAL_TIM_Base_Start_IT(&TIM2_Handler); //使能定时器2和定时3更新中断：TIM_IT_UPDATE    
}

void TIM7_Init(u16 arr,u16 psc)
{  
    __HAL_RCC_TIM7_CLK_ENABLE();                        //使能TIM7时钟
    TIM7_Handler.Instance=TIM7;                          //通用定时器7
    TIM7_Handler.Init.Prescaler=psc;                     //分频系数
    TIM7_Handler.Init.CounterMode=TIM_COUNTERMODE_UP;    //向上计数器
    TIM7_Handler.Init.Period=arr;                        //自动装载值
    TIM7_Handler.Init.ClockDivision=TIM_CLOCKDIVISION_DIV1;//时钟分频因子
    HAL_TIM_Base_Init(&TIM7_Handler);
    HAL_TIM_Base_Start_IT(&TIM7_Handler);   //使能定时器7和定时器7更新中断：TIM_IT_UPDATE   
    
    HAL_NVIC_SetPriority(TIM7_IRQn,0,1);    //设置中断优先级，抢占优先级1，子优先级3
    HAL_NVIC_EnableIRQ(TIM7_IRQn);          //开启ITM7中断  
}

//定时器底册驱动，开启时钟，设置中断优先级
//此函数会被HAL_TIM_Base_Init()函数调用
void HAL_TIM_Base_MspInit(TIM_HandleTypeDef *htim)
{
    if(htim->Instance==TIM3)
	{
		__HAL_RCC_TIM3_CLK_ENABLE();            //使能TIM3时钟
		HAL_NVIC_SetPriority(TIM3_IRQn,1,3);    //设置中断优先级，抢占优先级1，子优先级3
		HAL_NVIC_EnableIRQ(TIM3_IRQn);          //开启ITM3中断   
	} 
	
    if(htim->Instance==TIM2)
	{
		__HAL_RCC_TIM2_CLK_ENABLE();            //使能TIM2时钟
		HAL_NVIC_SetPriority(TIM2_IRQn,1,2);    //设置中断优先级，抢占优先级1，子优先级2
		HAL_NVIC_EnableIRQ(TIM2_IRQn);          //开启ITM3中断   
	}  	
}

//定时器3中断服务函数
void TIM3_IRQHandler(void)
{
    HAL_TIM_IRQHandler(&TIM3_Handler);
}

void TIM2_IRQHandler(void)
{
    HAL_TIM_IRQHandler(&TIM2_Handler);
}


//定时器3中断服务函数调用
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{	
    if(htim==(&TIM3_Handler))
    {

			TIM3_FLAG = 1;
		
		}
	
	if(htim==(&TIM2_Handler))
    {
			TIM2_FLAG = 1;
	  }
}
