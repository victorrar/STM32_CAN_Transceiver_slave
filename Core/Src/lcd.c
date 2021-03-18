#include <main.h>
#include <stm32f1xx_hal_tim.h>
#include <string.h>
#include <stdio.h>
#include "lcd.h"

#define timer htim1
extern TIM_HandleTypeDef timer;


void LCD_delayMicros(uint16_t us) {
    __HAL_TIM_SET_COUNTER(&timer, 0);
    while (__HAL_TIM_GET_COUNTER(&timer) < us);
}

void LCD_shiftOutHalfByte(uint8_t data, uint8_t rs) {
    HAL_GPIO_WritePin(LCD_RS_GPIO_Port, LCD_RS_Pin, rs);  // rs = 1 for data, rs=0 for command
    /* write the data to the respective pin */
    HAL_GPIO_WritePin(LCD_data7_GPIO_Port, LCD_data7_Pin, ((data >> 3) & 0x01));
    HAL_GPIO_WritePin(LCD_data6_GPIO_Port, LCD_data6_Pin, ((data >> 2) & 0x01));
    HAL_GPIO_WritePin(LCD_data5_GPIO_Port, LCD_data5_Pin, ((data >> 1) & 0x01));
    HAL_GPIO_WritePin(LCD_data4_GPIO_Port, LCD_data4_Pin, ((data >> 0) & 0x01));
    /* Toggle EN PIN to send the data
     * if the HCLK > 100 MHz, use the  20 us delay
     * if the LCD still doesn't work, increase the LCD_delayMicros to 50, 80 or 100..
     */
    HAL_GPIO_WritePin(LCD_EN_GPIO_Port, LCD_EN_Pin, 1);
    LCD_delayMicros(20);
    HAL_GPIO_WritePin(LCD_EN_GPIO_Port, LCD_EN_Pin, 0);
    LCD_delayMicros(20);
}

void LCD_sendCommand(char cmd) {
    char datatosend;
    /* send upper nibble first */
    datatosend = ((cmd >> 4) & 0x0f);
    LCD_shiftOutHalfByte(datatosend, 0);  // RS must be while sending command
    /* send Lower Nibble */
    datatosend = ((cmd) & 0x0f);
    LCD_shiftOutHalfByte(datatosend, 0);
//    HAL_Delay(1);
}

void LCD_sendData(char data) {
    char datatosend;

    /* send higher nibble */
    datatosend = ((data >> 4) & 0x0f);
    LCD_shiftOutHalfByte(datatosend, 1);  // rs =1 for sending data
    /* send Lower nibble */
    datatosend = ((data) & 0x0f);
    LCD_shiftOutHalfByte(datatosend, 1);
//    HAL_Delay(1);
}

void LCD_setCursor(int row, int col) {
    switch (row) {
        case 0:
            col |= 0x80;
            break;
        case 1:
            col |= 0xC0;
            break;
    }
    LCD_sendCommand(col);
}

void LCD_init(void) {
    // 4 bit initialisation
    HAL_Delay(100);  // wait for >40ms
    LCD_shiftOutHalfByte(0b0011, 0);
    HAL_Delay(5);  // wait for >4.1ms
    LCD_shiftOutHalfByte(0b0011, 0);
    HAL_Delay(1);  // wait for >100us
    LCD_shiftOutHalfByte(0b0011, 0);
    HAL_Delay(1);
    LCD_shiftOutHalfByte(0b0010, 0);
    HAL_Delay(1);

    // dislay initialisation
    LCD_sendCommand(0x28); // Function set --> DL=0 (4 bit mode), N = 1 (2 line display) F = 0 (5x8 characters)
    HAL_Delay(1);
    LCD_sendCommand(0x08); //Display on/off control --> D=0,C=0, B=0  ---> display off
    HAL_Delay(1);
    LCD_sendCommand(0x01);  // clear display
    HAL_Delay(3);
    LCD_sendCommand(0x06); //Entry mode set --> I/D = 1 (increment cursor) & S = 0 (no shift)
    HAL_Delay(1);
    LCD_sendCommand(0x0C); //Display on/off control --> D = 1, C and B = 0. (Cursor and blink, last two bits)
}


void LCD_sendString(char *string) {
    uint8_t stringLength = strlen(string);
    for (int i = 0; i < stringLength; ++i) {
        LCD_sendData(string[i]);
    }
}

void LCD_test() {
    LCD_init();
//    LCD_sendCommand(0b00001111);
    LCD_setCursor(0, 0);
    LCD_sendString("test ");
    LCD_sendString("WORLD ");
    LCD_sendString("FROM");
    HAL_Delay(1000);
    LCD_setCursor(1, 0);

    for (int i = 0; i < 255; ++i) {
        LCD_setCursor(1, i % 16);
        LCD_sendString((char *) &i);
        HAL_Delay(1);
    }
//    LCD_clear();
}

void LCD_clear() {
    LCD_sendCommand(0b00000001);
}

void LCD_CANPrintRxInfo(CAN_RxHeaderTypeDef *header, uint8_t *data) {
    char stringBuff[17];
    char isR = header->RTR ? 'R' : 'r';
    char isI = header->RTR ? 'I' : 'i';
    sprintf(stringBuff, "ID:0x%03lX %c%c DL%lu", header->StdId, isR, isI, header->DLC);
    LCD_setCursor(0, 0);
    LCD_sendString(stringBuff);

    sprintf(stringBuff, "D0:0x%02X     RECV", header->DLC ? data[0] : 0x00);
    LCD_setCursor(1, 0);
    LCD_sendString(stringBuff);
}




