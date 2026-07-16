#ifndef CANBUS_H
#define CANBUS_H

#include <stdint.h>

#include "INA231.h"
#include "stm32f1xx_hal.h"
#include "stm32f1xx_hal_can.h"
#include "stm32f1xx_hal_def.h"
#include "structs.h"

typedef enum {
    CAN_ID_STA = 0x100,
    CAN_ID_INA = 0x101
} Txid_t;

typedef enum {
    SET_MODE = 0x200,
    DATA_REQ = 0x201
} Rxid_t;

typedef enum {
    GET_STAT   = 0x00,
    GET_INA    = 0x01,
    GET_ALL    = 0x02,
    SET_PWR_ON = 0x03,
    SET_PWR_SAV = 0x04
} Req_t;

extern CAN_HandleTypeDef   hcan;
extern uint8_t             TxData[8];
extern CAN_RxHeaderTypeDef RxHeader;
extern uint8_t             RxData[8];

void              can_init(CAN_HandleTypeDef* hcan);
HAL_StatusTypeDef can_transmit(uint32_t txID);
HAL_StatusTypeDef can_transmit_ina(INA231_t* ina);
HAL_StatusTypeDef can_transmit_sta(PowerController* pc);

#endif