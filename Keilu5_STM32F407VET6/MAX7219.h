#include <stdint.h>
#ifndef MAX7219_SEGMENT
#define MAX7219_SEGMENT

// Register Address Map
#define NoOp_Address				0x00
#define Digit0_Address			0x01
#define Digit1_Address			0x02
#define Digit2_Address			0x03
#define Digit3_Address			0x04
#define Digit4_Address			0x05
#define Digit5_Address			0x06
#define Digit6_Address			0x07
#define Digit7_Address			0x08
#define DecodeMode_Address	0x09
#define Intensity_Address		0x0A
#define ScanLimit_Address		0x0B
#define Shutdown_Address		0x0C
#define DisplayTest_Address	0x0F

// Decode-Mode Register Value
#define Decode_Disable			0x00
#define Decode_Digit0 			0x01
#define Decode_Digit1 			0x02
#define Decode_Digit2 			0x04
#define Decode_Digit3 			0x08
#define Decode_Digit4 			0x10
#define Decode_Digit5 			0x20
#define Decode_Digit6 			0x40
#define Decode_Digit7 			0x80
#define Decode_All_Digit		0xFF

// Code B Font
#define CodeB_0							0x00
#define CodeB_1							0x01
#define CodeB_2							0x02
#define CodeB_3							0x03
#define CodeB_4							0x04
#define CodeB_5							0x05
#define CodeB_6							0x06
#define CodeB_7							0x07
#define CodeB_8							0x08
#define CodeB_9							0x09
#define CodeB_Negative			0x0A
#define CodeB_E							0x0B
#define CodeB_H							0x0C
#define CodeB_L							0x0D
#define CodeB_P							0x0E
#define CodeB_Blank					0x0F
#define CodeB_Dot						0x80

// Intensity Register Value
#define DutyCycle_1_32			0x00
#define DutyCycle_3_32			0x01
#define DutyCycle_5_32			0x02
#define DutyCycle_7_32			0x03
#define DutyCycle_9_32			0x04
#define DutyCycle_11_32			0x05
#define DutyCycle_13_32			0x06
#define DutyCycle_15_32			0x07
#define DutyCycle_17_32			0x08
#define DutyCycle_19_32			0x09
#define DutyCycle_21_32			0x0A
#define DutyCycle_23_32			0x0B
#define DutyCycle_25_32			0x0C
#define DutyCycle_27_32			0x0D
#define DutyCycle_29_32			0x0E
#define DutyCycle_31_32			0x0F

// Scan-Limit Register Value
#define Display_digit0_only	0x00
#define Display_digit0_to_1	0x01
#define Display_digit0_to_2	0x02
#define Display_digit0_to_3	0x03
#define Display_digit0_to_4	0x04
#define Display_digit0_to_5	0x05
#define Display_digit0_to_6	0x06
#define Display_digit0_to_7	0x07

// Shutdown Register Value
#define ShutdownMode_Enable		0x00
#define ShutdownMode_Disable	0x01

// Display-Test Register Value
#define DisplayTestMode_Enable	0x01
#define DisplayTestMode_Disable	0x00

void Send_Data (uint8_t Address, uint8_t Data);
void MAX7219_turn_off (void);
void MAX7219_turn_on (void);
void MAX7219_SegmentLED_config (void);
void MAX7219_SegmentLED_clear (void);
void MAX7219_SegmentLED_displayINT (int n);
void MAX7219_SegmentLED_displayFLOAT (double n, uint8_t len);
void MAX7219_SegmentLED_displayDATE (uint8_t date, uint8_t month, uint16_t year);
void MAX7219_SegmentLED_greeting (void);
void MAX7219_MatrixLED_config (void);
void MAX7219_MatrixLED_drawMAP (uint8_t map[8][8]);
void MAX7219_MatrixLED_drawO (void);
void MAX7219_MatrixLED_drawX (void);
void MAX7219_MatrixLED_clear (void);

#endif
