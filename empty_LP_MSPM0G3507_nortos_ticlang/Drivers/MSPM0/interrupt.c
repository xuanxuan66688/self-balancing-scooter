#include "ti_msp_dl_config.h"
#include "interrupt.h"
#include "clock.h"
#include "mpu6050.h"
#include "bno08x_uart_rvc.h"
#include "wit.h"
#include "vl53l0x.h"
#include "lsm6dsv16x.h"
#include "../../USER/Encoder.h"

uint8_t enable_group1_irq = 0;

void Interrupt_Init(void)
{
    if(enable_group1_irq)
    {
        NVIC_EnableIRQ(1);
    }
}

void SysTick_Handler(void)
{
    tick_ms++;
}

#if defined UART_BNO08X_INST_IRQHandler
void UART_BNO08X_INST_IRQHandler(void)
{
    uint8_t checkSum = 0;
    extern uint8_t bno08x_dmaBuffer[19];

    DL_DMA_disableChannel(DMA, DMA_BNO08X_CHAN_ID);
    uint8_t rxSize = 18 - DL_DMA_getTransferSize(DMA, DMA_BNO08X_CHAN_ID);

    if(DL_UART_isRXFIFOEmpty(UART_BNO08X_INST) == false)
        bno08x_dmaBuffer[rxSize++] = DL_UART_receiveData(UART_BNO08X_INST);

    for(int i=2; i<=14; i++)
        checkSum += bno08x_dmaBuffer[i];

    if((rxSize == 19) && (bno08x_dmaBuffer[0] == 0xAA) && (bno08x_dmaBuffer[1] == 0xAA) && (checkSum == bno08x_dmaBuffer[18]))
    {
        bno08x_data.index = bno08x_dmaBuffer[2];
        bno08x_data.yaw = (int16_t)((bno08x_dmaBuffer[4]<<8)|bno08x_dmaBuffer[3]) / 100.0;
        bno08x_data.pitch = (int16_t)((bno08x_dmaBuffer[6]<<8)|bno08x_dmaBuffer[5]) / 100.0;
        bno08x_data.roll = (int16_t)((bno08x_dmaBuffer[8]<<8)|bno08x_dmaBuffer[7]) / 100.0;
        bno08x_data.ax = (bno08x_dmaBuffer[10]<<8)|bno08x_dmaBuffer[9];
        bno08x_data.ay = (bno08x_dmaBuffer[12]<<8)|bno08x_dmaBuffer[11];
        bno08x_data.az = (bno08x_dmaBuffer[14]<<8)|bno08x_dmaBuffer[13];
    }
    
    uint8_t dummy[4];
    DL_UART_drainRXFIFO(UART_BNO08X_INST, dummy, 4);

    DL_DMA_setDestAddr(DMA, DMA_BNO08X_CHAN_ID, (uint32_t) &bno08x_dmaBuffer[0]);
    DL_DMA_setTransferSize(DMA, DMA_BNO08X_CHAN_ID, 18);
    DL_DMA_enableChannel(DMA, DMA_BNO08X_CHAN_ID);
}
#endif

