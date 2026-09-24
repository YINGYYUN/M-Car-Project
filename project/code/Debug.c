/*******************************************************************************
调试
*******************************************************************************/


#include "zf_common_headfile.h"

/**********************************************************/
/*[S] 界面样式 [S]----------------------------------------*/
/**********************************************************/

// [二级界面]Debug模式界面
void Debug_Page_Menu_UI(void)
{
    ips200_show_string(8  ,0  , "[Debug]");
    ips200_show_string(0  ,16 , "==============================");
    ips200_show_string(10 ,32 , "MOTOR");
    ips200_show_string(10 ,48 , "MOTOR-PID");
}

// [三级界面]电机调试界面
void Debug_MOTOR_UI(void)
{
    ips200_show_string(8  ,0  , "[DEBUG]-MOTOR");
    ips200_show_string(0  ,16 , "==============================");
    ips200_show_string(10 ,32 , "PWM FL:###");
    ips200_show_string(10 ,48 , "PWM FR:###");
    ips200_show_string(10 ,64 , "PWM RL:###");
    ips200_show_string(10 ,80 , "PWM RR:###");
    // 空行
    ips200_show_string(10 ,112, "ENC FL:###    SUM:###");
    ips200_show_string(10 ,128, "ENC FR:###    SUM:###");
    ips200_show_string(10 ,144, "ENC RL:###    SUM:###");
    ips200_show_string(10 ,160, "ENC RR:###    SUM:###");
}

// [三级界面]Motor_PID调试界面   
// 速度环
void Debug_Motor_PID_UI(void)
{
    ips200_show_string(8  ,0  , "[DEBUG]-MOTOR-PID");
    ips200_show_string(0  ,16 , "==============================");
    ips200_show_string(10 ,32 , "TAR FL:###    ENC:###");
    ips200_show_string(10 ,48 , "TAR FR:###    ENC:###");
    ips200_show_string(10 ,64 , "TAR RL:###    ENC:###");
    ips200_show_string(10 ,80 , "TAR RR:###    ENC:###");
    // 空行
    ips200_show_string(10 ,112, "PWM FL:###    SUM:###");
    ips200_show_string(10 ,128, "PWM FR:###    SUM:###");
    ips200_show_string(10 ,144, "PWM RL:###    SUM:###");
    ips200_show_string(10 ,160, "PWM RR:###    SUM:###");
}
/**********************************************************/
/*----------------------------------------[E] 界面样式 [E]*/
/**********************************************************/


/**********************************************************/
/*[S] 菜单逻辑 [S]----------------------------------------*/
/**********************************************************/

// 相关函数提前声明
int Debug_Motor         (void);
int Debug_Motor_PID     (void);

