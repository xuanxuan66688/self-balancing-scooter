#include "WITshow.h"
#include "main.h"
#include "ti_msp_dl_config.h"

/* 显示 WIT 姿态角（单位：度，1 位小数）：P=俯仰 pitch，R=横滚 roll，Y=偏航 yaw */
void WIT_Show_Data(void)
{
    /* 显示 WIT 姿态角（单位：度，保留 1 位小数）
     *  P = 俯仰 pitch，R = 横滚 roll，Y = 偏航 yaw
     *  第 4 行 AC 为成功解析的角度(0x53)帧计数，用于确认解析正常，调试确认后可删除*/
    OLED_ShowString(0, 0, (uint8_t *)"P:", 16);
    OLED_ShowSignedFloat(16, 0, wit_data.pitch, 1, 16);

    OLED_ShowString(0, 2, (uint8_t *)"R:", 16);
    OLED_ShowSignedFloat(16, 2, wit_data.roll, 1, 16);

    OLED_ShowString(0, 4, (uint8_t *)"Y:", 16);
    OLED_ShowSignedFloat(16, 4, wit_data.yaw, 1, 16);

}
