/*******************************************************************************
参数 Flash 存储 — 实现（CYT4BB 适配）
*******************************************************************************/

#include "Param_Storage.h"

// 4 个电机的增量式 PID（定义于 PID.c）
extern PID_INC_t Motor_FL_PID;           // 左前电机
extern PID_INC_t Motor_FR_PID;           // 右前电机
extern PID_INC_t Motor_RL_PID;           // 左后电机
extern PID_INC_t Motor_RR_PID;           // 右后电机


// 默认参数值(首次使用或恢复出厂设置时使用)
static const float DEFAULT_PARAMS[PARAM_COUNT] = {
    // Motor_FL_PID
    4.0f, 0.6f, 1.0f,       // KP, KI, KD
    // Motor_FR_PID
    4.0f, 0.6f, 1.0f,       // KP, KI, KD
    // Motor_RL_PID
    4.0f, 0.6f, 1.0f,       // KP, KI, KD
    // Motor_RR_PID
    4.0f, 0.6f, 1.0f,       // KP, KI, KD
};

// 参数缓存区(菜单直接修改此数组, Flash 读写也通过此数组)
float param_cache[PARAM_COUNT];


/**********************************************************/
/*[S] 内部函数 [S]----------------------------------------*/
/**********************************************************/

static void load_default(void)
{
    for (uint8_t i = 0; i < PARAM_COUNT; i++)
        param_cache[i] = DEFAULT_PARAMS[i];
}

static void copy_cache_to_flash_buffer(void)
{
    for (uint8_t i = 0; i < PARAM_COUNT; i++)
        flash_union_buffer[i].float_type = param_cache[i];
}

static void copy_flash_buffer_to_cache(void)
{
    for (uint8_t i = 0; i < PARAM_COUNT; i++)
        param_cache[i] = flash_union_buffer[i].float_type;
}

// 校验参数是否合法(异常值无效)
static uint8_t param_cache_is_valid(void)
{
    uint8_t kp_nonzero = 0;

    for (uint8_t i = 0; i < PARAM_COUNT; i++)
    {
        // 范围检查
        if (param_cache[i] > 10000.0f || param_cache[i] < -10000.0f)
            return 0;

        // 任意一个 Kp 非零即认为有效
        // 只是采样检验有效性，后续参数不必要参与该判定
        if (i == MOTOR_FL_KP_IDX || i == MOTOR_FR_KP_IDX ||
            i == MOTOR_RL_KP_IDX || i == MOTOR_RR_KP_IDX)
        {
            if (param_cache[i] > 0.0001f || param_cache[i] < -0.0001f)
                kp_nonzero = 1;
        }
    }

    return kp_nonzero;
}
/**********************************************************/
/*----------------------------------------[E] 内部函数 [E]*/
/**********************************************************/


/**********************************************************/
/*[S] 外部函数 [S]----------------------------------------*/
/**********************************************************/

// 初始化参数系统(上电调用一次)
void Param_Init(void)
{
    flash_init();       // CYT4BB Flash 使用前必须先初始化

    if (flash_check(PARAM_FLASH_SECTION, PARAM_FLASH_PAGE))
    {
        // Flash 有数据 → 读取到缓冲区
        flash_read_page_to_buffer(PARAM_FLASH_SECTION, PARAM_FLASH_PAGE, PARAM_COUNT);
        copy_flash_buffer_to_cache();

        // 校验失败 → 回退默认值并修复 Flash
        if (!param_cache_is_valid())
        {
            load_default();
            copy_cache_to_flash_buffer();
            flash_write_page_from_buffer(PARAM_FLASH_SECTION, PARAM_FLASH_PAGE, PARAM_COUNT);
        }
    }
    else
    {
        // 首次使用 → 写入默认值
        load_default();
        copy_cache_to_flash_buffer();
        flash_write_page_from_buffer(PARAM_FLASH_SECTION, PARAM_FLASH_PAGE, PARAM_COUNT);
    }

    // 将参数推送到实际应用参数
    Flash_SyncTo_Param();
}

// 保存参数到 Flash(退出参数页面时调用)
void Param_Save(void)
{
    copy_cache_to_flash_buffer();
    flash_write_page_from_buffer(PARAM_FLASH_SECTION, PARAM_FLASH_PAGE, PARAM_COUNT);
}

// 擦除 Flash(下次启动恢复默认)
void Param_Erase(void)
{
    flash_erase_page(PARAM_FLASH_SECTION, PARAM_FLASH_PAGE);
}

// 将缓存区值同步到实际应用参数
void Flash_SyncTo_Param(void)
{
    Motor_FL_PID.Kp = MOTOR_FL_KP;
    Motor_FL_PID.Ki = MOTOR_FL_KI;
    Motor_FL_PID.Kd = MOTOR_FL_KD;

    Motor_FR_PID.Kp = MOTOR_FR_KP;
    Motor_FR_PID.Ki = MOTOR_FR_KI;
    Motor_FR_PID.Kd = MOTOR_FR_KD;

    Motor_RL_PID.Kp = MOTOR_RL_KP;
    Motor_RL_PID.Ki = MOTOR_RL_KI;
    Motor_RL_PID.Kd = MOTOR_RL_KD;

    Motor_RR_PID.Kp = MOTOR_RR_KP;
    Motor_RR_PID.Ki = MOTOR_RR_KI;
    Motor_RR_PID.Kd = MOTOR_RR_KD;
}
/**********************************************************/
/*----------------------------------------[E] 外部函数 [E]*/
/**********************************************************/
