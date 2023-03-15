#include "stm32l476xx.h"
#include <stdio.h>

volatile uint32_t TimeDelay;
volatile uint32_t MillisecondsElapsed = 0;
volatile uint32_t SecondsElapsed = 0;
volatile uint32_t MinutesElapsed = 0;

// ticks: number of ticks between two interrupts
void SysTick_Init(uint32_t ticks) {
	
	// Disable SysTick IRQ and SysTick counter
	SysTick->CTRL = 0;
	
	// Set reload register
	SysTick->LOAD = ticks - 1;
	
	// Set interrupt priority of SysTick
	// Make SysTick least urgent (i.e. highest priority number)
	// __NVIC_PRIO_BITS: number of bits for priority levels, defined in CMSIS
	//NVIC_SetPriority(SysTick_IRQn, (1<<__NVIC_PRIO_BITS) - 1);
	
	NVIC_SetPriority(SysTick_IRQn, 1);		// Set Priority to 1
	NVIC_EnableIRQ(SysTick_IRQn);					// Enable EXTI0_1 interrupt in NVIC

	
	// Reset the SysTick counter value
	SysTick->VAL = 0;
	
	// Select processor clock
	// 1 = processor clock; 0 = external clock
	SysTick->CTRL &= ~SysTick_CTRL_CLKSOURCE_Msk;
	//SysTick->CTRL = 0;
	// Enable SysTick exception request
	// 1 = counting down to zero asserts the SysTick exception request
	// 0 = counting down to zero does no assert the SysTick exception request
	SysTick->CTRL |= SysTick_CTRL_TICKINT_Msk;
	
	// Enable SysTick timer
	SysTick->CTRL |= SysTick_CTRL_ENABLE_Msk;
}

// SysTick interrupt service routine
void SysTick_Handler(void) {
	TimeDelay--;
	MillisecondsElapsed++;
}

void SysTick_Print_Time(char *str, uint32_t time_to_format, int offset) {
	if (time_to_format > 10) {
		sprintf(str + offset, "%d", time_to_format);
	} else if (time_to_format > 0) {
		sprintf(str + offset, "0%d", time_to_format);
	} else {
		sprintf(str + offset, "00.");
	}
}

// Return ms
void SysTick_Write_Time(char *str) {
	int n, MillisecondsToDisplay;
	
	MinutesElapsed = (MillisecondsElapsed % 3600000) / 60000;
	SecondsElapsed = (MillisecondsElapsed % 60000) / 1000;
	MillisecondsToDisplay = (MillisecondsElapsed % 1000) / 10;
	
	/*if (MinutesElapsed > 10) {
		sprintf(str, "%d:", MinutesElapsed);
	} else if (MinutesElapsed > 0) {
		sprintf(str, "0%d:", MinutesElapsed);
	} else {
		sprintf(str, "00:");
	}
	
	if (SecondsElapsed > 10) {
		sprintf(str + 3, "%d:", SecondsElapsed);
	} else if (SecondsElapsed > 0) {
		sprintf(str + 3, "0%d:", SecondsElapsed);
	} else {
		sprintf(str + 3, "00.");
	}	
	
	
	if (MillisecondsToDisplay > 10) {
		sprintf(str + 6, "%d", MillisecondsToDisplay);
	} else if (MillisecondsToDisplay > 0) {
		sprintf(str + 6, "0%d", MillisecondsToDisplay);
	} else {
		sprintf(str + 6, "00");
	}*/
	
	SysTick_Print_Time(str, MinutesElapsed, 0);
	sprintf(str + 2, ":");
	SysTick_Print_Time(str, SecondsElapsed, 3);
	sprintf(str + 5, ".");
	SysTick_Print_Time(str, MillisecondsToDisplay, 6);
}

// nTime: specifies the delay time length
void delay(uint32_t nTime) {
	TimeDelay = nTime;
	while(TimeDelay != 0);
}
