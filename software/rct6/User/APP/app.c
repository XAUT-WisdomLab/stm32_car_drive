
#include "app.h"
#include <math.h>
// 题目选择标志位
extern uint8_t Problem_Flag = 0;
// [激光]实时位置
int x = 0;
int y = 0;
//openmv A4
int x_a1 = 0; //左上角
int y_a1 = 0;
int x_a2 = 0;	//右上角
int y_a2 = 0;
int x_a3 = 0; //右下角
int y_a3 = 0;
int x_a4 = 0;	//左下角
int y_a4 = 0;
int x_err =0;
int y_err =0;

// [openmv]实时位置原始值
char openmv_msg[64];
// 消息接收标志位
uint8_t openmv_Flag;
//--------------------------舵机参考系---------------------
// 中心位置处参数
extern uint16_t Centre_A = 1460;
extern uint16_t Centre_B = 1417;
// 舵机A（最值）   垂直
extern uint16_t Servo_MAX_A = 2000;
extern uint16_t Servo_MIN_A = 1000;
// 舵机B （最值）  水平
extern uint16_t Servo_MIN_B = 1000;
extern uint16_t Servo_MAX_B = 2000;
// 舵机（左下位置）
extern uint16_t left_PWM = 1635;
extern uint16_t down_PWM =1480;
// 舵机（右上位置）
extern uint16_t right_PWM = 1315;
extern uint16_t up_PWM =1740;
// 实时位置
extern uint16_t pwm_A;//竖直
extern uint16_t pwm_B;//水平
//--------------------------K210参考系---------------------
//左上角坐标
extern uint16_t K210_left = 0;
extern uint16_t K210_up= 0;
//右下角坐标
extern uint16_t K210_right = 0;
extern uint16_t K210_down= 0;
//宽度长度
extern uint16_t K210_Width = 0;
extern uint16_t K210_Height= 0;

extern uint16_t K210_data[8];

//左下角
int x1 = 0;	
int y1 = 0;

//右上角
int x2 = 0;	
int y2 = 0;

//左上角
int x3 = 0;	
int y3 = 0;

//右下角
int x4 = 0;	
int y4 = 0;

//A4纸右上角
int x5 = 0;
int y5 = 0;

//原点
int x0 = 0;	
int y0 = 0;

extern int flag_key4;
extern int flag_key3;
// OLED屏幕缓冲区
uint8_t str_buff1[64] = {0};
uint8_t str_buff2[64] = {0};
uint8_t str_buff3[64] = {0};
uint8_t str_buff4[64] = {0};
uint8_t str_buff5[64] = {0};
uint8_t str_buff6[64] = {0};

void Problem1(void);
void Problem2(void);
void Problem3(void);
void Problem4(void);
void Problem5(void);
void Problem6(void);

void App_Init(void)
{
 
    UART_IT_Init(); // 总串口接收初始化
    HAL_TIM_Base_Start_IT(&htim2); // 启动定时器2
		OLED_Init();		// OLED初始化
		OLED_Clear();
		OLED_ShowString(4, 0, "START", 16);
	  PWM_Init();
    Yuntai_Control();              // 云台初始化
		DEBUG_printf("APP", "系统初始化完成~");
    pwm_A =1500;
    pwm_B =1475;
    Yuntaiz_A_Move(pwm_A,0); //竖直
    Yuntaiz_B_Move(pwm_B,0); //水平
		HAL_Delay(2000);
		pwm_A  = down_PWM;
		pwm_B  = left_PWM;
		Yuntaiz_A_Move(pwm_A,0); //竖直
    Yuntaiz_B_Move(pwm_B,0); //水平
	}


