/*
 * @Author: 符文麒
 * @Date: 2022-10-02 16:32:40
 * @LastEditors: 符文麒
 * @LastEditTime: 2022-10-05 19:28:45
 * @FilePath: \USERg:\Innovation and Entrepreneurship\Research on online control method of optimal power output for pure solar UAV\code\exp5all\Exp5test_liziqun\HARDWARE\ACC\acc.h
 * @Description: 需要本代码相关的支持请邮件:debuking@qq.com
 * 
 * Copyright (c) 2022 by 符文麒/北京理工大学宇航学院, All Rights Reserved. 
 */
#ifndef _ACC_H
#define _ACC_H
#include "sdram.h"

typedef struct
{
	//电压电流
	float Power[3];		   //个体计算功率pp
	float Power_temp[3];	   //个体缓存功率pptemp
	float Power_max[3];	   //个体缓存最大功率ppmax
    float Powermax;     //总体最大功率pmax
    float Powermax_temp;
    u16 acc;            //最大功率对应油门
	u16 accelerator[3];	   //个体油门a
	u16 accelerator_temp[3];  //个体缓存油门atemp
	//u16 accelerator_max[3];   //个体缓存最大油门
	u16 delta_accelerator[3]; //油门变化步长v
    u16 ws;
    u16 we;
    int t;
    u16 c1;
    u16 c2;
    u16 delta_accelerator_max;//vmax
    u16 delta_accelerator_min;//vmin
    u16 accelerator_max;//amax
    u16 accelerator_min;//amin
    u16 w;
						   //中断
} throttle_optimal;		   //结构体命名：油门寻优

void initialize_val(); //初始化变量
u16 jud_P(u16 i);		   //扰动观测法
float get_P();		   //获取电压电流，计算功率

extern throttle_optimal TO;

#endif
