/*******************************************************************************
运动控制
*******************************************************************************/


#ifndef __MOTION_ANALYSIS_H
#define __MOTION_ANALYSIS_H


// a:0.09m  ;b:0.10m 
// A_B_SUM = (a + b)，即底盘几何中心到轮子轴心的纵向距离(a)与横向距离(b)之和
// 单位：米 (m)，用于将底盘自转角速度(rad/s)转换为轮子的切向线速度(m/s)
#define A_B_SUM         (0.19)

// 逆运动学解算
void Motion_Inverse_Analysis(float vx, float vy, float wz);

// M车物理系数
#define ENCODER_RESOLUTION      4096.0f    // 编码器本体一圈的计数值
#define GEAR_TEETH_ENCODER      30.0f      // 编码器侧齿轮齿数
#define GEAR_TEETH_WHEEL        70.0f      // 轮子侧齿轮齿数
#define WHEEL_DIAMETER_M        0.061f     // 轮子直径 (单位: m)
#define SAMPLE_PERIOD_S         0.01f      // 采样周期 10ms (单位: s)

// 转换系数
// 系数 = (PI * 轮径) / (编码器分辨率 * (轮侧齿数/编码器侧齿数) * 采样周期)
#define WHEEL_SPEED_FACTOR      (3.1415926f * WHEEL_DIAMETER_M) / \
                                (ENCODER_RESOLUTION * (GEAR_TEETH_WHEEL / GEAR_TEETH_ENCODER) * SAMPLE_PERIOD_S)
// 线速度(m/s) = 脉冲增量(counts) × WHEEL_SPEED_FACTOR
// 注：当前理论系数值约为 0.002005

float Wheel_to_Speed(int delta_counts);

#endif
