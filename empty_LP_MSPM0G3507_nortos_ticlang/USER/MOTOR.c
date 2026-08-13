#include "ti_msp_dl_config.h"
#include "MOTOR.h"
/**
 * @brief  设置电机转动方向（H 桥：AIN1/AIN2）
 * @param  dir  方向选择
 *              - 0：正转
 *              - 1：反转
 * @param  wheel  电机选择
 *              - 0：电机B
 *              - 1：电机A
 * @return 无
 */
void MOTOR_Dir(uint8_t dir, uint8_t wheel)
{
    if (wheel == 0)
    {
        if (dir == 0)
        {
             DL_GPIO_clearPins(BIN_PORT, BIN_BIN1_PIN);
            DL_GPIO_setPins(BIN_PORT, BIN_BIN2_PIN);
        }
        else
        {
            DL_GPIO_setPins(BIN_PORT, BIN_BIN1_PIN);
            DL_GPIO_clearPins(BIN_PORT, BIN_BIN2_PIN);
           
        }
    }
    else
    {
        if (dir == 0)
        {
            DL_GPIO_setPins(AIN_PORT, AIN_AIN1_PIN);
            DL_GPIO_clearPins(AIN_PORT, AIN_AIN2_PIN);
        }
        else
        {
            DL_GPIO_clearPins(AIN_PORT, AIN_AIN1_PIN);
            DL_GPIO_setPins(AIN_PORT, AIN_AIN2_PIN);
        }
    }
}
/**
 * @brief  设置电机占空比（PWM：AIN1/AIN2）
 * @param  duty  占空比，范围 -100~100
 * @param  wheel  电机选择
 *              - 0：电机B
 *              - 1：电机A
 * @return 无
 */
void MOTOR_duty(int16_t duty, uint8_t wheel)
{
    // 输出限幅
    if (duty > 100)
    {
        duty = 100;
    }
    else if (duty < -100)
    {
        duty = -100;
    }

    if (wheel == 0)
    {
        if (duty < 0)
        {
            MOTOR_Dir(1, 0);
            duty = -duty;
            duty = 100 - duty;
        }
        else
        {
            MOTOR_Dir(0, 0);
            duty = 100 - duty;
        }
        DL_TimerA_setCaptureCompareValue(PWM_0_INST, duty, DL_TIMER_CC_0_INDEX);
    }
    else
    {
        if (duty < 0)
        {
            MOTOR_Dir(1, 1);
            duty = -duty;
            duty = 100 - duty;
        }
        else
        {
            MOTOR_Dir(0, 1);
            duty = 100 - duty;
        }
        DL_TimerA_setCaptureCompareValue(PWM_0_INST, duty, DL_TIMER_CC_1_INDEX);
    }
}
