#include "stm32f4xx.h"                  // Device header
#include "System_Clock.h"
#include "SPI1_Lib.h"
#include "MAX7219.h"
#include <stdint.h>
#include "Waste_Time.h"

/*
					PIN MAP
PA4	-	SPI1_CS		-	MAX7219_CS
PA5	-	SPI1_CLK	-	MAX7219_CLK
PA7	-	SPI1_MOSI	-	MAX7219_DIN

since both of Segement and Matrix LED use the same CS
you need to configure new CS and rewrite a little bit
in oder to use both of them separately at the same time
*/

static uint8_t MatrixLED_map[8][8] ={	{0,0,0,0,0,0,0,0},
																			{0,1,1,0,0,1,1,0},
																			{1,0,0,1,1,0,0,1},
																			{1,0,0,1,1,0,0,1},
																			{0,1,1,0,0,1,1,0},
																			{0,0,1,0,0,1,0,0},
																			{0,0,0,1,1,0,0,0},
																			{0,0,0,0,0,0,0,0}};
int main (void){
	SystemClock_config();
	SPI1_config();

	// Working with matrix LED display
	MAX7219_MatrixLED_config();
	MAX7219_MatrixLED_drawMAP(MatrixLED_map);
	Wasting_time();
	MAX7219_MatrixLED_drawO();
	Wasting_time();
	MAX7219_MatrixLED_drawX();
	Wasting_time();
	MAX7219_MatrixLED_clear();
/*
	// Working with segment LED display
	MAX7219_SegmentLED_config();
	MAX7219_SegmentLED_greeting();
	Wasting_time();
	MAX7219_SegmentLED_displayINT(27021);
	Wasting_time();
	MAX7219_SegmentLED_displayFLOAT(-33.141035, 4);
	Wasting_time();
	MAX7219_SegmentLED_displayDATE(30, 4, 2024);
	Wasting_time();
	MAX7219_SegmentLED_clear();
*/
	return 0;
}
