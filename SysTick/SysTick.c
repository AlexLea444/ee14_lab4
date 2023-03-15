#include "stm32l476xx.h"
#include <stdio.h>
#include <stdbool.h>

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
void SysTick_Handler(bool pause) {
	TimeDelay--;
	if (pause == false) {
		MillisecondsElapsed++;
	}
}

void Joy_Init(void){
	/* Enable GPIOs clock */ 	
	RCC->AHB2ENR |= RCC_AHB2ENR_GPIOBEN | RCC_AHB2ENR_GPIOEEN;
	
	///////////////////////////////////////////////////////////////////////////////////////////////
	// LD4 Red = PB2
	///////////////////////////////////////////////////////////////////////////////////////////////
	// GPIO Mode: Input(00), Output(01), AlterFunc(10), Analog(11, reset)
	GPIOB->MODER &= ~(3U<<(2*2));  
	GPIOB->MODER |= 1U<<(2*2);      //  Output(01)
	
	// GPIO Speed: Low speed (00), Medium speed (01), Fast speed (10), High speed (11)
	GPIOB->OSPEEDR &= ~(3U<<(2*2));
	GPIOB->OSPEEDR |=   3U<<(2*2);  // High speed
	
	// GPIO Output Type: Output push-pull (0, reset), Output open drain (1) 
	GPIOB->OTYPER &= ~(1U<<2);       // Push-pull
	
	// GPIO Push-Pull: No pull-up, pull-down (00), Pull-up (01), Pull-down (10), Reserved (11)
	GPIOB->PUPDR   &= ~(3U<<(2*2));  // No pull-up, no pull-down
	
	///////////////////////////////////////////////////////////////////////////////////////////////
	// LD5 Green = PE8
	///////////////////////////////////////////////////////////////////////////////////////////////
	// GPIO Mode: Input(00, reset), Output(01), AlterFunc(10), Analog(11, reset)
	GPIOE->MODER &= ~(3U<<(2*8));  
	GPIOE->MODER |= 1U<<(2*8);      //  Output(01)
	
	// GPIO Speed: Low speed (00), Medium speed (01), Fast speed (10), High speed (11)
	GPIOE->OSPEEDR &= ~(3U<<(2*8));
	GPIOE->OSPEEDR |=   3U<<(2*8);  // High speed
	
	// GPIO Output Type: Output push-pull (0, reset), Output open drain (1) 
	GPIOE->OTYPER &= ~(1U<<8);       // Push-pull
	
	// GPIO Push-Pull: No pull-up, pull-down (00), Pull-up (01), Pull-down (10), Reserved (11)
	GPIOE->PUPDR   &= ~(3U<<(2*8));  // No pull-up, no pull-down
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
