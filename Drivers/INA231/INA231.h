#ifndef INA231_H
#define INA231_H

#include <stdint.h>

#include "stm32f1xx_hal.h"
#include "stm32f1xx_hal_def.h"

#define DEFAULT_R_SHUNT     100  // mOhm
#define DEFAULT_CURRENT_LSB 0.4  // mA (x2^15 > Imax)

#define VOLTAGE_LSB 1.25e-3f

#define DEFAULT_CONFIG  0x4127
#define DEFAULT_TIMEOUT 100

#define REG_SIZE    0x02
#define CONFIG_ADDR 0x00
#define CALIB_ADDR  0x05
#define VOL_ADDR    0x02
#define PWR_ADDR    0x03
#define CUR_ADDR    0x04

#define RESET_MASK 0x8000

typedef struct INA231_t {
    // Device
    I2C_HandleTypeDef* i2c;
    uint16_t           address;
    HAL_StatusTypeDef  status;

    // Config
    uint16_t config;
    uint16_t calib;
    uint8_t  timeout;
    float r_shunt;
    float current_lsb;

    // Outputs
    int16_t i_reg;
    int16_t v_reg;
    int16_t p_reg;

    // Test
    float v_val;  // V
    float i_val;  // mA
    float p_val;  // W

} INA231_t;

HAL_StatusTypeDef INA231_Init(INA231_t* dev, I2C_HandleTypeDef* hi2c, uint8_t addr, float r_shunt, float current_lsb);
HAL_StatusTypeDef INA231_GetVal_Test(INA231_t* dev, int16_t* value);
HAL_StatusTypeDef INA231_GetVals(INA231_t* dev);
HAL_StatusTypeDef INA231_Reset(INA231_t* dev);

#endif