#include "ti_msp_dl_config.h"
#include "Encoder.h"

/*
 * 正交编码器计次（A 相跳变沿触发 + B 相电平判向，2 倍频）
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
 */

volatile int32_t Encoder_L_Count = 0;
volatile int32_t Encoder_R_Count = 0;

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

void Encoder_Init(void)
{
    Encoder_L_Count = 0;
    Encoder_R_Count = 0;

    /* GPIOA/GPIOB 中断都挂在 GROUP1（IRQ1），使能一次即可 */
    NVIC_EnableIRQ(GPIO_ENCODER_GPIOA_INT_IRQN);
    NVIC_EnableIRQ(GPIO_ENCODER_GPIOB_INT_IRQN);
}

/* 左编码器 A 相跳变沿处理 */
void Encoder_L_Update(void)
{
    uint8_t a = Encoder_L_ReadA();
    uint8_t b = Encoder_L_ReadB();

    if (a != b)
    {
        Encoder_L_Count++;      /* 正向 */
    }
    else
    {
        Encoder_L_Count--;      /* 负向 */
    }
}

/* 右编码器 A 相跳变沿处理 */
void Encoder_R_Update(void)
{
    uint8_t a = Encoder_R_ReadA();
    uint8_t b = Encoder_R_ReadB();

    if (a != b)
    {
        Encoder_R_Count++;      /* 正向 */
    }
    else
    {
        Encoder_R_Count--;      /* 负向 */
    }
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