// [二级界面]Debug模式界面
int Debug_Page_Menu(void)
{
    // Debug模式选项 标志位
    uint8_t Debug_Page_flag = 1;

    Debug_Page_Menu_UI();
    ips200_show_string(0  ,32 , ">");

    // 重置计时参考值
    Time_Count1 = 0;
    Time_Count2 = 0;

    while(1)
    {
        // 存储确认键被按下时Debug_Page_flag的值的临时变量，默认为无效值0
		uint8_t Debug_Page_flag_temp = 0;
		// 上/下按键是否被按下过
		uint8_t key_pressed = 0;


        /* 按键处理*/
        if (KEY_SHORT_PRESS == key_get_state(KEY_UP))
        {
            key_clear_state(KEY_UP);
            key_pressed = 1;
            Debug_Page_flag --;
            if (Debug_Page_flag < 1)Debug_Page_flag = 2;
        }
        else if (KEY_SHORT_PRESS == key_get_state(KEY_DOWN))
        {
            key_clear_state(KEY_DOWN); 
            key_pressed = 1;
            Debug_Page_flag ++;
            if (Debug_Page_flag > 2)Debug_Page_flag = 1;
        }
        else if (KEY_SHORT_PRESS == key_get_state(KEY_CONFIRM))
        {
            key_clear_state(KEY_CONFIRM);
            Debug_Page_flag_temp = Debug_Page_flag;
        }
        else if (KEY_SHORT_PRESS == key_get_state(KEY_BACK))    
        {
			key_clear_state(KEY_BACK);
            // 返回上一级界面
            return 0;   
        }


        /* 模式跳转*/
        if (Debug_Page_flag_temp == 1)
        {
            ips200_clear();
            Debug_Motor();
            
            // 从子界面返回后
            ips200_clear();
            Debug_Page_Menu_UI();
            ips200_show_string(0  ,32 , ">");
        }
        if (Debug_Page_flag_temp == 2)
        {
            ips200_clear();
            Debug_Motor_PID();

            // 从子界面返回后
            ips200_clear();
            Debug_Page_Menu_UI();
            ips200_show_string(0  ,48 , ">");
        }

        
        /* 显示更新*/
        if (key_pressed)
        {
			// 清理光标
			ips200_show_string(0  ,32 , " ");
			ips200_show_string(0  ,48 , " ");
			ips200_show_string(0  ,64 , " ");
			ips200_show_string(0  ,80 , " ");
			ips200_show_string(0  ,96 , " ");
			// 显示光标
			ips200_show_string(0  ,16 + 16*Debug_Page_flag , ">");
        }
    }
}
/**********************************************************/
/*----------------------------------------[E] 菜单逻辑 [E]*/
/**********************************************************/


/**********************************************************/
/*[S] 调试逻辑 [S]----------------------------------------*/
/**********************************************************/

