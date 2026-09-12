#include "ti_msp_dl_config.h"
#include "Encoder.h"
#include "clock.h"

/*
 * 正交编码器计次 + T 法测速（A 相跳变沿触发 + B 相电平判向，2 倍频）
 *
 * 判向规则：
 *   A 相上升沿 & B 相为低 -> 正向 +1
 *   A 相下降沿 & B 相为高 -> 正向 +1
 *   A 相上升沿 & B 相为高 -> 负向 -1
 *   A 相下降沿 & B 相为低 -> 负向 -1
 *
 * 上式等价于：在 A 相跳变瞬间，A != B 则正向 +1，A == B 则负向 -1。
 *
 * 左编码器: A = PB26, B = PA13
 * 右编码器: A = PB4,  B = PB5
 *
 * T 法测速：记录相邻两个计数边沿的微秒时间戳，相减得到周期 T，
 * 速度 = 1 / T（计数/秒）。低速时 T 大、测量精度高，适合低速测速。
 */

/* 判停阈值：超过该时间（us）没有新边沿，则认为电机已停止 */
#define ENCODER_STALL_TIMEOUT_US   (100000UL)   /* 100 ms */

volatile int32_t Encoder_L_Count = 0;
volatile int32_t Encoder_R_Count = 0;

/* 旋转方向：1-正向，-1-反向 */
static volatile int8_t direction_L = 1;
static volatile int8_t direction_R = 1;

/* T 法测速相关：上次边沿时间、相邻边沿周期、有效性标志 */
static volatile uint32_t g_L_lastTimeUs = 0;
static volatile uint32_t g_L_periodUs   = 0;
static volatile uint8_t  g_L_valid      = 0;

static volatile uint32_t g_R_lastTimeUs = 0;
static volatile uint32_t g_R_periodUs   = 0;
static volatile uint8_t  g_R_valid      = 0;

/* 读取左编码器 A 相电平 */
static uint8_t Encoder_L_ReadA(void)
{
    return DL_GPIO_readPins(GPIO_ENCODER_L_ENCODER_A_PORT,
                GPIO_ENCODER_L_ENCODER_A_PIN) ? 1U : 0U;
}

/* 读取左编码器 B 相电平 */
static uint8_t Encoder_L_ReadB(void)
{
    return DL_GPIO_readPins(GPIO_ENCODER_L_ENCODER_B_PORT,
                GPIO_ENCODER_L_ENCODER_B_PIN) ? 1U : 0U;
}

/* 读取右编码器 A 相电平 */
static uint8_t Encoder_R_ReadA(void)
{
    return DL_GPIO_readPins(GPIO_ENCODER_R_ENCODER_A_PORT,
                GPIO_ENCODER_R_ENCODER_A_PIN) ? 1U : 0U;
}

/* 读取右编码器 B 相电平 */
static uint8_t Encoder_R_ReadB(void)
{
    return DL_GPIO_readPins(GPIO_ENCODER_R_ENCODER_B_PORT,
                GPIO_ENCODER_R_ENCODER_B_PIN) ? 1U : 0U;
}

/*
 * 基于 SysTick 的微秒级时间戳
 * SysTick 每 1ms（32000 个 CPU 时钟）中断一次累加 tick_ms，
 * 24bit 递减计数器提供 1ms 内的细粒度（1us = 32 个 CPU 时钟）。
 */
static uint32_t Encoder_GetTimeUs(void)
{
    uint32_t period = DL_SYSTICK_getPeriod();   /* 32000 */
    uint32_t ms1, ms2, val;

    do
    {
        ms1 = tick_ms;
        val = DL_SYSTICK_getValue();            /* 当前递减计数值 */
        ms2 = tick_ms;
    } while (ms1 != ms2);                       /* 跨 tick 重读，避免拼接错乱 */

    return ms1 * 1000UL + (period - val) / (CPUCLK_FREQ / 1000000UL);
}

