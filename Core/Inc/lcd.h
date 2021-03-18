#ifndef STM32_CAN_TRANSCEIVER_MASTER_LCD_H
#define STM32_CAN_TRANSCEIVER_MASTER_LCD_H

void LCD_delayMicros(uint16_t us);

void LCD_sendString(char *string);

void LCD_shiftOutHalfByte(uint8_t data, uint8_t rs);

void LCD_sendCommand(char cmd);

void LCD_sendData(char data);

void LCD_setCursor(int row, int col);

void LCD_init(void);

void LCD_test();

void LCD_clear();

void LCD_CANPrintRxInfo(CAN_RxHeaderTypeDef* header, uint8_t* data);

#endif //STM32_CAN_TRANSCEIVER_MASTER_LCD_H
