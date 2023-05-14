#ifndef _ACC_H
#define _ACC_H
#include "sdram.h"

typedef struct
{
	//电压电流
	float Power;		   //计算功率
	float Power_temp;	   //缓存功率
	float Power_max;	   //缓存最大功率
	u16 accelerator;	   //油门
	u16 accelerator_temp;  //缓存油门
	u16 accelerator_max;   //缓存最大油门
	u16 delta_accelerator; //油门变化步长
						   //中断
} throttle_optimal;		   //结构体命名：油门寻优

void initialize_val(); //初始化变量
u16 jud_P();		   //扰动观测法
float get_P();		   //获取电压电流，计算功率

extern throttle_optimal TO;

#endif