#if defined UART_WIT_INST_IRQHandler
void UART_WIT_INST_IRQHandler(void)
{
    uint8_t checkSum;
    extern uint8_t wit_dmaBuffer[33];

    wit_rx_count++;   /* 调试：记录中断触发次数 */

    DL_DMA_disableChannel(DMA, DMA_WIT_CHAN_ID);
    uint8_t rxSize = 32 - DL_DMA_getTransferSize(DMA, DMA_WIT_CHAN_ID);

    if(DL_UART_isRXFIFOEmpty(UART_WIT_INST) == false)
        wit_dmaBuffer[rxSize++] = DL_UART_receiveData(UART_WIT_INST);

    /* 逐字节扫描 0x55 帧头，动态对齐。
     * 原实现假设帧固定从缓冲区 0/11/22 字节开始，但 DMA 缓冲(32B)不是帧长(11B)的
     * 整数倍，缓冲区起点会相对帧流漂移，导致部分帧被跳过。改为扫描帧头后不再依赖对齐。 */
    {
        uint8_t idx = 0;
        while(idx + 11 <= rxSize)
        {
            if(wit_dmaBuffer[idx] != 0x55)
            {
                idx++;               /* 不是帧头，向后找 */
                continue;
            }

            checkSum = 0;
            for(int i = 0; i < 10; i++)
                checkSum += wit_dmaBuffer[idx + i];

            if(checkSum != wit_dmaBuffer[idx + 10])
            {
                idx++;               /* 校验和不匹配，继续找下一个 0x55 */
                continue;
            }

            /* 校验通过，解析该帧类型 */
            if(wit_dmaBuffer[idx + 1] == 0x51)
            {
                wit_data.ax = (int16_t)((wit_dmaBuffer[idx + 3]<<8)|wit_dmaBuffer[idx + 2]) / 2.048; //mg
                wit_data.ay = (int16_t)((wit_dmaBuffer[idx + 5]<<8)|wit_dmaBuffer[idx + 4]) / 2.048; //mg
                wit_data.az = (int16_t)((wit_dmaBuffer[idx + 7]<<8)|wit_dmaBuffer[idx + 6]) / 2.048; //mg
                wit_data.temperature =  (int16_t)((wit_dmaBuffer[idx + 9]<<8)|wit_dmaBuffer[idx + 8]) / 100.0; //°C
            }
            else if(wit_dmaBuffer[idx + 1] == 0x52)
            {
                wit_data.gx = (int16_t)((wit_dmaBuffer[idx + 3]<<8)|wit_dmaBuffer[idx + 2]) / 16.384; //°/S
                wit_data.gy = (int16_t)((wit_dmaBuffer[idx + 5]<<8)|wit_dmaBuffer[idx + 4]) / 16.384; //°/S
                wit_data.gz = (int16_t)((wit_dmaBuffer[idx + 7]<<8)|wit_dmaBuffer[idx + 6]) / 16.384; //°/S
            }
            else if(wit_dmaBuffer[idx + 1] == 0x53)
            {
                wit_angle_count++;   /* 调试：统计成功解析的角度帧数 */
                wit_data.roll  = (int16_t)((wit_dmaBuffer[idx + 3]<<8)|wit_dmaBuffer[idx + 2]) / 32768.0 * 180.0; //°
                wit_data.pitch = (int16_t)((wit_dmaBuffer[idx + 5]<<8)|wit_dmaBuffer[idx + 4]) / 32768.0 * 180.0; //°
                wit_data.yaw   = (int16_t)((wit_dmaBuffer[idx + 7]<<8)|wit_dmaBuffer[idx + 6]) / 32768.0 * 180.0; //°
                wit_data.version = (int16_t)((wit_dmaBuffer[idx + 9]<<8)|wit_dmaBuffer[idx + 8]);
            }

            idx += 11;               /* 跳过已解析的整帧 */
        }
    }
    
    uint8_t dummy[4];
    DL_UART_drainRXFIFO(UART_WIT_INST, dummy, 4);

    DL_DMA_setDestAddr(DMA, DMA_WIT_CHAN_ID, (uint32_t) &wit_dmaBuffer[0]);
    DL_DMA_setTransferSize(DMA, DMA_WIT_CHAN_ID, 32);
    DL_DMA_enableChannel(DMA, DMA_WIT_CHAN_ID);
}
#endif

