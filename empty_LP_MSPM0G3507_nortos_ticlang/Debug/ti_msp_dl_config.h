/*
 * Copyright (c) 2023, Texas Instruments Incorporated - http://www.ti.com
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

/*
 *  ============ ti_msp_dl_config.h =============
 *  Configured MSPM0 DriverLib module declarations
 *
 *  DO NOT EDIT - This file is generated for the MSPM0G350X
 *  by the SysConfig tool.
 */
#ifndef ti_msp_dl_config_h
#define ti_msp_dl_config_h

#define CONFIG_MSPM0G350X
#define CONFIG_MSPM0G3507

#if defined(__ti_version__) || defined(__TI_COMPILER_VERSION__)
#define SYSCONFIG_WEAK __attribute__((weak))
#elif defined(__IAR_SYSTEMS_ICC__)
#define SYSCONFIG_WEAK __weak
#elif defined(__GNUC__)
#define SYSCONFIG_WEAK __attribute__((weak))
#endif

#include <ti/devices/msp/msp.h>
#include <ti/driverlib/driverlib.h>
#include <ti/driverlib/m0p/dl_core.h>

#ifdef __cplusplus
extern "C" {
#endif

/*
 *  ======== SYSCFG_DL_init ========
 *  Perform all required MSP DL initialization
 *
 *  This function should be called once at a point before any use of
 *  MSP DL.
 */


/* clang-format off */

#define POWER_STARTUP_DELAY                                                (16)


#define CPUCLK_FREQ                                                     32000000



/* Defines for PWM_0 */
#define PWM_0_INST                                                         TIMA0
#define PWM_0_INST_IRQHandler                                   TIMA0_IRQHandler
#define PWM_0_INST_INT_IRQN                                     (TIMA0_INT_IRQn)
#define PWM_0_INST_CLK_FREQ                                              1000000
/* GPIO defines for channel 0 */
#define GPIO_PWM_0_C0_PORT                                                 GPIOB
#define GPIO_PWM_0_C0_PIN                                         DL_GPIO_PIN_14
#define GPIO_PWM_0_C0_IOMUX                                      (IOMUX_PINCM31)
#define GPIO_PWM_0_C0_IOMUX_FUNC                     IOMUX_PINCM31_PF_TIMA0_CCP0
#define GPIO_PWM_0_C0_IDX                                    DL_TIMER_CC_0_INDEX
/* GPIO defines for channel 1 */
#define GPIO_PWM_0_C1_PORT                                                 GPIOA
#define GPIO_PWM_0_C1_PIN                                          DL_GPIO_PIN_7
#define GPIO_PWM_0_C1_IOMUX                                      (IOMUX_PINCM14)
#define GPIO_PWM_0_C1_IOMUX_FUNC                     IOMUX_PINCM14_PF_TIMA0_CCP1
#define GPIO_PWM_0_C1_IDX                                    DL_TIMER_CC_1_INDEX



/* Defines for TIMER_0 */
#define TIMER_0_INST                                                     (TIMA1)
#define TIMER_0_INST_IRQHandler                                 TIMA1_IRQHandler
#define TIMER_0_INST_INT_IRQN                                   (TIMA1_INT_IRQn)
#define TIMER_0_INST_LOAD_VALUE                                           (624U)




/* Defines for I2C_OLED */
#define I2C_OLED_INST                                                       I2C0
#define I2C_OLED_INST_IRQHandler                                 I2C0_IRQHandler
#define I2C_OLED_INST_INT_IRQN                                     I2C0_INT_IRQn
#define I2C_OLED_BUS_SPEED_HZ                                             400000
#define GPIO_I2C_OLED_SDA_PORT                                             GPIOA
#define GPIO_I2C_OLED_SDA_PIN                                      DL_GPIO_PIN_0
#define GPIO_I2C_OLED_IOMUX_SDA                                   (IOMUX_PINCM1)
#define GPIO_I2C_OLED_IOMUX_SDA_FUNC                    IOMUX_PINCM1_PF_I2C0_SDA
#define GPIO_I2C_OLED_SCL_PORT                                             GPIOA
#define GPIO_I2C_OLED_SCL_PIN                                      DL_GPIO_PIN_1
#define GPIO_I2C_OLED_IOMUX_SCL                                   (IOMUX_PINCM2)
#define GPIO_I2C_OLED_IOMUX_SCL_FUNC                    IOMUX_PINCM2_PF_I2C0_SCL

/* Defines for I2C_MPU6050 */
#define I2C_MPU6050_INST                                                    I2C1
#define I2C_MPU6050_INST_IRQHandler                              I2C1_IRQHandler
#define I2C_MPU6050_INST_INT_IRQN                                  I2C1_INT_IRQn
#define I2C_MPU6050_BUS_SPEED_HZ                                          400000
#define GPIO_I2C_MPU6050_SDA_PORT                                          GPIOB
#define GPIO_I2C_MPU6050_SDA_PIN                                   DL_GPIO_PIN_3
#define GPIO_I2C_MPU6050_IOMUX_SDA                               (IOMUX_PINCM16)
#define GPIO_I2C_MPU6050_IOMUX_SDA_FUNC                IOMUX_PINCM16_PF_I2C1_SDA
#define GPIO_I2C_MPU6050_SCL_PORT                                          GPIOB
#define GPIO_I2C_MPU6050_SCL_PIN                                   DL_GPIO_PIN_2
#define GPIO_I2C_MPU6050_IOMUX_SCL                               (IOMUX_PINCM15)
#define GPIO_I2C_MPU6050_IOMUX_SCL_FUNC                IOMUX_PINCM15_PF_I2C1_SCL


/* Defines for UART_0 */
#define UART_0_INST                                                        UART0
#define UART_0_INST_FREQUENCY                                           32000000
#define UART_0_INST_IRQHandler                                  UART0_IRQHandler
#define UART_0_INST_INT_IRQN                                      UART0_INT_IRQn
#define GPIO_UART_0_RX_PORT                                                GPIOA
#define GPIO_UART_0_TX_PORT                                                GPIOA
#define GPIO_UART_0_RX_PIN                                        DL_GPIO_PIN_11
#define GPIO_UART_0_TX_PIN                                        DL_GPIO_PIN_10
#define GPIO_UART_0_IOMUX_RX                                     (IOMUX_PINCM22)
#define GPIO_UART_0_IOMUX_TX                                     (IOMUX_PINCM21)
#define GPIO_UART_0_IOMUX_RX_FUNC                      IOMUX_PINCM22_PF_UART0_RX
#define GPIO_UART_0_IOMUX_TX_FUNC                      IOMUX_PINCM21_PF_UART0_TX
#define UART_0_BAUD_RATE                                                (115200)
#define UART_0_IBRD_32_MHZ_115200_BAUD                                      (17)
#define UART_0_FBRD_32_MHZ_115200_BAUD                                      (23)
/* Defines for UART_WIT */
#define UART_WIT_INST                                                      UART2
#define UART_WIT_INST_FREQUENCY                                         32000000
#define UART_WIT_INST_IRQHandler                                UART2_IRQHandler
#define UART_WIT_INST_INT_IRQN                                    UART2_INT_IRQn
#define GPIO_UART_WIT_RX_PORT                                              GPIOB
#define GPIO_UART_WIT_RX_PIN                                      DL_GPIO_PIN_16
#define GPIO_UART_WIT_IOMUX_RX                                   (IOMUX_PINCM33)
#define GPIO_UART_WIT_IOMUX_RX_FUNC                    IOMUX_PINCM33_PF_UART2_RX
#define UART_WIT_BAUD_RATE                                              (115200)
#define UART_WIT_IBRD_32_MHZ_115200_BAUD                                    (17)
#define UART_WIT_FBRD_32_MHZ_115200_BAUD                                    (23)





/* Defines for DMA_WIT */
#define DMA_WIT_CHAN_ID                                                      (0)
#define UART_WIT_INST_DMA_TRIGGER                            (DMA_UART2_RX_TRIG)


/* Port definition for Pin Group TEST_LED */
#define TEST_LED_PORT                                                    (GPIOB)

/* Defines for LED: GPIOB.22 with pinCMx 50 on package pin 21 */
#define TEST_LED_LED_PIN                                        (DL_GPIO_PIN_22)
#define TEST_LED_LED_IOMUX                                       (IOMUX_PINCM50)
/* Port definition for Pin Group GPIO_MPU6050 */
#define GPIO_MPU6050_PORT                                                (GPIOB)

/* Defines for PIN_MPU6050_INT: GPIOB.1 with pinCMx 13 on package pin 48 */
#define GPIO_MPU6050_PIN_MPU6050_INT_PIN                         (DL_GPIO_PIN_1)
#define GPIO_MPU6050_PIN_MPU6050_INT_IOMUX                       (IOMUX_PINCM13)
/* Port definition for Pin Group BIN */
#define BIN_PORT                                                         (GPIOB)

/* Defines for BIN1: GPIOB.6 with pinCMx 23 on package pin 58 */
#define BIN_BIN1_PIN                                             (DL_GPIO_PIN_6)
#define BIN_BIN1_IOMUX                                           (IOMUX_PINCM23)
/* Defines for BIN2: GPIOB.7 with pinCMx 24 on package pin 59 */
#define BIN_BIN2_PIN                                             (DL_GPIO_PIN_7)
#define BIN_BIN2_IOMUX                                           (IOMUX_PINCM24)
/* Port definition for Pin Group AIN */
#define AIN_PORT                                                         (GPIOB)

/* Defines for AIN1: GPIOB.9 with pinCMx 26 on package pin 61 */
#define AIN_AIN1_PIN                                             (DL_GPIO_PIN_9)
#define AIN_AIN1_IOMUX                                           (IOMUX_PINCM26)
/* Defines for AIN2: GPIOB.10 with pinCMx 27 on package pin 62 */
#define AIN_AIN2_PIN                                            (DL_GPIO_PIN_10)
#define AIN_AIN2_IOMUX                                           (IOMUX_PINCM27)
/* Defines for L_ENCODER_A: GPIOB.26 with pinCMx 57 on package pin 28 */
#define GPIO_ENCODER_L_ENCODER_A_PORT                                    (GPIOB)
// pins affected by this interrupt request:["L_ENCODER_A","R_ENCODER_A","R_ENCODER_B"]
#define GPIO_ENCODER_GPIOB_INT_IRQN                             (GPIOB_INT_IRQn)
#define GPIO_ENCODER_GPIOB_INT_IIDX             (DL_INTERRUPT_GROUP1_IIDX_GPIOB)
#define GPIO_ENCODER_L_ENCODER_A_IIDX                       (DL_GPIO_IIDX_DIO26)
#define GPIO_ENCODER_L_ENCODER_A_PIN                            (DL_GPIO_PIN_26)
#define GPIO_ENCODER_L_ENCODER_A_IOMUX                           (IOMUX_PINCM57)
/* Defines for L_ENCODER_B: GPIOA.13 with pinCMx 35 on package pin 6 */
#define GPIO_ENCODER_L_ENCODER_B_PORT                                    (GPIOA)
// pins affected by this interrupt request:["L_ENCODER_B"]
#define GPIO_ENCODER_GPIOA_INT_IRQN                             (GPIOA_INT_IRQn)
#define GPIO_ENCODER_GPIOA_INT_IIDX             (DL_INTERRUPT_GROUP1_IIDX_GPIOA)
#define GPIO_ENCODER_L_ENCODER_B_IIDX                       (DL_GPIO_IIDX_DIO13)
#define GPIO_ENCODER_L_ENCODER_B_PIN                            (DL_GPIO_PIN_13)
#define GPIO_ENCODER_L_ENCODER_B_IOMUX                           (IOMUX_PINCM35)
/* Defines for R_ENCODER_A: GPIOB.4 with pinCMx 17 on package pin 52 */
#define GPIO_ENCODER_R_ENCODER_A_PORT                                    (GPIOB)
#define GPIO_ENCODER_R_ENCODER_A_IIDX                        (DL_GPIO_IIDX_DIO4)
#define GPIO_ENCODER_R_ENCODER_A_PIN                             (DL_GPIO_PIN_4)
#define GPIO_ENCODER_R_ENCODER_A_IOMUX                           (IOMUX_PINCM17)
/* Defines for R_ENCODER_B: GPIOB.5 with pinCMx 18 on package pin 53 */
#define GPIO_ENCODER_R_ENCODER_B_PORT                                    (GPIOB)
#define GPIO_ENCODER_R_ENCODER_B_IIDX                        (DL_GPIO_IIDX_DIO5)
#define GPIO_ENCODER_R_ENCODER_B_PIN                             (DL_GPIO_PIN_5)
#define GPIO_ENCODER_R_ENCODER_B_IOMUX                           (IOMUX_PINCM18)

/* clang-format on */

void SYSCFG_DL_init(void);
void SYSCFG_DL_initPower(void);
void SYSCFG_DL_GPIO_init(void);
void SYSCFG_DL_SYSCTL_init(void);
void SYSCFG_DL_PWM_0_init(void);
void SYSCFG_DL_TIMER_0_init(void);
void SYSCFG_DL_I2C_OLED_init(void);
void SYSCFG_DL_I2C_MPU6050_init(void);
void SYSCFG_DL_UART_0_init(void);
void SYSCFG_DL_UART_WIT_init(void);
void SYSCFG_DL_DMA_init(void);


bool SYSCFG_DL_saveConfiguration(void);
bool SYSCFG_DL_restoreConfiguration(void);

#ifdef __cplusplus
}
#endif

#endif /* ti_msp_dl_config_h */
