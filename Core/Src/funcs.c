#include "INA231.h"
#include "TMP102.h"
#include "canbus.h"
#include "stm32f103xb.h"
#include "stm32f1xx_hal_gpio.h"
#include "structs.h"

void pc_init(PowerController* pc, I2C_HandleTypeDef* hi2c) {
    pc->state.raw       = 0x0000;
    pc->state.bits.mode = MODE_INIT;

    INA231_Init(&pc->inaSol, hi2c, INA_SOL_ADDR, DEFAULT_R_SHUNT, DEFAULT_CURRENT_LSB);
    // INA231_Init(&pc->inaSol, hi2c, INA_MPPT_ADDR, DEFAULT_R_SHUNT, DEFAULT_CURRENT_LSB);
    // INA231_Init(&pc->inaSol, hi2c, INA_BAT_ADDR, DEFAULT_R_SHUNT, DEFAULT_CURRENT_LSB);
    // INA231_Init(&pc->inaSol, hi2c, INA_V5_ADDR, DEFAULT_R_SHUNT, DEFAULT_CURRENT_LSB);
    // INA231_Init(&pc->inaSol, hi2c, INA_V12A_ADDR, DEFAULT_R_SHUNT, DEFAULT_CURRENT_LSB);
    // INA231_Init(&pc->inaSol, hi2c, INA_V12B_ADDR, DEFAULT_R_SHUNT, DEFAULT_CURRENT_LSB);

    TMP102_init(&pc->tmp, hi2c, TMP_ADDR);
    pc_check_dc_in(pc);
}

void pc_check_dc_in(PowerController* pc) {
    GPIO_PinState activeState = GPIO_PIN_RESET;

    pc->state.bits.sol    = (HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_5) == activeState);
    pc->state.bits.solOut = (HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_6) == activeState);
    pc->state.bits.mpptIn = (HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_7) == activeState);
    pc->state.bits.bat    = (HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_0) == activeState);
}

void pc_ina_update(PowerController* pc) {
    INA231_GetVals(&pc->inaSol);
    // INA231_GetVals(&pc->inaMppt);
    // INA231_GetVals(&pc->inaBat);
    // INA231_GetVals(&pc->inaV5);
    // INA231_GetVals(&pc->inaV12A);
    // INA231_GetVals(&pc->inaV12B);

    TMP102_getVal(&pc->tmp);
}

void pc_set_mode(PowerController* pc) {
    return;
}

void pc_update_mode(PowerController* pc) {
    switch (pc->state.bits.mode) {
        case MODE_INIT:
            return;
        case MODE_PWR_ON:
            return;
        case MODE_PWR_SAV:
            return;
        case MODE_ERR:
            return;
    }
}

void pc_reply(PowerController* pc) {
    if (pc->state.bits.req_all) {
        pc->state.bits.req_all = 0;
        can_transmit_sta(pc);
        can_transmit_ina(&pc->inaSol);
        return;
    }

    if (pc->state.bits.req_stat) {
        pc->state.bits.req_stat = 0;
        can_transmit_sta(pc);
    }
    if (pc->state.bits.req_ina) {
        pc->state.bits.req_ina = 0;
        can_transmit_ina(&pc->inaSol);
    }
}
