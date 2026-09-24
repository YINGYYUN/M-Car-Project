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
}

// [三级界面]电机调试界面
void Debug_MOTOR_UI(void)
{
    ips200_show_string(8  ,0  , "[DEBUG]-MOTOR");
    ips200_show_string(0  ,16 , "==============================");
    ips200_show_string(10 ,32 , "PWM 1:###");
    ips200_show_string(10 ,48 , "PWM 2:###");
    ips200_show_string(10 ,64 , "PWM 3:###");
    ips200_show_string(10 ,80 , "PWM 4:###");
    ips200_show_string(10 ,112, "ENC 1:###        SUM:###");
    ips200_show_string(10 ,128, "ENC 2:###        SUM:###");
    ips200_show_string(10 ,144, "ENC 3:###        SUM:###");
    ips200_show_string(10 ,160, "ENC 4:###        SUM:###");
}
/**********************************************************/
/*----------------------------------------[E] 界面样式 [E]*/
/**********************************************************/


/**********************************************************/
/*[S] 菜单逻辑 [S]----------------------------------------*/
/**********************************************************/

// 相关函数提前声明
int Debug_Motor         (void);

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
            if (Debug_Page_flag < 1)Debug_Page_flag = 1;
        }
        else if (KEY_SHORT_PRESS == key_get_state(KEY_DOWN))
        {
            key_clear_state(KEY_DOWN); 
            key_pressed = 1;
            Debug_Page_flag ++;
            if (Debug_Page_flag > 1)Debug_Page_flag = 1;
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
    ips200_printf(58 ,32 , "%d   ", pwm[1]);
    ips200_printf(58 ,48 , "%d   ", pwm[2]);
    ips200_printf(58 ,64 , "%d   ", pwm[1]);
    ips200_printf(58 ,80 , "%d   ", pwm[2]);

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
                    ips200_printf(58 ,16 + 16*Debug_M_f, "%d   ", pwm[Debug_M_f]);
                }
                else if (KEY_SHORT_PRESS == key_get_state(KEY_DOWN))
                {
                    key_clear_state(KEY_DOWN);
                    pwm[Debug_M_f] -= 100;
                    if (pwm[Debug_M_f] < -10000)pwm[Debug_M_f] = -10000;
                    Motor_Set(Debug_M_f, pwm[Debug_M_f]);
                    ips200_printf(58 ,16 + 16*Debug_M_f, "%d   ", pwm[Debug_M_f]);
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
                if (Time_Count1 >= 10)// 10ms * 10n周期
                {
                    Time_Count1 = 0;
                    
                    ips200_printf(56 ,112, "%d   ", ENC_FL_CNT);
                    ips200_printf(56 ,128, "%d   ", ENC_FR_CNT);
                    ips200_printf(56 ,144, "%d   ", ENC_RL_CNT);
                    ips200_printf(56 ,160, "%d   ", ENC_RR_CNT);
                }
            }
        }
        

        /* 显示更新 */
        if (Time_Count1 >= 10)// 10ms * 10n周期
        {
            Time_Count1 = 0;
            
            ips200_printf(56 ,112, "%d   ", ENC_FL_CNT);
            ips200_printf(56 ,128, "%d   ", ENC_FR_CNT);
            ips200_printf(56 ,144, "%d   ", ENC_RL_CNT);
            ips200_printf(56 ,160, "%d   ", ENC_RR_CNT);
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
/**********************************************************/
/*----------------------------------------[E] 调试逻辑 [E]*/
/**********************************************************/