//	#   #   ###   #####   ###   ####   
//  ## ##  #   #    #    #   #  #   #  
//  # # #  #   #    #    #   #  ####   
//  #   #  #   #    #    #   #  #  #   
//  #   #   ###     #     ###   #   #  
//
// [三级界面]电机调试
int Debug_Motor (void)
{
    // 电机驱动相关,为方便调用元素数量为5
    int16_t pwm[5] = {0};
    Motor_ALL_Zero();

    // 电机调试界面光标 标志位
    // 完整的命名为Debug_Motor_flag，此处进行简化
    uint8_t Debug_M_f = 1;

    Debug_MOTOR_UI();
    ips200_show_string(0 ,32 , ">");
    ips200_printf(66 ,32 , "%d   ", pwm[1]);
    ips200_printf(66 ,48 , "%d   ", pwm[2]);
    ips200_printf(66 ,64 , "%d   ", pwm[3]);
    ips200_printf(66 ,80 , "%d   ", pwm[4]);

    // 重置计时参考值
    Time_Count1 = 0;
    Time_Count2 = 0;

    while(1)
    {
        // 存储确认键被按下时Debug_M_f的值的临时变量，默认为无效值0
        uint8_t Debug_M_f_temp = 0;
        // 上/下按键是否被按下过
        uint8_t key_pressed = 0;

        /* 按键处理 */
        if (KEY_SHORT_PRESS == key_get_state(KEY_UP))
        {
            key_clear_state(KEY_UP);
            key_pressed = 1;
            Debug_M_f --;
            if (Debug_M_f < 1){Debug_M_f = 4;}
        }
        else if (KEY_SHORT_PRESS == key_get_state(KEY_DOWN))
        {
            key_clear_state(KEY_DOWN);
            key_pressed = 1;
            Debug_M_f ++;
            if (Debug_M_f > 4){Debug_M_f = 1;}      
        }
        else if (KEY_SHORT_PRESS == key_get_state(KEY_CONFIRM))
        {
            key_clear_state(KEY_CONFIRM);
            Debug_M_f_temp = Debug_M_f;
        }
        else if (KEY_SHORT_PRESS == key_get_state(KEY_BACK))
        {
            key_clear_state(KEY_BACK);

            Motor_ALL_Zero();
            // 返回上一级界面
            return 0;
        }
            
            
        /* 参数设置 */
        if (1 <=Debug_M_f_temp && Debug_M_f_temp <= 4)
        {
            ips200_show_string(0 ,16 + 16*Debug_M_f_temp , "=");
            
            // 电机手动设置
            while(1)
            {
                /* 按键解析 */
                if (KEY_SHORT_PRESS == key_get_state(KEY_UP))
                {
                    key_clear_state(KEY_UP);
                    pwm[Debug_M_f] += 100;
                    if (pwm[Debug_M_f] > 10000)pwm[Debug_M_f] = 10000;
                    Motor_Set(Debug_M_f, pwm[Debug_M_f]);
                    ips200_printf(66 ,16 + 16*Debug_M_f, "%d   ", pwm[Debug_M_f]);
                }
                else if (KEY_SHORT_PRESS == key_get_state(KEY_DOWN))
                {
                    key_clear_state(KEY_DOWN);
                    pwm[Debug_M_f] -= 100;
                    if (pwm[Debug_M_f] < -10000)pwm[Debug_M_f] = -10000;
                    Motor_Set(Debug_M_f, pwm[Debug_M_f]);
                    ips200_printf(66 ,16 + 16*Debug_M_f, "%d   ", pwm[Debug_M_f]);
                }
                else if (KEY_SHORT_PRESS == key_get_state(KEY_CONFIRM) || 
                         KEY_SHORT_PRESS == key_get_state(KEY_BACK))
                {
                    key_clear_state(KEY_CONFIRM);
                    key_clear_state(KEY_BACK);
					ips200_show_string(0 ,16 + 16*Debug_M_f_temp , ">");
                    
                    break;  // 退出修改模式
                }


                /* 显示更新 */
                if (Time_Count1 >= 10)// 10ms * 10 显示周期
                {
                    Time_Count1 = 0;
                    
                    ips200_printf(66 ,112, "%d   ", ENC_FL_CNT);
                    ips200_printf(66 ,128, "%d   ", ENC_FR_CNT);
                    ips200_printf(66 ,144, "%d   ", ENC_RL_CNT);
                    ips200_printf(66 ,160, "%d   ", ENC_RR_CNT);
                    ips200_printf(154,112, "%d     ", ENC_FL_SUM);
                    ips200_printf(154,128, "%d     ", ENC_FR_SUM);
                    ips200_printf(154,144, "%d     ", ENC_RL_SUM);
                    ips200_printf(154,160, "%d     ", ENC_RR_SUM);
                }
            }
        }
        

        /* 显示更新 */
        if (Time_Count1 >= 10)// 10ms * 10 显示周期
        {
            Time_Count1 = 0;
            
            ips200_printf(66 ,112, "%d   ", ENC_FL_CNT);
            ips200_printf(66 ,128, "%d   ", ENC_FR_CNT);
            ips200_printf(66 ,144, "%d   ", ENC_RL_CNT);
            ips200_printf(66 ,160, "%d   ", ENC_RR_CNT);
            ips200_printf(154,112, "%d     ", ENC_FL_SUM);
            ips200_printf(154,128, "%d     ", ENC_FR_SUM);
            ips200_printf(154,144, "%d     ", ENC_RL_SUM);
            ips200_printf(154,160, "%d     ", ENC_RR_SUM);
        }


        /* 光标更新 */
        if (key_pressed)
        {
            // 清理光标
            ips200_show_string(0 ,32 , " ");
            ips200_show_string(0 ,48 , " ");
            ips200_show_string(0 ,64 , " ");
            ips200_show_string(0 ,80 , " ");
            // 显示光标
            ips200_show_string(0 ,16 + 16*Debug_M_f  , ">");
        }
    }
}

