/*******************************************************************************
PAW_Analysis — 光流传感器数据处理模块（业务层）
驱动层为 paw3395.c/h（软件 SPI 读写），本模块负责读取 + 通过共享内存传给 CM7_0
*******************************************************************************/


#ifndef __PAW_ANALYSIS_H__
#define __PAW_ANALYSIS_H__


#include "zf_common_headfile.h"
#include "driver_paw3395.h"

//================================================ 光流共享内存（独立于 IMU 共享内存） ====================================
typedef struct {
    volatile int32  flow_x_sum;  // CM7_1 -> CM7_0：累计 X 位移
    volatile int32  flow_y_sum;  // CM7_1 -> CM7_0：累计 Y 位移
    volatile uint8  cmd_clear;   // CM7_0 -> CM7_1：1 = 清零累计值
} shared_flow_t;

#define SHARED_FLOW_ADDR    ((volatile shared_flow_t *)0x280B8020)   // IMU 在 0x280B8000，光流错开一个 cache line

// cache 同步（32 字节对齐，覆盖整个结构体）
#define SHARED_FLOW_WRITE_SYNC()    SCB_CleanDCache_by_Addr((uint32_t*)SHARED_FLOW_ADDR, 32)
#define SHARED_FLOW_READ_SYNC()     SCB_InvalidateDCache_by_Addr((uint32_t*)SHARED_FLOW_ADDR, 32)
//================================================ 光流共享内存 ========================================================


// 光流更新：读 paw3395 位移 + 写共享内存（供 CM7_1 周期调用）
void PAW_Analysis_Update(void);

#endif
