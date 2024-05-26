#include "stm32f4xx.h"                  // Device header
#include "MAX7219.h"
#include "SPI1_Lib.h"
#include <stdint.h>
#include <math.h>

/*
Function Name: Send_Data
Input: uint8_t Address, uint8_t Data
Output: N/A

Description:
-	Send 16bits of dataframe to particular address of MAX7219 with SPI protocol
-	The 16bits of dataframe includes:
	bit0-7: transmited data
	bit8-11: particular address of MAX7219
	bit12-15: reverse (dont care)
-	SPI property:
	CPOL = 0
	CPHA = 0
	baudrate: 5.25MHz (<10MHz)
	full duplex
	master mode
	send MSB first
	8bit of dataframe 
*/

void Send_Data (uint8_t Address, uint8_t Data){
	uint8_t temp[2] = {Address, Data};
	SPI1_transmit(temp, 2);
}

/*
Function Name: MAX7219_turn_off
Input: N/A
Output: N/A

Description:
-	Enable Shutdown mode of MAX7219
*/

void MAX7219_turn_off (void){
	Send_Data(Shutdown_Address, ShutdownMode_Enable);
}

/*
Function Name: MAX7219_turn_on
Input: N/A
Output: N/A

Description:
-	Disable Shutdown mode of MAX7219
*/

void MAX7219_turn_on (void){
	Send_Data(Shutdown_Address, ShutdownMode_Disable);
}

/*
Function Name: MAX7219_SegmentLED_config
Input: N/A
Output: N/A

Description:
-	This function initializes MAX7219 properties inlcuding:
	enable decode mode: display all 8 digits of segment LED with Code B Font
	set intensity: configure the duty cycle
	set scanline: enable all 8 digits of segment LED tobe displayable
	disable shutdown mode: turn on MAX7219
	clear all digits
*/

void MAX7219_SegmentLED_config (void){
	// Enable decode mode
	Send_Data(DecodeMode_Address, Decode_All_Digit);
	// Set intensity
	Send_Data(Intensity_Address, DutyCycle_19_32);
	// Set scanlimit
	Send_Data(ScanLimit_Address, Display_digit0_to_7);
	// Disable shutdown mode
	Send_Data(Shutdown_Address, ShutdownMode_Disable);
	// Turn off display test
	Send_Data(DisplayTest_Address, DisplayTestMode_Disable);
	// Clear all
	for(uint8_t address=0x01; address<=0x08; address++)
		Send_Data(address, CodeB_Blank);
}

/*
Function Name: MAX7219_SegmentLED_clear
Input: N/A
Output: N/A

Description:
-	This function will clear all context on segment LED display by showing blanks instead of numbers
*/

void MAX7219_SegmentLED_clear (void){
	for(uint8_t address=0x01; address<=0x08; address++)
		Send_Data(address, CodeB_Blank);
}

/*
Function Name: MAX7219_SegmentLED_displayINT
Input: int n
Output: N/A

Description:
-	This function will display an interger number <n> on the segment LED display
-	The idea of this function is to display digits in the given interger one-by-one from the rightmost position
-	Whenever displaying digits in that given number is completed, display the negative sign if given number is negative
-	Start display blanks on the rest of the display if 2 upper steps are completed
*/

void MAX7219_SegmentLED_displayINT (int n){
	// sign=1 if n is positive else sign=0
	uint8_t sign = (n>0) ? 1:0;
	// Start showing blanks whenever show_blank=1
	uint8_t show_blank = 0;
	// Make n tobe a positve number
	n = (n<0) ? (n*(-1)):n;
	// Display Code B Font on each digit
	for(uint8_t address=Digit0_Address; address<=Digit7_Address; address++){
		// Display the rightmost number of given interger on particular digit if show_blank=0, else, display blank
		Send_Data(address, show_blank ? CodeB_Blank : (uint8_t)(n%10));
		// Display negative sign whenever completed displaying number
		if(!n && !sign){
			Send_Data(address, CodeB_Negative);
			sign = ~sign;
		}
		// Refresh n and check if displaying number is completed
		n/=10;
		if(n==0) show_blank = 1;
	}
}

/*
Function Name: MAX7219_SegmentLED_displayFLOAT
Input: double n, uint8_t len
Output: N/A

Description:
-	This function display a floating point number on the segment LED display with a number of <len> digits go after the dot
-	Simply transform floating point number into an interger
-	Display that interger onto the segment LED
-	Add a dot to the right position
*/