void Encoder_Init(void)
{
    Encoder_L_Count = 0;
    Encoder_R_Count = 0;

    g_L_lastTimeUs = 0;
    g_L_periodUs   = 0;
    g_L_valid      = 0;
    g_R_lastTimeUs = 0;
    g_R_periodUs   = 0;
    g_R_valid      = 0;

    /* GPIOA/GPIOB 中断都挂在 GROUP1（IRQ1），使能一次即可 */
   NVIC_EnableIRQ(GPIO_ENCODER_GPIOA_INT_IRQN);
    NVIC_EnableIRQ(GPIO_ENCODER_GPIOB_INT_IRQN);
}

/* 左编码器 A 相跳变沿处理，在中断响应函数中处理 */
void Encoder_L_Update(void)
{
    uint8_t a = Encoder_L_ReadA();
    uint8_t b = Encoder_L_ReadB();
    uint32_t now;

    if (a == 1)
    {
        if(b==0)
        {
            Encoder_L_Count++;      /* 正向 */
            direction_L = 1;
         }
         else {
         
             Encoder_L_Count--;      /* 负向 */
        direction_L = -1;
         }
        
       
    }
    else
    {
         if(b==1)
        {
            Encoder_L_Count++;      /* 正向 */
            direction_L = 1;
         }
         else {
         
             Encoder_L_Count--;      /* 负向 */
        direction_L = -1;
         }
    }

    /* T 法：记录本次边沿时间，与上次边沿时间相减得到周期 */
    now = Encoder_GetTimeUs();
    if (g_L_valid)
    {
        g_L_periodUs = now - g_L_lastTimeUs;
    }
    g_L_lastTimeUs = now;
    g_L_valid = 1;
}

/* 右编码器 A 相跳变沿处理，在中断响应函数中处理 */
void Encoder_R_Update(void)
{
    uint8_t a = Encoder_R_ReadA();
    uint8_t b = Encoder_R_ReadB();
    uint32_t now;

    if (a !=b)
    {
            Encoder_R_Count++;      /* 正向 */
            direction_R = 1;
    }
    else
    {
          Encoder_R_Count--;      /* 负向 */
        direction_R = -1;
         
    }

    /* T 法：记录本次边沿时间，与上次边沿时间相减得到周期 */
    now = Encoder_GetTimeUs();
    if (g_R_valid)
    {
        g_R_periodUs = now - g_R_lastTimeUs;
    }
    g_R_lastTimeUs = now;
    g_R_valid = 1;
}

int32_t Encoder_Read_L(void)
{
    return Encoder_L_Count;
}

int32_t Encoder_Read_R(void)
{
    return Encoder_R_Count;
}

void Encoder_Clear_L(void)
{
    Encoder_L_Count = 0;
}

void Encoder_Clear_R(void)
{
    Encoder_R_Count = 0;
}

/* 左编码器相邻边沿周期（us），无有效测量时返回 0 */
uint32_t Encoder_PeriodUs_L(void)
{
    return g_L_periodUs;
}

/* 右编码器相邻边沿周期（us），无有效测量时返回 0 */
uint32_t Encoder_PeriodUs_R(void)
{
    return g_R_periodUs;
}

/*
 * T 法测速：速度 = 1000000 / 周期（计数/秒），带方向符号的整数。
 * 超过判停阈值没有新边沿则返回 0（视为停止）。
 */
int32_t Encoder_Speed_L(void)
{
    if (!g_L_valid || g_L_periodUs == 0)
    {
        return 0;
    }
    if ((Encoder_GetTimeUs() - g_L_lastTimeUs) > ENCODER_STALL_TIMEOUT_US)
    {
        return 0;    /* 已停止 */
    }
    return (int32_t)direction_L * (int32_t)(1000000UL / g_L_periodUs);
}

int32_t Encoder_Speed_R(void)
{
    if (!g_R_valid || g_R_periodUs == 0)
    {
        return 0;
    }
    if ((Encoder_GetTimeUs() - g_R_lastTimeUs) > ENCODER_STALL_TIMEOUT_US)
    {
        return 0;    /* 已停止 */
    }
    return (int32_t)direction_R * (int32_t)(1000000UL / g_R_periodUs);
}
