#include <stdio.h>
#include <stdlib.h>
#include "sdram.h"
#include <acc.h>
#include <hil.h>

throttle_optimal TO;

void initialize_val()
{
    u16 i;
    u16 j;
    TO.accelerator[0] = 30;
    TO.accelerator[1] = 50;
    TO.accelerator[2] = 70;
    for (i = 0; i <= 2; i++)
    {

        TO.delta_accelerator[i] = 5;

        TO.accelerator_temp[i] = TO.accelerator[i];

        TO.Power[i] = 0;
        TO.Power_temp[i] = TO.Power[i];

        TO.Power_max[i] = TO.Power[i];
        TO.Powermax = TO.Power_max[0];
        TO.acc = TO.accelerator[0];
        for (j = 1; j <= 2; j++)
        {
            if (TO.Power_max[j] > TO.Powermax)
            {
                TO.Powermax = TO.Power_max[j];
                TO.acc = TO.accelerator[j];
            }
        }
    }

    TO.ws = 0.9;
    TO.we = 0.4;
    TO.c1 = 0.149445;
    TO.c2 = 0.149445;
    TO.accelerator_max = 100;
    TO.accelerator_min = 0;
    TO.delta_accelerator_max = 5;
    TO.delta_accelerator_min = -5;
    TO.t = 5;
    TO.w = 0;
    TO.Powermax_temp = 0;
}

u16 jud_P(u16 i)
{

    Throttle_set_Request(TO.accelerator[i]);
    delay_ms(100);
    TO.Power[i] = HIL.current * HIL.voltage; // delay_ms(100);

    TO.delta_accelerator[i] = 0.4 * TO.delta_accelerator[i] + TO.c1 * (TO.Power_max[i] - TO.Power[i]) + TO.c2 * (TO.Powermax - TO.Power[i]);

    if (TO.delta_accelerator[i] > TO.delta_accelerator_max)
    {
        TO.delta_accelerator[i] = TO.delta_accelerator_max;
    }
    if (TO.delta_accelerator[i] < TO.delta_accelerator_min)
    {
        TO.delta_accelerator[i] = TO.delta_accelerator_min;
    }
    delay_ms(100);

    if (TO.Power[i] > TO.Power_temp[i])
    {
        if (TO.accelerator[i] > TO.accelerator_temp[i])
        {
            TO.Power_temp[i] = TO.Power[i];
            TO.accelerator_temp[i] = TO.accelerator[i];
            TO.accelerator[i] = TO.accelerator[i] + TO.delta_accelerator[i];
        }
        else
        {
            TO.Power_temp[i] = TO.Power[i];
            TO.accelerator_temp[i] = TO.accelerator[i];
            TO.accelerator[i] = TO.accelerator[i] - TO.delta_accelerator[i];
        }
    }
    else
    {
        if (TO.accelerator[i] > TO.accelerator_temp[i])
        {
            TO.Power_temp[i] = TO.Power[i];
            TO.accelerator_temp[i] = TO.accelerator[i];
            TO.accelerator[i] = TO.accelerator[i] - TO.delta_accelerator[i];
        }
        else
        {
            TO.Power_temp[i] = TO.Power[i];
            TO.accelerator_temp[i] = TO.accelerator[i];
            TO.accelerator[i] = TO.accelerator[i] + TO.delta_accelerator[i];
        }
    }
    delay_ms(100);

    if (TO.accelerator[i] > TO.accelerator_max)
    {
        TO.accelerator[i] = TO.accelerator_max;
    }
    if (TO.accelerator[i] < TO.accelerator_min)
    {
        TO.accelerator[i] = TO.accelerator_min;
    }
    delay_ms(100);

    if (TO.Power[i] > TO.Power_max[i])
    {
        TO.Power_max[i] = TO.Power[i];
    }

    if (TO.Power[i] > TO.Powermax)
    {

        TO.Powermax = TO.Power[i];
        TO.acc = TO.accelerator[i];
    }

    delay_ms(300);

    return TO.acc;
}