int App_Task(void)
{
	sprintf((char *)str_buff3, "problem_flag:%02d", Problem_Flag);
	// 刷新屏幕
	sprintf((char *)str_buff4, "Px:%04d Py:%04d", pwm_B,pwm_A );
	OLED_ShowString(0, 4, str_buff3, 8);
	OLED_ShowString(0, 5, str_buff4, 8);
	
	  // 参数限幅
    if (pwm_A > Servo_MAX_A)
        pwm_A = Servo_MAX_A;
    if (pwm_A < Servo_MIN_A)
        pwm_A = Servo_MIN_A;
    if (pwm_B > Servo_MAX_B)
        pwm_B = Servo_MAX_B;
    if (pwm_B < Servo_MIN_B)
        pwm_B = Servo_MIN_B;


//	Problem3();	

	static uint8_t last_Flag = 0;
		
  // 处理问题
	switch (Problem_Flag)
	{
	case 0:
		last_Flag = Problem_Flag;
	case 1:
		if (last_Flag != Problem_Flag)
		{
			Problem1();
			last_Flag = Problem_Flag;
		}
		break;
	case 2:
		if (last_Flag != Problem_Flag)
		{
			Problem2();
			last_Flag = Problem_Flag;
		}
		break;
	case 3:
		if (last_Flag != Problem_Flag)
		{
			while(1)
			{
			Problem3();
			}
			last_Flag = Problem_Flag;
		}
		break;
	case 4:
		if (last_Flag != Problem_Flag)
		{
//			Problem4();
			last_Flag = Problem_Flag;
		}
		break;
	case 5: 
		if (last_Flag != Problem_Flag)
		{
//			Problem5();
			last_Flag = Problem_Flag;
		}
		break;
	case 6: // 
		if (last_Flag != Problem_Flag)
		{
//			Problem6();
			last_Flag = Problem_Flag;
		}
		break;
	default:
		break;
	}

}

// 题目1
void Problem1(void)
{
	DEBUG_printf("APP", "Problem1 runing--");
//	HAL_Delay(2);
//	// 逻辑控制
//	//---------now----------
	x3 = x1;
	y3 = y2;
	x4 = x2;
	y4 = y1;
	while(1)
	{
		flag_key3 = 0;
		Yuntaiz_AB_Move_2(y1, x1, 0);//移动到左下角
		HAL_Delay(200);
		Yuntaiz_AB_Move_2(y3, x3, 0);//移动到左上角
		HAL_Delay(200);
		Yuntaiz_AB_Move_2(y2, x2, 0);//移动到右上角
		HAL_Delay(200);
		Yuntaiz_AB_Move_2(y4, x4, 0);//移动到右下角
		HAL_Delay(200);
		Yuntaiz_AB_Move_2(y1, x1, 0);//移动到左下角
		HAL_Delay(200);
		if(flag_key3==1)
		{
			Yuntaiz_AB_Move(y0, x0, 1);
			HAL_Delay(2);
			return ;
		}
	}
}

// 题目2
void Problem2(void)
{
	//A4纸右上角  左下角(x1,y1) 左上角(x1,y5) 右上角(x5,y5) 右下角(x5,y1)

	
	x5 = x1 - (x1-x2)*0.6;
	y5 = y1 + (y2-y1)*0.42;
	while(1)
	{
		flag_key3 = 0;
		Yuntaiz_AB_Move_2(y1, x1, 0);//移动到左下角
		HAL_Delay(200);
		Yuntaiz_AB_Move_2(y5, x1, 0);//移动到左上角
		HAL_Delay(200);
		Yuntaiz_AB_Move_2(y5, x5, 0);//移动到右上角
		HAL_Delay(200);
		Yuntaiz_AB_Move_2(y1, x5, 0);//移动到右下角
		HAL_Delay(200);
		Yuntaiz_AB_Move_2(y1, x1, 0);//移动到左下角
		HAL_Delay(200);	
		if(flag_key3==1)
		{
			
			Yuntaiz_AB_Move(y0, x0, 1);
			openmv();
			HAL_Delay(2);
			return ;
		}		
	}
}


