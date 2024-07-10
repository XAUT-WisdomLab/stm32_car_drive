/*
 * @Author: DuRuofu duruofu@qq.com
 * @Date: 2023-08-02 12-02-17
 * @LastEditors: DuRuofu
 * @LastEditTime: 2024-04-13 18-13-01
 * @FilePath: \STM32(simple)\User\KEY\key.c
 * @Description: 键盘扫描
 * Copyright (c) 2023 by duruofu@foxmail.com All Rights Reserved. 
 */

#include "key.h"
#include "led.h"
//#include "gui.h"
#include "yuntai.h"
//#include "Buzzer.h"
#include "app.h"
#define DEBOUNCE_DELAY 250 // 设置消抖延时为200毫秒

#define KEY_1_Pin KEY1_Pin 
#define KEY_2_Pin KEY2_Pin 


//GUI菜单标志位
//extern uint8_t GUI_Menu;

//题目标志位
extern uint8_t Problem_Flag;

//校准确认标志位
//extern uint8_t Calibration_Flag;

//急停标志位
//extern uint8_t Stop_Flag;

// 初始位置  //舵机中值
extern uint16_t pwm_A ;
extern uint16_t pwm_B ;
int change=5;
//左下角
extern int x1 ;	
extern int y1;

//右上角
extern int x2;	
extern int y2;
//右上角
extern int x3;	
extern int y3;

//右下角
extern int x4;
extern int y4;

//原点
extern int x0;	
extern int y0;

// 舵机（左下位置）
extern uint16_t left_PWM ;
extern uint16_t down_PWM ;
// 舵机（右上位置）
extern uint16_t right_PWM ;
extern uint16_t up_PWM ;
//原点
extern uint16_t center_x= 1475;
extern uint16_t center_y= 1615;
//按键flag
int flag_key4 = 0;

int flag_key3 = 0;
/**
 * @description: 按键初始化 (使用CubeMX自动生成的宏定义，就不用写这个函数了)
 * @return {*}
 */
void Key_Init(void)
{

}




void Key_1_Callback(void)
{
	if(flag_key4 < 3)
	{
		pwm_A = pwm_A+change;
		Yuntaiz_AB_Move(pwm_A, pwm_B, 1);
	}
	// 更换题目
	else if(Problem_Flag < 7)
	{
			Problem_Flag++;
	}
	else
	{
			Problem_Flag = 0;
	}
		
}




void Key_2_Callback(void)
{
	if(flag_key4 < 3)
	{
		pwm_B = pwm_B+change;
		Yuntaiz_AB_Move(pwm_A, pwm_B, 1);
	}
	else
		Problem_Flag = 2;
}

void Key_3_Callback(void)
{
	if(flag_key4 < 3)
	{
		change = -change;
	}
	else
		flag_key3 = 1; //退出循环
	HAL_GPIO_WritePin(LED3_GPIO_Port, LED3_Pin, GPIO_PIN_RESET);
	
}

void Key_4_Callback(void)
{
	
	switch (flag_key4)
	{
		case 0:
			x1 = pwm_B;
			y1 = pwm_A;
			Yuntaiz_AB_Move_2(up_PWM,right_PWM,0);
			pwm_A = up_PWM;
			pwm_B = right_PWM;
			flag_key4 = flag_key4+1;
			DEBUG_printf("KEY", "x1:%d y1:%d",x1,y1);
			break;
		case 1:
			x2 = pwm_B;
			y2 = pwm_A;
			

			Yuntaiz_AB_Move_2(y1, x1, 0);//移动到左下角
			HAL_Delay(200);
			Yuntaiz_AB_Move_2(y2, x1, 0);//移动到左上角
			HAL_Delay(200);
			Yuntaiz_AB_Move_2(y2, x2, 0);//移动到右上角
			HAL_Delay(200);
			Yuntaiz_AB_Move_2(y1, x2, 0);//移动到右下角
			HAL_Delay(200);
			Yuntaiz_AB_Move_2(y1, x1, 0);//移动到左下角
			HAL_Delay(200);
			pwm_A = center_y;
			pwm_B = center_x;
			Yuntaiz_AB_Move_2(center_y,center_x,0);
			flag_key4 = flag_key4+1;
			DEBUG_printf("KEY", "x1:%d y1:%d",x2,y2);
			break;
		case 2:
			x0 = pwm_B;
			y0 = pwm_A;
			Yuntaiz_AB_Move(1650, 1360, 1);
			flag_key4 = flag_key4+1;
			DEBUG_printf("KEY", "x2:%d y2:%d",x0,y0);
			break;

		case 3:
//			flag_key4 = flag_key4+1;
			Yuntaiz_AB_Move(y0, x0, 1);
			DEBUG_printf("start", "x0:%d y0:%d",x0,y0);
			
		default:
			break;		
	}
	HAL_GPIO_WritePin(LED3_GPIO_Port, LED3_Pin, GPIO_PIN_SET);
}
//
/**
 * @description: 按键检测，外部中断回调函数
 * @param {uint16_t} GPIO_Pin
 * @return {*}
 */
void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{

    /* Prevent unused argument(s) compilation warning */
    UNUSED(GPIO_Pin);
    /* NOTE: This function Should not be modified, when the callback is needed,
            the HAL_GPIO_EXTI_Callback could be implemented in the user file
    */
    // 按键按下
    if(GPIO_Pin == KEY_1_Pin)
    {
        //printf("按键1");
        Debounce(GPIO_Pin, Key_1_Callback);
       
    }
    else if(GPIO_Pin == KEY_2_Pin)
    {

        // 按键2按下的处理代码
        Debounce(GPIO_Pin, Key_2_Callback);
    }
		    else if(GPIO_Pin == KEY3_Pin)
    {

        // 按键3按下的处理代码
        Debounce(GPIO_Pin, Key_3_Callback);
    }
		    else if(GPIO_Pin == KEY4_Pin)
    {

        // 按键4按下的处理代码
        Debounce(GPIO_Pin, Key_4_Callback);
    }
}




// 通用的按键消抖函数
void Debounce(uint16_t GPIO_Pin, void (*callback)(void))
{
    static uint32_t lastTriggerTime = 0;
    uint32_t currentTime = HAL_GetTick(); // 获取当前时间戳

    if (currentTime - lastTriggerTime >= DEBOUNCE_DELAY)
    {
//        Buzzer_ShortBeep();
        callback(); // 调用传入的回调函数
        lastTriggerTime = currentTime; // 更新上一次触发的时间戳
    }
}

