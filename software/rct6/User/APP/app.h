#ifndef __APP_H
#define __APP_H

#include "main.h"
#include "tim.h"
#include "led.h"
#include "key.h"
#include "oled.h"
#include "uart_it_config.h"
#include "uart_1.h"
#include "uart_2.h"
#include "pwm.h"
#include "yuntai.h"
//typedef struct {
//			float x;
//			float y;
//		} Point;

//void calculate_equal_points(float x_start, float y_start, float x_end, float y_end, Point points[6]);
int openmv(void);
void App_Init(void);
int App_Task(void);
int abs_app(int num);
#endif // !__APP_H
