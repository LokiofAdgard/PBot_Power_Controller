#ifndef CANBUS_H
#define CANBUS_H

#include <stdint.h>

#include "INA231.h"
#include "stm32f1xx_hal.h"
#include "stm32f1xx_hal_can.h"
#include "stm32f1xx_hal_def.h"

#define CAN_ID_STA 0x100
#define CAN_ID_INA 0x101
#define CAN_ID_TMP 0x102

extern CAN_HandleTypeDef   hcan;
extern uint8_t             TxData[8];
extern CAN_RxHeaderTypeDef RxHeader;
extern uint8_t             RxData[8];

void              can_init(CAN_HandleTypeDef* hcan);
HAL_StatusTypeDef can_transmit(uint32_t txID);
HAL_StatusTypeDef can_transmit_ina(INA231_t* ina);

#endif