int problem3_flag = 0;
void Problem3(void)
{
	int dx;	
	int dy;
	static	int pwm_x_a1=0;
	static	int pwm_y_a1=0;
	static	int pwm_x_a2=0;
	static	int pwm_y_a2=0;
	static	int pwm_x_a3=0;
	static	int pwm_y_a3=0;
	static	int pwm_x_a4=0;
	static	int pwm_y_a4=0;
		
	dx = (x_a1 - x_a2)/5.0;
	dy = (y_a1 - y_a2)/5.0;
	openmv();
	switch(problem3_flag)
	{
		case 0:
			openmv();
			sprintf((char *)str_buff2, "start");
			x_err = (x - x_a1);
			y_err = (y - y_a1);
			Yuntai_PID();
			sprintf((char *)str_buff2, "1:%03d:%03d", x_a1, y_a1);
			if((x_err<2&&x_err>-2)&&(y_err<2&&y_err>-2))
			{
				x_err = 0,y_err = 0;
				pwm_x_a1 = pwm_B;
				pwm_y_a1 = pwm_A;
				DEBUG_printf("pwm", "pwm1:%d  %d",pwm_x_a1, pwm_y_a1);
				HAL_Delay(300);
				problem3_flag = 1;
			}

			break;
		
		case 1:
			openmv();
			sprintf((char *)str_buff2, "2:%03d:%03d", x_a2, y_a2);
			x_err = (x - (x_a2));
			y_err = (y - (y_a2));
			Yuntai_PID();
			if((x_err<3&&x_err>-3)&&(y_err<3&&y_err>-3))
			{
				x_err = 0,y_err = 0;
				pwm_x_a2 = pwm_B;
				pwm_y_a2 = pwm_A;
				DEBUG_printf("pwm", "pwm2:%d  %d",pwm_x_a2, pwm_y_a2);
				HAL_Delay(300);
				problem3_flag = 2;
				
			}
			break;		
		case 2:
			openmv();
			sprintf((char *)str_buff2, "3:%03d:%03d", x_a3, y_a3);
			x_err = (x - (x_a3));
			y_err = (y - (y_a3));
			Yuntai_PID();
			if((x_err<3&&x_err>-3)&&(y_err<3&&y_err>-3))
			{
				x_err = 0,y_err = 0;
				pwm_x_a3 = pwm_B;
				pwm_y_a3 = pwm_A;
				DEBUG_printf("pwm", "pwm123:%d %d %d %d %d %d",pwm_x_a1, pwm_y_a1,pwm_x_a2, pwm_y_a2,pwm_x_a3, pwm_y_a3);
				HAL_Delay(300);
				problem3_flag = 3;
			}
			break;	
		case 3:
			openmv();
			sprintf((char *)str_buff2, "4:%03d:%03d", x_a4, y_a4);
			x_err = (x - (x_a4));
			y_err = (y - (y_a4));
			Yuntai_PID();
			if((x_err<3&&x_err>-3)&&(y_err<3&&y_err>-3))
			{
				x_err = 0,y_err = 0;
				pwm_x_a4 = pwm_B;
				pwm_y_a4 = pwm_A;
				DEBUG_printf("pwm", "pwm4:%d  %d",pwm_x_a4, pwm_y_a4);
				HAL_Delay(300);
				problem3_flag = 4;
			}
			break;	
		case 4:
				Yuntaiz_AB_Move_2(pwm_y_a1,pwm_x_a1,3);
				HAL_Delay(300);
				Yuntaiz_AB_Move_2(pwm_y_a2-10,pwm_x_a2,3);
				HAL_Delay(300);
				Yuntaiz_AB_Move_2(pwm_y_a3,pwm_x_a3+5,3);
				HAL_Delay(300);
				Yuntaiz_AB_Move_2(pwm_y_a4,pwm_x_a4,3);
				HAL_Delay(300);
				if(flag_key3==1)
				{
					Yuntaiz_AB_Move(y0, x0, 1);
					HAL_Delay(2);
					Problem_Flag = 1;
					problem3_flag = 5;
					return ;
				}		
			
			break;		
		case 5:
			Problem_Flag = 1;
			DEBUG_printf("pwm", "Problem_Flag:%d",Problem_Flag);
			problem3_flag = 6;
			return;
			break;
		default:
			App_Task();
			return;
			break;

	}

}


int openmv(void)
{
	// 处理openmv数据
	if (1 == openmv_Flag)
	{
		// 提取数据
		sscanf(openmv_msg, "%d,%d,%d,%d,%d,%d,%d,%d,%d,%d", &x_a1,&y_a1,&x_a2,&y_a2,&x_a3,&y_a3,&x_a4,&y_a4,&x, &y);
		DEBUG_printf("APP", "openmv-> %d,%d,%d,%d,%d,%d,%d,%d,X:%d,Y:%d", x_a1, y_a1, x_a2, y_a2, x_a3, y_a3, x_a4, y_a4, x, y);
		HAL_Delay(10);

		// 清除缓存
		memset(openmv_msg, 0, sizeof(openmv_msg));

		// 刷新屏幕
		sprintf((char *)str_buff1, "x:%03d y:%03d", x, y);
//		sprintf((char *)str_buff2, "1:%03d:%03d", x_a1, y_a1);

		OLED_ShowString(0, 2, str_buff1, 8);
		OLED_ShowString(0, 3, str_buff2, 8);

		// 清除标志位
		openmv_Flag = 0;
		return 1;
	}
	else
	{
		DEBUG_printf("APP", "openmv close");
		return 0;
	}
}
int abs_app(int num) {
    return (num < 0) ? -num : num;
}
// 定时器中断回调函数(1ms一次)
uint8_t LED_Heartbeat = 0; // LED心跳
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
    if (htim == &htim2) // 判断中断是否来自于定时器2
    {
        // 心跳(10ms一次)
        LED_Heartbeat++;
        if (LED_Heartbeat == 200)
        {
            LED_Toggle();

        }
    }
}
