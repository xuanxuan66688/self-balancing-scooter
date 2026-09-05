#ifndef __ENCODER_H
#define __ENCODER_H

#include <stdint.h>

/*
 * 正交编码器计次模块（A 相跳变沿触发 + B 相电平判向，2 倍频）
 *
 * 引脚（由 SysConfig 配置，见 main.syscfg 的 GPIO_ENCODER）：
 *   左编码器: A = PB26, B = PA13
 *   右编码器: A = PB4,  B = PB5
 *
 * 计数为有符号值：正转递增，反转递减。
 */

/* 左轮编码器计数值 */
extern volatile int32_t Encoder_L_Count;
/* 右轮编码器计数值 */
extern volatile int32_t Encoder_R_Count;

/* 初始化编码器（读取初始相位，并使能 GROUP1 中断） */
void Encoder_Init(void);

/* 读取计数值 */
int32_t Encoder_Read_L(void);
int32_t Encoder_Read_R(void);

/* 清零计数值 */
void Encoder_Clear_L(void);
void Encoder_Clear_R(void);

/* T 法测速：返回最近一次相邻编码器边沿的周期（单位 us；无有效测量返回 0） */
uint32_t Encoder_PeriodUs_L(void);
uint32_t Encoder_PeriodUs_R(void);

/* T 法测速：返回速度（计数/秒，带符号整数，正转正、反转负；停止返回 0） */
int32_t Encoder_Speed_L(void);
int32_t Encoder_Speed_R(void);

/* 相位更新函数，由 GROUP1_IRQHandler 调用，用户无需直接调用 */
void Encoder_L_Update(void);
void Encoder_R_Update(void);

#endif /* __ENCODER_H */
