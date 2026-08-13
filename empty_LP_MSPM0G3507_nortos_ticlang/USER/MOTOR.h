#ifndef __MOTOR_H
#define __MOTOR_H

#include <stdint.h>

/* 电机选择 */
#define MOTOR_B         0    /* 电机B：BIN1/BIN2 */
#define MOTOR_A         1    /* 电机A：AIN1/AIN2 */

/* 转动方向 */
#define MOTOR_FORWARD   0    /* 正转 */
#define MOTOR_BACKWARD  1    /* 反转 */

/* PWM 满占空比，对应 SysConfig 里 PWM_0 的 period = 1000 */
#define MOTOR_MAX_DUTY  1000
void MOTOR_Dir(uint8_t dir, uint8_t wheel);
void MOTOR_duty(int16_t duty, uint8_t wheel);
#endif /* __MOTOR_H */
