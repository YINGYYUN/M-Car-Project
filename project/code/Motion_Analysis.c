/*******************************************************************************
运动控制
*******************************************************************************/


#include "zf_common_headfile.h"
#include "Motion_Analysis.h"


//----------------------------------------------------------
// 函数简介     运动学逆解算
// 使用示例     Motion_Inverse_Analysis(vx, vy, wz);
// 参数说明     vx 范围：底盘X轴线速度 (单位: m/s)
// 参数说明     vy 范围：底盘Y轴线速度 (单位: m/s)
// 参数说明     wz 范围：底盘Z轴自转角速度 (单位: rad/s，逆时针为正)
// 返回参数     void
// 备注信息     计算结果直接写入对应的变量
//----------------------------------------------------------
void Motion_Inverse_Analysis(float vx, float vy, float wz)
{
    Motor_FL_PID.Target = vy + vx + wz * A_B_SUM; 
    Motor_FR_PID.Target = vy - vx - wz * A_B_SUM;
    Motor_RL_PID.Target = vy - vx + wz * A_B_SUM;
    Motor_RR_PID.Target = vy + vx - wz * A_B_SUM;
}

//----------------------------------------------------------
// 函数简介     将编码器脉冲增量转换为轮子线速度
// 备注信息     实际速度(m/s) = Wheel_to_Speed(单位时间获得脉冲量)
// 备注信息     当前单位时间为10ms，相关配置在.h文件内
//----------------------------------------------------------
float Wheel_to_Speed(int delta_counts)
{
    return (float)delta_counts * WHEEL_SPEED_FACTOR;
}
