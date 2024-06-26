#include "stm32f10x.h"                  // Device header
#include "SPI1_Lib.h"
#include <stdint.h>

void SPI1_config (void){
	
/*
	PA4 - SPI1_NSS
	PA5 - SPI1_SCLK
	PA6 - SPI1_MISO
	PA7 - SPI1_MOSI
	
	CPOL = 0
	CPHA = 0
	Full duplex
	Master mode
	APB2CLK = 84MHz and SPI1_div = 16
	SPI1_baudrate = APB2CLK / SPI1_div = 5.25MHz
	Send MSB first
	Data frame 8bit
*/

	// Enable GPIOA clock
	RCC->APB2ENR |= (1UL<<2);
	// Enable SPI1 CLock
	RCC->APB2ENR |= (1UL<<12);
	// Configure PA4, PA5, PA6 and PA7
	GPIOA->CRL = 0;
	GPIOA->CRL |= (11UL<<20); // PA5 (SCK) AF output Push Pull
	GPIOA->CRL |= (11UL<<28); // PA7 (MOSI) AF output Push Pull
	GPIOA->CRL |= (1UL<<26); // PA6 (MISO) Input mode (floating)
	GPIOA->CRL |= (3UL<<16); // PA4 used for CS, GPIO Output 
	// CPOL=0, CPHA=0
  SPI1->CR1 &= ~((1UL<<0) | (1UL<<1));   
	// Master Mode
  SPI1->CR1 |= (1UL<<2);  
	// Setup Baudrate 
  SPI1->CR1 |= (4UL<<3);
	// Send MSB first
  SPI1->CR1 &= ~(1UL<<7);
	// Software Slave Management, use GPIO instead of SPI1_NSS
  SPI1->CR1 |= (1UL<<8) | (1UL<<9);
	// Full-duplex
  SPI1->CR1 &= ~(1UL<<10);
	// Dataframe has 8 bits data
  SPI1->CR1 &= ~(1UL<<11); 
  SPI1->CR2 = 0;
	// Enable SPI1
	SPI1->CR1 |= (1UL<<6);   
}

void SPI1_control_NSS (uint8_t state){
	if(state) GPIOA->ODR |= (1UL<<4);
	else GPIOA->ODR &= ~(1UL<<4);
}

void SPI1_transmit (uint8_t *data, uint32_t size){
	uint32_t i = 0;
	// Reset NSS to start trasnmitting
	SPI1_control_NSS(0);
	while (i<size){
		// Wait for buffer tobe empty (TXE flag)
	  while (!((SPI1->SR)&(1<<1)));  
	  // Load the data into the Data Register
		SPI1->DR = data[i];
	  i++;
	}	
	// Wait for buffer is empty (TXE flag)
	while (!((SPI1->SR)&(1<<1)));
	// Wait for SPI to ready (BSY flag)	
	while (((SPI1->SR)&(1<<7)));  
	// Clear the Overrun flag by reading DR and SR
	i = SPI1->DR;
	i = SPI1->SR;
	// Set NSS to end transmitting
	SPI1_control_NSS(1);
}

void SPI1_receive (uint8_t *data, uint32_t size){
	// Reset NSS to start receiving
	SPI1_control_NSS(0);
	while (size){
		// Wait SPI to ready to communication (BSY flag)
		while ((SPI1->SR)&(1UL<<7));
		// Send dummy data to receive data
		SPI1->DR = 0;
		// Wait for Rx buffer to get some data
		while (!((SPI1->SR) &(1UL<<0)));
		*data++ = (uint8_t) (SPI1->DR);
		size--;
	}
	// Set NSS to end receiving
	SPI1_control_NSS(1);
}
