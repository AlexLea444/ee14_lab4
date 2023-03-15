#ifndef __STM32L476G_DISCOVERY_SYSTICK_H
#define __STM32L476G_DISCOVERY_SYSTICK_H

#include "stm32l476xx.h"

void SysTick_Init(uint32_t ticks);
void SysTick_Handler(void);
void SysTick_Write_Time(char *str);
void SysTick_Print_Time(char *str, uint32_t time_to_format, int offset);
void delay (uint32_t T);

#endif /* __STM32L476G_DISCOVERY_SYSTICK_H */
