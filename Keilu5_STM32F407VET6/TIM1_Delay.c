#include "stm32f4xx.h"                  // Device header
#include "TIM1_Delay.h"

void TIM1_config (void){
	
/*
	APB2CLK = 84MHz
	CLK_to_TIM1 = APB2CLK * 2 = 168MHz (APB2_PRESC != 1)
	TIM1_PSC = 16800
	TIM1_CLK = CLK_to_TIM1 / TIM1_PSC = 0.01MHz
	Each count of TIM1 takes 100ns
	So 1ms would cost 10 counts
*/

	// Enable TIM1 clock
	RCC->APB2ENR |= (1UL<<0);
	// Configure TIM1 prescaler
	TIM1->PSC = 16800-1;
	// Set max count
	TIM1->ARR = 0xFFFF;
	// reset counter
	TIM1->CNT = 0;
	// Enable counter
	TIM1->CR1 |= (1UL<<0);
}

void Delay_ms (uint32_t time){
	for (uint32_t i=0; i<time; i++){
		TIM1->CNT = 0;
		while (TIM1->CNT < time);
	}
}
