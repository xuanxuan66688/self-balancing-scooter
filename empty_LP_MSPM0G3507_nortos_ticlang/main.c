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

uint8_t oled_buffer[32];
uint8_t buf[13] = "hello";

int main(void)
{
    SYSCFG_DL_init();
    SysTick_Init();

    OLED_Init();

    /* Don't remove this! */
    Interrupt_Init();

    OLED_ShowString(0, 0, buf, 16);
    DL_TimerA_startCounter(PWM_0_INST);
    Encoder_Init();

    /* 驱动电机 A、B，各 50% 占空比（满占空比 100） */
    MOTOR_duty(50, MOTOR_B);
    MOTOR_duty(50, MOTOR_A);
    //
    while (1)
    {
        /* 在 OLED 上显示两个电机的转速（RPM） */
        sprintf((char *)oled_buffer, "A:%drpm", (int)Encoder_GetRPM(ENCODER_A));
        OLED_ShowString(0, 0, oled_buffer, 16);

        sprintf((char *)oled_buffer, "B:%drpm", (int)Encoder_GetRPM(ENCODER_B));
        OLED_ShowString(0, 2, oled_buffer, 16);

        mspm0_delay_ms(100);
    }
}
