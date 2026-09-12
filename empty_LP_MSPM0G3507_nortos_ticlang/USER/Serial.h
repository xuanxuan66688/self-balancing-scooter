#ifndef __SERIAL_H
#define __SERIAL_H

#include <stdint.h>

/*
 * 串口 0（UART_0）标准收发模块
 *
 * 引脚（由 SysConfig 配置，见 main.syscfg 的 UART_0）：
 *   TX = PA10，RX = PA11，波特率 115200，8N1，无校验
 *
 * 发送：阻塞式，写完才返回。
 * 接收：中断 + 环形缓冲区，非阻塞，由 UART0_IRQHandler 写入。
 */

/* 接收环形缓冲区大小（需为 2 的幂） */
#define SERIAL_RX_BUF_SIZE 128

/* serialprintf 格式化缓冲区大小 */
#define SERIAL_PRINTF_BUF_SIZE 128

/* 初始化串口（使能 UART0 接收中断；外设配置已在 SYSCFG_DL_init 完成） */
void Serial_Init(void);

/* 阻塞发送一个字节 */
void Serial_SendByte(uint8_t data);

/* 阻塞发送指定长度的数据 */
void Serial_SendData(const uint8_t *buf, uint16_t len);

/* 阻塞发送字符串（\0 结尾） */
void Serial_SendString(const char *str);

/* printf 风格格式化输出到串口 0（阻塞），用法同 printf，返回写入字符数 */
int serialprintf(const char *fmt, ...);

/* 阻塞发送 16 位有符号整数（2 字节，小端） */
void Serial_SendInt16(int16_t data);

/* 阻塞发送 32 位有符号整数（4 字节，小端） */
void Serial_SendInt32(int32_t data);

/* 阻塞发送 float（4 字节，小端 IEEE754） */
void Serial_SendFloat(float data);

/* 接收缓冲区中当前可读的字节数 */
uint16_t Serial_Available(void);

/* 读一个字节；无数据时返回 -1 */
int16_t Serial_ReadByte(void);

/* 读最多 maxLen 字节到 buf，返回实际读到的字节数 */
uint16_t Serial_ReadData(uint8_t *buf, uint16_t maxLen);

/* 清空接收缓冲区 */
void Serial_FlushRX(void);

#endif /* __SERIAL_H */
