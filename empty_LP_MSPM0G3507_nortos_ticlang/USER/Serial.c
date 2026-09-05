#include "Serial.h"
#include "ti_msp_dl_config.h"

/* 接收环形缓冲区（中断写入，主循环读取） */
static volatile uint8_t  serial_rx_buf[SERIAL_RX_BUF_SIZE];
static volatile uint16_t serial_rx_head = 0;   /* 写指针 */
static volatile uint16_t serial_rx_tail = 0;   /* 读指针 */

#define SERIAL_RX_MASK (SERIAL_RX_BUF_SIZE - 1)

void Serial_Init(void)
{
    /* 外设及 RX 中断已在 SYSCFG_DL_init() 配置，这里只使能 NVIC */
    NVIC_ClearPendingIRQ(UART_0_INST_INT_IRQN);
    NVIC_EnableIRQ(UART_0_INST_INT_IRQN);
}

void Serial_SendByte(uint8_t data)
{
    /* 等待上一次发送完成，再写下一个字节 */
    while (DL_UART_Main_isBusy(UART_0_INST));
    DL_UART_Main_transmitData(UART_0_INST, data);
}

void Serial_SendData(const uint8_t *buf, uint16_t len)
{
    while (len--) {
        Serial_SendByte(*buf++);
    }
}

void Serial_SendString(const char *str)
{
    while (*str) {
        Serial_SendByte((uint8_t)(*str++));
    }
}

void Serial_SendInt16(int16_t data)
{
    uint16_t u = (uint16_t)data;
    uint8_t  b[2] = { (uint8_t)(u & 0xFF), (uint8_t)(u >> 8) };
    Serial_SendData(b, 2);
}

void Serial_SendInt32(int32_t data)
{
    uint32_t u = (uint32_t)data;
    uint8_t  b[4] = {
        (uint8_t)(u & 0xFF),
        (uint8_t)((u >> 8) & 0xFF),
        (uint8_t)((u >> 16) & 0xFF),
        (uint8_t)((u >> 24) & 0xFF)
    };
    Serial_SendData(b, 4);
}

void Serial_SendFloat(float data)
{
    union {
        float   f;
        uint8_t b[4];
    } u;
    u.f = data;
    Serial_SendData(u.b, 4);
}

uint16_t Serial_Available(void)
{
    return (uint16_t)((serial_rx_head - serial_rx_tail) & SERIAL_RX_MASK);
}

int16_t Serial_ReadByte(void)
{
    if (serial_rx_head == serial_rx_tail) {
        return -1;
    }
    uint8_t data = serial_rx_buf[serial_rx_tail];
    serial_rx_tail = (serial_rx_tail + 1) & SERIAL_RX_MASK;
    return data;
}

uint16_t Serial_ReadData(uint8_t *buf, uint16_t maxLen)
{
    uint16_t n = 0;
    while (n < maxLen) {
        int16_t c = Serial_ReadByte();
        if (c < 0) {
            break;
        }
        buf[n++] = (uint8_t)c;
    }
    return n;
}

void Serial_FlushRX(void)
{
    serial_rx_tail = serial_rx_head;
}

/* 串口 0 接收中断：把收到的字节写入环形缓冲区（缓冲区满则丢弃） */
void UART0_IRQHandler(void)
{
    switch (DL_UART_Main_getPendingInterrupt(UART_0_INST)) {
        case DL_UART_MAIN_IIDX_RX: {
            uint16_t next = (serial_rx_head + 1) & SERIAL_RX_MASK;
            uint8_t data = DL_UART_Main_receiveData(UART_0_INST);
            if (next != serial_rx_tail) {   /* 未满才存入 */
                serial_rx_buf[serial_rx_head] = data;
                serial_rx_head = next;
            }
            break;
        }
        default:
            break;
    }
}