void MAX7219_SegmentLED_displayFLOAT (double n, uint8_t len){
	// Convert float into int and display it onto segment LED display
	int interger = (int)(n*pow(10,len));
	uint8_t data, address = Digit0_Address + len;
	MAX7219_SegmentLED_displayINT(interger);
	// Get the digit which the dot follow right after
	interger = (interger >= 0) ? interger : (-1)*interger;
	data = (uint8_t)(((int)(interger/pow(10,len))%10));
	// Add the dot into that number and display it
	data |= (1UL<<7);
	Send_Data(address, data);
}

/*
Function Name: MAX7219_SegmentLED_displayDATE
Input: uint8_t date, uint8_t month, uint16_t year
Output: N/A

Description:
-	Display a date in format of DD-MM-YY
*/

void MAX7219_SegmentLED_displayDATE (uint8_t date, uint8_t month, uint16_t year){
	Send_Data(Digit0_Address, (uint8_t)(year%10));
	Send_Data(Digit1_Address, (uint8_t)(year/10%10));
	Send_Data(Digit2_Address, CodeB_Negative);
	Send_Data(Digit3_Address, month%10);
	Send_Data(Digit4_Address, month/10%10);
	Send_Data(Digit5_Address, CodeB_Negative);
	Send_Data(Digit6_Address, date%10);
	Send_Data(Digit7_Address, date/10%10);
}

/*
Function Name: MAX7219_SegmentLED_greeting
Input: N/A
Output: N/A

Description:
-	Display "HELLO" on the digit 1 to 5
*/

void MAX7219_SegmentLED_greeting (void){
	Send_Data(Digit1_Address, CodeB_0);
	Send_Data(Digit2_Address, CodeB_L);
	Send_Data(Digit3_Address, CodeB_L);
	Send_Data(Digit4_Address, CodeB_E);
	Send_Data(Digit5_Address, CodeB_H);
}

/*
Function Name: MAX7219_MatrixLED_config
Input: N/A
Output: N/A

Description:
-	This function initializes MAX7219 properties inlcuding:
	disable decode mode: set and reset each LED by hand
	set intensity: configure the duty cycle
	set scanline: enable all 8 digits of segment LED tobe displayable
	disable shutdown mode: turn on MAX7219
	turn off all LED
*/

void MAX7219_MatrixLED_config (void){
	// Disable decode mode
	Send_Data(DecodeMode_Address, Decode_Disable);
	// Set intensity
	Send_Data(Intensity_Address, DutyCycle_19_32);
	// Set scanlimit
	Send_Data(ScanLimit_Address, Display_digit0_to_7);
	// Disable shutdown mode
	Send_Data(Shutdown_Address, ShutdownMode_Disable);
	// Turn off display test
	Send_Data(DisplayTest_Address, DisplayTestMode_Disable);
	// turn off all LED
	for(uint8_t address=0x01; address<=0x08; address++)
		Send_Data(address, 0x00);
}

void MAX7219_MatrixLED_drawMAP (uint8_t map[8][8]){
	// Each index of list is collum address (i=0 so address=i+1)
	uint8_t list[8] = {0,0,0,0,0,0,0,0};
	for(uint8_t collum=0; collum<8; collum++){
		for(uint8_t row=0; row<8; row++){
			if(map[row][collum]){
				list[collum] |= (1<<row);
			}
		}
	}
	for(uint8_t collum=0; collum<8; collum++){
		Send_Data(collum+1, list[collum]);
	}
}

/*
Function Name: MAX7219_MatrixLED_drawO
Input: N/A
Output: N/A

Description:
-	Display O onto matrix LED display
*/

void MAX7219_MatrixLED_drawO (void){
	Send_Data(0x01, 0x3C);
	Send_Data(0x02, 0x42);
	Send_Data(0x03, 0x81);
	Send_Data(0x04, 0x81);
	Send_Data(0x05, 0x81);
	Send_Data(0x06, 0x81);
	Send_Data(0x07, 0x42);
	Send_Data(0x08, 0x3C);
}

/*
Function Name: MAX7219_MatrixLED_config
Input: N/A
Output: N/A

Description:
-	Display X onto matrix LED display
*/

void MAX7219_MatrixLED_drawX (void){
	Send_Data(0x01, 0x81);
	Send_Data(0x02, 0x42);
	Send_Data(0x03, 0x24);
	Send_Data(0x04, 0x18);
	Send_Data(0x05, 0x18);
	Send_Data(0x06, 0x24);
	Send_Data(0x07, 0x42);
	Send_Data(0x08, 0x81);
}

/*
Function Name: MAX7219_MatrixLED_config
Input: N/A
Output: N/A

Description:
-	Clear screen by turning off all LED on the screen
*/

void MAX7219_MatrixLED_clear (void){
	for(uint8_t address=1; address<9; address++){
		Send_Data(address, 0x00);
	}
}
