/*******************************************************************************
PAW3395 / PAW3950 光位移传感器驱动（软件 SPI 版）
基于逐飞 PMW3901 驱动框架 + gitee stm32_paw3395_mouse 初始化序列改写
芯片为 PixArt PAW3395DM-T6QU（Product_ID = 0x51）
*******************************************************************************/

#ifndef __PAW3395_H__
#define __PAW3395_H__

#include "zf_common_headfile.h"

//==================================================== 软件 SPI 引脚 ====================================================
#define PAW3395_SCLK_PIN        (P14_4)                 // SCLK
#define PAW3395_MOSI_PIN        (P12_5)                 // MOSI
#define PAW3395_MISO_PIN        (P12_4)                 // MISO
#define PAW3395_NCS_PIN         (P14_5)                 // NCS（软件片选，低有效）
#define PAW3395_NCS(x)          ((x) ? (gpio_high(PAW3395_NCS_PIN)) : (gpio_low(PAW3395_NCS_PIN)))
//==================================================== 软件 SPI 引脚 ====================================================


//================================================ 定义 PAW3395 内部地址 ================================================
#define PAW3395_PRODUCT_ID      (0x00)                  // 产品 ID（0x51）
#define PAW3395_REVISION_ID     (0x01)                  // 版本号
#define PAW3395_MOTION          (0x02)                  // 运动状态
#define PAW3395_DELTA_X_L       (0x03)                  // X 位移低字节
#define PAW3395_DELTA_X_H       (0x04)                  // X 位移高字节
#define PAW3395_DELTA_Y_L       (0x05)                  // Y 位移低字节
#define PAW3395_DELTA_Y_H       (0x06)                  // Y 位移高字节
#define PAW3395_MOTION_BURST    (0x16)                  // 运动突发读取（12 字节）
#define PAW3395_POWER_UP_RESET  (0x3A)                  // 上电复位（写 0x5A）
#define PAW3395_BANK_SWITCH     (0x7F)                  // Bank 切换

// CPI（分辨率）配置寄存器
#define PAW3395_SET_RESOLUTION  (0x47)                  // 分辨率更新触发
#define PAW3395_RESOLUTION_X_L  (0x48)                  // X 分辨率低字节
#define PAW3395_RESOLUTION_X_H  (0x49)                  // X 分辨率高字节
#define PAW3395_RESOLUTION_Y_L  (0x4A)                  // Y 分辨率低字节
#define PAW3395_RESOLUTION_Y_H  (0x4B)                  // Y 分辨率高字节
#define PAW3395_MOTION_CTRL     (0x5C)                  // 运动控制
//================================================ 定义 PAW3395 内部地址 ================================================


//================================================ 声明 PAW3395 全局变量 ================================================
extern int16 paw3395_delta_x,   paw3395_delta_y;       // 每周期位移增量
extern int32 paw3395_delta_x_i, paw3395_delta_y_i;     // 累计位移
//================================================ 声明 PAW3395 全局变量 ================================================


//================================================ 声明 PAW3395 基础函数 ================================================
uint8 paw3395_init              (void);                 // 初始化（含上电序列 + 寄存器配置）
void  paw3395_get_motion        (void);                 // 读取位移（结果在 delta_x/y）
void  paw3395_set_cpi           (uint16 cpi);           // 设置分辨率（cpi，50 的整数倍）
void  paw3395_clear_count       (void);                 // 清零累计位移
//================================================ 声明 PAW3395 基础函数 ================================================

#endif
