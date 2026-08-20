#ifndef __ENCODER_H
#define __ENCODER_H

#include <stdint.h>

/* 编码器通道选择（与 MOTOR.h 的电机编号保持一致） */
#define ENCODER_B       0    /* 电机B 编码器，接 CAPTURE_1（PB26 / TIMG6） */
#define ENCODER_A       1    /* 电机A 编码器，接 CAPTURE_0（PB4 / TIMA1）  */

/* 编码器线数：电机每转一圈，单相输出脉冲数，请按实际编码器修改 */
#define ENCODER_PPR     20

/* 初始化编码器：使能 NVIC 中断并启动两个输入捕获定时器 */
void Encoder_Init(void);

/* 获取电机转速（脉冲/秒），wheel 取 ENCODER_A 或 ENCODER_B */
int32_t Encoder_GetSpeed(uint8_t wheel);

/* 获取电机转速（转/分钟），wheel 取 ENCODER_A 或 ENCODER_B */
int32_t Encoder_GetRPM(uint8_t wheel);

#endif /* __ENCODER_H */