void GROUP1_IRQHandler(void)
{
    switch (DL_Interrupt_getPendingGroup(DL_INTERRUPT_GROUP_1)) {
        #if defined GPIO_ENCODER_GPIOA_INT_IIDX
        case GPIO_ENCODER_GPIOA_INT_IIDX:
            switch (DL_GPIO_getPendingInterrupt(GPIOA))
            {
                /* 左编码器 B 相（PA13）：仅清中断，不计数 */
                case GPIO_ENCODER_L_ENCODER_B_IIDX:
                    DL_GPIO_clearInterruptStatus(GPIOA, GPIO_ENCODER_L_ENCODER_B_PIN);
                    break;

                default:
                    break;
            }
            break;
        #endif

        #if defined GPIO_ENCODER_GPIOB_INT_IIDX
        case GPIO_ENCODER_GPIOB_INT_IIDX:


            // if(DL_GPIO_getPendingInterrupt(GPIOB) ==  GPIO_ENCODER_L_ENCODER_A_IIDX)
            // {
            //     DL_GPIO_clearInterruptStatus(GPIOB, GPIO_ENCODER_L_ENCODER_A_PIN);
            //      Encoder_L_Update();
            // }
            // else if(DL_GPIO_getPendingInterrupt(GPIOB) ==  GPIO_ENCODER_R_ENCODER_A_IIDX)
            // {
            //     DL_GPIO_clearInterruptStatus(GPIOB, GPIO_ENCODER_R_ENCODER_A_PIN);
            //     Encoder_R_Update();
            //     }
            // else if(DL_GPIO_getPendingInterrupt(GPIOB) ==  GPIO_ENCODER_R_ENCODER_B_IIDX){ DL_GPIO_clearInterruptStatus(GPIOB, GPIO_ENCODER_R_ENCODER_B_PIN);}
            
            switch (DL_GPIO_getPendingInterrupt(GPIOB))
            {
                /* 左编码器 A 相（PB26）：跳变沿计数 */
                case GPIO_ENCODER_L_ENCODER_A_IIDX:
                   DL_GPIO_clearInterruptStatus(GPIOB, GPIO_ENCODER_L_ENCODER_A_PIN);
                     Encoder_L_Update();
                    break;

                /* 右编码器 A 相（PB4）：跳变沿计数 */
                case GPIO_ENCODER_R_ENCODER_A_IIDX:
                     DL_GPIO_clearInterruptStatus(GPIOB, GPIO_ENCODER_R_ENCODER_A_PIN);
                    Encoder_R_Update();
                    break;

                /* 右编码器 B 相（PB5）：仅清中断，不计数 */
                case GPIO_ENCODER_R_ENCODER_B_IIDX:
                    DL_GPIO_clearInterruptStatus(GPIOB, GPIO_ENCODER_R_ENCODER_B_PIN);
                    break;

                default:
                    break;
            }
            break;
        #endif

        #if defined GPIO_MULTIPLE_GPIOA_INT_IIDX
        case GPIO_MULTIPLE_GPIOA_INT_IIDX:
            switch (DL_GPIO_getPendingInterrupt(GPIOA))
            {
                #if (defined GPIO_MPU6050_PORT) && (GPIO_MPU6050_PORT == GPIOA)
                case GPIO_MPU6050_PIN_MPU6050_INT_IIDX:
                    Read_Quad();
                    break;
                #endif

                #if (defined GPIO_LSM6DSV16X_PORT) && (GPIO_LSM6DSV16X_PORT == GPIOA)
                case GPIO_LSM6DSV16X_PIN_LSM6DSV16X_INT_IIDX:
                    Read_LSM6DSV16X();
                    break;
                #endif

                #if (defined GPIO_VL53L0X_PIN_VL53L0X_GPIO1_PORT) && (GPIO_VL53L0X_PIN_VL53L0X_GPIO1_PORT == GPIOA)
                case GPIO_VL53L0X_PIN_VL53L0X_GPIO1_IIDX:
                    Read_VL53L0X();
                    break;
                #endif

                default:
                    break;
            }
        #endif

        #if defined GPIO_MULTIPLE_GPIOB_INT_IIDX
        case GPIO_MULTIPLE_GPIOB_INT_IIDX:
            switch (DL_GPIO_getPendingInterrupt(GPIOB))
            {
                #if (defined GPIO_MPU6050_PORT) && (GPIO_MPU6050_PORT == GPIOB)
                case GPIO_MPU6050_PIN_MPU6050_INT_IIDX:
                    Read_Quad();
                    break;
                #endif

                #if (defined GPIO_LSM6DSV16X_PORT) && (GPIO_LSM6DSV16X_PORT == GPIOB)
                case GPIO_LSM6DSV16X_PIN_LSM6DSV16X_INT_IIDX:
                    Read_LSM6DSV16X();
                    break;
                #endif

                #if (defined GPIO_VL53L0X_PIN_VL53L0X_GPIO1_PORT) && (GPIO_VL53L0X_PIN_VL53L0X_GPIO1_PORT == GPIOB)
                case GPIO_VL53L0X_PIN_VL53L0X_GPIO1_IIDX:
                    Read_VL53L0X();
                    break;
                #endif

                default:
                    break;
            }
        #endif

        #if defined GPIO_MPU6050_INT_IIDX
            case GPIO_MPU6050_INT_IIDX:
                Read_Quad();
                break;
        #endif

        #if defined GPIO_LSM6DSV16X_INT_IIDX
            case GPIO_LSM6DSV16X_INT_IIDX:
                Read_LSM6DSV16X();
                break;
        #endif

        #if defined GPIO_VL53L0X_INT_IIDX
            case GPIO_VL53L0X_INT_IIDX:
                Read_VL53L0X();
                break;
        #endif
    }
}
