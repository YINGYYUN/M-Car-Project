/*******************************************************************************
参数 Flash 存储
将可调参数持久化到 Flash，上电自动加载并同步到实际应用参数。
CYT4BB 适配：存储位置改为 Work Flash 第 95 页（sector 参数恒为 0）。
宏定义映射 — 每个参数单独编号，不强制 Kp/Ki/Kd 整齐排列。
*******************************************************************************/


#ifndef __PARAM_STORAGE_H__
#define __PARAM_STORAGE_H__


#include "zf_common_headfile.h"

// Flash 存储位置（CYT4BB Work Flash：sector 恒为 0，页范围 0-95）
#define PARAM_FLASH_SECTION         (0)
#define PARAM_FLASH_PAGE            (95)

// 参数总数（4 个电机 × 3 个参数）
#define PARAM_COUNT                 (12)

// 参数缓冲区(Flash 读写的唯一载体)
extern float param_cache[PARAM_COUNT];

// 参数索引定义
// Motor_FL_PID (0-2)  左前电机 速度环
#define MOTOR_FL_KP_IDX             0
#define MOTOR_FL_KI_IDX             1
#define MOTOR_FL_KD_IDX             2

// Motor_FR_PID (3-5)  右前电机 速度环
#define MOTOR_FR_KP_IDX             3
#define MOTOR_FR_KI_IDX             4
#define MOTOR_FR_KD_IDX             5

// Motor_RL_PID (6-8)  左后电机 速度环
#define MOTOR_RL_KP_IDX             6
#define MOTOR_RL_KI_IDX             7
#define MOTOR_RL_KD_IDX             8

// Motor_RR_PID (9-11) 右后电机 速度环
#define MOTOR_RR_KP_IDX             9
#define MOTOR_RR_KI_IDX             10
#define MOTOR_RR_KD_IDX             11

// 便捷访问宏
#define MOTOR_FL_KP                 param_cache[MOTOR_FL_KP_IDX]
#define MOTOR_FL_KI                 param_cache[MOTOR_FL_KI_IDX]
#define MOTOR_FL_KD                 param_cache[MOTOR_FL_KD_IDX]

#define MOTOR_FR_KP                 param_cache[MOTOR_FR_KP_IDX]
#define MOTOR_FR_KI                 param_cache[MOTOR_FR_KI_IDX]
#define MOTOR_FR_KD                 param_cache[MOTOR_FR_KD_IDX]

#define MOTOR_RL_KP                 param_cache[MOTOR_RL_KP_IDX]
#define MOTOR_RL_KI                 param_cache[MOTOR_RL_KI_IDX]
#define MOTOR_RL_KD                 param_cache[MOTOR_RL_KD_IDX]

#define MOTOR_RR_KP                 param_cache[MOTOR_RR_KP_IDX]
#define MOTOR_RR_KI                 param_cache[MOTOR_RR_KI_IDX]
#define MOTOR_RR_KD                 param_cache[MOTOR_RR_KD_IDX]


void    Param_Init          (void);     // 初始化(加载或设默认值)
void    Param_Save          (void);     // 保存当前参数到 Flash
void    Param_Erase         (void);     // 擦除 Flash(下次启动恢复默认)
void    Flash_SyncTo_Param  (void);     // 将缓存区的值推送到实际参数


#endif
