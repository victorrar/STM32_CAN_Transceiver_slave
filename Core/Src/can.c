#include "can.h"
#include "main.h"
#include "lcd.h"

extern CAN_HandleTypeDef hcan;

void CAN_Mytest() {
    CAN_TxHeaderTypeDef asd;
    uint16_t CAN_ID;
    uint8_t CAN_DLC;
    uint8_t DATA[8];
    DATA[0]=0xDE;

    asd.DLC = 1;
    asd.StdId = 0x123;
    asd.RTR = 0;
    asd.ExtId = 0;
    asd.IDE = 0;
    while(!HAL_CAN_GetTxMailboxesFreeLevel(&hcan)){}
    uint32_t mailbox;
    HAL_CAN_AddTxMessage(&hcan, &asd, DATA, &mailbox);


    for(int a = 0; a < 10; a++){

    }
}

void CAN_Init() {
    CAN_FilterTypeDef filterCfg;
    filterCfg.FilterMode = CAN_FILTERMODE_IDMASK;
    filterCfg.FilterScale = CAN_FILTERSCALE_16BIT;

    filterCfg.FilterBank = 0;
    filterCfg.FilterFIFOAssignment = CAN_FILTER_FIFO0;
    filterCfg.FilterIdHigh = 0;
    filterCfg.FilterIdLow = 0;
    filterCfg.FilterMaskIdLow = 0;
    filterCfg.FilterMaskIdHigh = 0;
    filterCfg.FilterActivation = CAN_FILTER_ENABLE;

    HAL_CAN_ConfigFilter(&hcan, &filterCfg);

    HAL_CAN_Start(&hcan);

}

void CAN_loopTick() {
    if (HAL_CAN_GetRxFifoFillLevel(&hcan, CAN_RX_FIFO0)) {
        
        CAN_RxHeaderTypeDef rxheader;
        uint8_t data[8];

        HAL_CAN_GetRxMessage(&hcan, CAN_RX_FIFO0, &rxheader, data);
        LCD_CANPrintRxInfo(&rxheader, data);
    }
}

