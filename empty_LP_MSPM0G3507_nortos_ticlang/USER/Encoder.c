#include "ti_msp_dl_config.h"
#include "Encoder.h"
#include "clock.h"          /* 使用 tick_ms 做测速采样定时 */

/* 每个编码器通道的脉冲累计值（在中断里自增） */
static volatile int32_t g_pulse[2] = {0, 0};

/* 测速采样：记录上次采样的时间与脉冲数 */
static uint32_t g_last_tick[2]  = {0, 0};
static int32_t  g_last_pulse[2] = {0, 0};
static int32_t  g_speed[2]      = {0, 0};   /* 单位：脉冲/秒 */

/**
 * @brief  编码器初始化
 *         使能 NVIC 中断，并启动两个输入捕获定时器
 * @return 无
 */
void Encoder_Init(void)
{
    NVIC_EnableIRQ(CAPTURE_0_INST_INT_IRQN);   /* TIMA1：电机A 编码器 */
    NVIC_EnableIRQ(CAPTURE_1_INST_INT_IRQN);   /* TIMG6：电机B 编码器 */

    DL_TimerA_startCounter(CAPTURE_0_INST);
    DL_TimerG_startCounter(CAPTURE_1_INST);
}

/**
 * @brief  获取电机转速（脉冲/秒）
 * @param  wheel  编码器通道：ENCODER_A 或 ENCODER_B
 * @return 转速（脉冲/秒），未到采样周期时返回上一次结果
 * @note   每 100ms 对脉冲数求一次差分，换算成每秒脉冲数，结果平滑稳定
 */
int32_t Encoder_GetSpeed(uint8_t wheel)
{
    uint32_t now = tick_ms;
    uint32_t dt  = now - g_last_tick[wheel];

    /* 每 100ms 更新一次速度 */
    if (dt >= 100)
    {
        int32_t delta = g_pulse[wheel] - g_last_pulse[wheel];
        g_speed[wheel] = (delta * 1000) / (int32_t)dt;   /* 脉冲/秒 */
        g_last_tick[wheel]  = now;
        g_last_pulse[wheel] = g_pulse[wheel];
    }

    return g_speed[wheel];
}

/**
 * @brief  获取电机转速（转/分钟）
 * @param  wheel  编码器通道：ENCODER_A 或 ENCODER_B
 * @return 转速（RPM）
 */
int32_t Encoder_GetRPM(uint8_t wheel)
{
    /* RPM = 脉冲/秒 × 60 ÷ 每圈脉冲数 */
    return (Encoder_GetSpeed(wheel) * 60) / ENCODER_PPR;
}

/* TIMA1 输入捕获中断：电机A 编码器（PB4），每来一个脉冲计数加一 */
void CAPTURE_0_INST_IRQHandler(void)
{
    switch (DL_TimerA_getPendingInterrupt(CAPTURE_0_INST))
    {
        case DL_TIMERA_IIDX_CC0_DN:
            g_pulse[ENCODER_A]++;
            break;
        default:
            break;
    }
}

/* TIMG6 输入捕获中断：电机B 编码器（PB26），每来一个脉冲计数加一 */
void CAPTURE_1_INST_IRQHandler(void)
{
    switch (DL_TimerG_getPendingInterrupt(CAPTURE_1_INST))
    {
        case DL_TIMERG_IIDX_CC0_DN:
            g_pulse[ENCODER_B]++;
            break;
        default:
            break;
    }
}
