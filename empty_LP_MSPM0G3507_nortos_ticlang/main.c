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



/* 显示 WIT 姿态角（单位：度，1 位小数）：P=俯仰 pitch，R=横滚 roll，Y=偏航 yaw */
void WIT_Show_Data(void)
{
    /* 显示 WIT 姿态角（单位：度，保留 1 位小数）
     *  P = 俯仰 pitch，R = 横滚 roll，Y = 偏航 yaw
     *  第 4 行 AC 为成功解析的角度(0x53)帧计数，用于确认解析正常，调试确认后可删除 */
    OLED_ShowString(0, 0, (uint8_t *)"P:", 16);
    OLED_ShowSignedFloat(16, 0, wit_data.pitch, 1, 16);

    OLED_ShowString(0, 2, (uint8_t *)"R:", 16);
    OLED_ShowSignedFloat(16, 2, wit_data.roll, 1, 16);

    OLED_ShowString(0, 4, (uint8_t *)"Y:", 16);
    OLED_ShowSignedFloat(16, 4, wit_data.yaw, 1, 16);

}

int main(void)
{
    SYSCFG_DL_init();
    SysTick_Init();

   // OLED_Init();

    /* Don't remove this! */
    Interrupt_Init();

    /* 初始化编码器计次（使能 GROUP1 中断并读取初始相位） */
    Encoder_Init();
    /* 初始化 WIT 姿态传感器（使能 DMA 与 UART_WIT 接收中断） */
    WIT_Init();
    /* 串口收发初始化（使能 UART0 接收中断） */
    Serial_Init();
    Serial_SendString("1\r\n");
    DL_TimerA_startCounter(PWM_0_INST);
    int16_t i = -5;
    Serial_SendInt16(i);
    Serial_SendString("\r\n");

    /* 驱动电机 A、B，各 50% 占空比（满占空比 100） */
   // MOTOR_duty(50, MOTOR_B);
   // MOTOR_duty(50, MOTOR_A);

    while (1)
    {
        /* 显示 WIT 姿态角（P/R/Y） */
        //WIT_Show_Data();
        DL_GPIO_togglePins(TEST_LED_PORT, TEST_LED_LED_PIN);
       delay_ms(500);
    }
}