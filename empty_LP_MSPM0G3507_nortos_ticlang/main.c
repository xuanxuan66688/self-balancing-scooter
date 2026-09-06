/*
 * Copyright (c) 2021, Texas Instruments Incorporated
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 * *  Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 *
 * *  Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 * *  Neither the name of Texas Instruments Incorporated nor the names of
 *    its contributors may be used to endorse or promote products derived
 *    from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO,
 * THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
 * PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR
 * CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
 * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 * PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS;
 * OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
 * WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR
 * OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE,
 * EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#include "ti_msp_dl_config.h"
#include "main.h"
#include "stdio.h"
#include "USER/MOTOR.h"
#include "USER/Encoder.h"
#include "USER/Serial.h"
#include "USER/WITshow.h"
#include "USER/pid.h"
pid_type_def pid_stand;

int main(void)       
{
    SYSCFG_DL_init();
    SysTick_Init();

    OLED_Init();
    /* Don't remove this! */
    Interrupt_Init();
    /* 初始化编码器计次（使能 GROUP1 中断并读取初始相位） */
    Encoder_Init();
    /* 初始化 WIT 姿态传感器（使能 DMA 与 UART_WIT 接收中断） */
    WIT_Init();
    /* 串口收发初始化（使能 UART0 接收中断） */
    Serial_Init();
    /*pid控制函数初始化*/
    // 定义电机pid参数
    float pid_param_0[3] = {3, 0.0, 20.0};
    PID_init(&pid_stand, PID_POSITION, pid_param_0, 100.0, 50.0);
   
    /* 驱动电机 A、B，各 50% 占空比（满占空比 100） */
     DL_TimerA_startCounter(PWM_0_INST);
   // MOTOR_duty(100, MOTOR_B);
   // MOTOR_duty(0, MOTOR_A);
    NVIC_EnableIRQ(TIMER_0_INST_INT_IRQN);
    DL_TimerA_startCounter(TIMER_0_INST);

    while (1)
    {
        /* 显示 WIT 姿态角（P/R/Y） */
        WIT_Show_Data();
         
    }
}
void TIMER_0_INST_IRQHandler()
{
   float angle, speed, balance_out, speed_out;
    int16_t duty;

    /* 1. 读姿态角（WIT 俯仰角，单位：度） */
    angle = wit_data.pitch;

    /* 2. 读速度（左右编码器平均，计数/秒） */
   // speed = (Encoder_Speed_L() + Encoder_Speed_R()) * 0.5f;

    /* 3. 直立环：目标 0°，反馈当前角度 → 输出期望速度 */
    balance_out = PID_calc(&pid_stand, angle, 0.0f);

    /* 4. 速度环：目标 = 直立环输出，反馈当前速度 → 输出 PWM 占空比 */
   // speed_out = PID_calc(&pid_speed, speed, balance_out);

    /* 5. 两轮同向输出纠正倾斜；若方向反了，把 Kp 取负或互换电机正负 */
    duty = (int16_t)balance_out;
    if(duty==0)
    {
        duty=1;
    }
    MOTOR_duty(duty, MOTOR_B);   /* 左轮 */
    MOTOR_duty(duty, MOTOR_A);   /* 右轮 */
       
}