//	#   #   ###   #####   ###   ####          ####   #####  ####   
//  ## ##  #   #    #    #   #  #   #         #   #    #    #   #  
//  # # #  #   #    #    #   #  ####    ###   ####     #    #   #  
//  #   #  #   #    #    #   #  #  #          #        #    #   #  
//  #   #   ###     #     ###   #   #         #      #####  ####   
//
// [三级界面]电机调试
int Debug_Motor_PID (void)
{
    // PID期望值相关,为方便调用元素数量为5
    int16_t enc_tar[5] = {0};
    // 重置PID中间量
    PID_ALL_Init();
    // 电机速度重置
    Motor_ALL_Zero();

    // 参考计时值重置
    Time_Count1 = 0;
    Time_Count2 = 0;

    Speed_PID_Crtl_Enable = 1;

	Debug_Motor_PID_UI();
	ips200_show_string(0 ,32 , ">");
    ips200_printf(66 ,32 , "%d  ", enc_tar[1]);
    ips200_printf(66 ,48 , "%d  ", enc_tar[2]);
    ips200_printf(66 ,64 , "%d  ", enc_tar[3]);
    ips200_printf(66 ,80 , "%d  ", enc_tar[4]);
    
    // 电机调试界面光标 标志位
    // 正常的命名为Debug_Motor_PID_flag，此处进行简化
    uint8_t Debug_M_P_f = 1;

    while(1)
    {

        // 存储确认键被按下时Debug_M_f的值的临时变量，默认为无效值0
        uint8_t Debug_M_P_f_temp = 0;
        // 上/下按键是否被按下过
        uint8_t key_pressed = 0;

        /* 按键处理 */
        if (KEY_SHORT_PRESS == key_get_state(KEY_UP))
        {
            key_clear_state(KEY_UP);
            key_pressed = 1;
            Debug_M_P_f --;
            if (Debug_M_P_f < 1){Debug_M_P_f = 4;}
        }
        else if (KEY_SHORT_PRESS == key_get_state(KEY_DOWN))
        {
            key_clear_state(KEY_DOWN);
            key_pressed = 1;
            Debug_M_P_f ++;
            if (Debug_M_P_f > 4){Debug_M_P_f = 1;}      
        }
        else if (KEY_SHORT_PRESS == key_get_state(KEY_CONFIRM))
        {
            key_clear_state(KEY_CONFIRM);
            Debug_M_P_f_temp = Debug_M_P_f;
        }
        else if (KEY_SHORT_PRESS == key_get_state(KEY_BACK))
        {
            key_clear_state(KEY_BACK);

            Speed_PID_Crtl_Enable = 0;
            // 重置PID中间量
            PID_ALL_Init();
            // 电机速度重置
            Motor_ALL_Zero();
            // 返回上一级界面
            return 0;
        }

        
        /* 参数设置 */
        if (1 <=Debug_M_P_f_temp && Debug_M_P_f_temp <= 4)
        {
            ips200_show_string(0 ,16 + 16*Debug_M_P_f_temp , "=");
            
            // 电机手动设置
            while(1)
            {
                /* 按键解析 */
                if (KEY_SHORT_PRESS == key_get_state(KEY_UP))
                {
                    key_clear_state(KEY_UP);
                    enc_tar[Debug_M_P_f] += 20;
                    if (enc_tar[Debug_M_P_f] > 800)enc_tar[Debug_M_P_f] = 800;
                    Motor_FL_PID.Target = enc_tar[1];
                    Motor_FR_PID.Target = enc_tar[2];
                    Motor_RL_PID.Target = enc_tar[3];
                    Motor_RR_PID.Target = enc_tar[4];
                    ips200_printf(66 ,16 + 16*Debug_M_P_f, "%d  ", enc_tar[Debug_M_P_f]);
                }
                else if (KEY_SHORT_PRESS == key_get_state(KEY_DOWN))
                {
                    key_clear_state(KEY_DOWN);
                    enc_tar[Debug_M_P_f] -= 20;
                    if (enc_tar[Debug_M_P_f] < -800)enc_tar[Debug_M_P_f] = -800;
                    Motor_FL_PID.Target = enc_tar[1];
                    Motor_FR_PID.Target = enc_tar[2];
                    Motor_RL_PID.Target = enc_tar[3];
                    Motor_RR_PID.Target = enc_tar[4];
                    ips200_printf(66 ,16 + 16*Debug_M_P_f, "%d  ", enc_tar[Debug_M_P_f]);
                }
                else if (KEY_SHORT_PRESS == key_get_state(KEY_CONFIRM) || 
                        KEY_SHORT_PRESS == key_get_state(KEY_BACK))
                {
                    key_clear_state(KEY_CONFIRM);
                    key_clear_state(KEY_BACK);
                    ips200_show_string(0 ,16 + 16*Debug_M_P_f_temp , ">");
                    
                    break;  // 退出修改模式
                }


                /* 数据显示 */
                if (Time_Count1 >= 8)// 10ms * 8 周期
                {
                    Time_Count1 = 0;

                    ips200_printf(154,32, "%d    ", (int16_t)Motor_FL_PID.Actual);
                    ips200_printf(154,48, "%d    ", (int16_t)Motor_FR_PID.Actual);
                    ips200_printf(154,64, "%d    ", (int16_t)Motor_RL_PID.Actual);
                    ips200_printf(154,80, "%d    ", (int16_t)Motor_RR_PID.Actual);
                    // ips200_printf(66 ,112, "%d   ", (int16_t)Motor_FL_PID.Out);
                    // ips200_printf(66 ,128, "%d   ", (int16_t)Motor_FR_PID.Out);
                    // ips200_printf(66 ,144, "%d   ", (int16_t)Motor_RL_PID.Out);
                    // ips200_printf(66 ,160, "%d   ", (int16_t)Motor_RR_PID.Out);
                    // ips200_printf(154,112, "%d     ", ENC_FL_SUM);
                    // ips200_printf(154,128, "%d     ", ENC_FR_SUM);
                    // ips200_printf(154,144, "%d     ", ENC_RL_SUM);
                    // ips200_printf(154,160, "%d     ", ENC_RR_SUM);
					// printf("%d,%d,%d\n", (int16_t)Motor_1_PID.Actual, (int16_t)Motor_1_PID.Target, (int16_t)Motor_1_PID.Out);
                }
            }
        }
        

        /* 数据显示 */
        if (Time_Count1 >= 8)// 10 * 8 ms周期
        {
            Time_Count1 = 0;

            ips200_printf(154,32, "%d    ", (int16_t)Motor_FL_PID.Actual);
            ips200_printf(154,48, "%d    ", (int16_t)Motor_FR_PID.Actual);
            ips200_printf(154,64, "%d    ", (int16_t)Motor_RL_PID.Actual);
            ips200_printf(154,80, "%d    ", (int16_t)Motor_RR_PID.Actual);
            // ips200_printf(66 ,112, "%d   ", (int16_t)Motor_FL_PID.Out);
            // ips200_printf(66 ,128, "%d   ", (int16_t)Motor_FR_PID.Out);
            // ips200_printf(66 ,144, "%d   ", (int16_t)Motor_RL_PID.Out);
            // ips200_printf(66 ,160, "%d   ", (int16_t)Motor_RR_PID.Out);
            // ips200_printf(154,112, "%d     ", ENC_FL_SUM);
            // ips200_printf(154,128, "%d     ", ENC_FR_SUM);
            // ips200_printf(154,144, "%d     ", ENC_RL_SUM);
            // ips200_printf(154,160, "%d     ", ENC_RR_SUM);
			// printf("%d,%d,%d\n", (int16_t)Motor_1_PID.Actual, (int16_t)Motor_1_PID.Target, (int16_t)Motor_1_PID.Out);
        }
        
        
        /* 光标更新 */
        if (key_pressed)
        {
            // 清理光标
            ips200_show_string(0 ,32 , " ");
            ips200_show_string(0 ,48 , " ");
            ips200_show_string(0 ,64 , " ");
            ips200_show_string(0 ,80 , " ");
            // 显示光标
            ips200_show_string(0 ,16 + 16*Debug_M_P_f  , ">");
        }
    }
}
/**********************************************************/
/*----------------------------------------[E] 调试逻辑 [E]*/
/**********************************************************/