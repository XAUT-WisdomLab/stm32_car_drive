#ifndef __KEY_H
#define __KEY_H

#include "main.h"
#include "key.h"
#include "led.h"
#include "app.h"
void Debounce(uint16_t GPIO_Pin, void (*callback)(void));
void Key_Init(void);

#endif // !__LED_H



