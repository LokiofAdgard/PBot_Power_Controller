#include "INA231.h"

#include <stdint.h>

#include "stm32f1xx_hal_def.h"
#include "stm32f1xx_hal_i2c.h"

HAL_StatusTypeDef INA231_Init(INA231_t* dev, I2C_HandleTypeDef* hi2c, uint8_t addr, float r_shunt, float current_lsb) {
    dev->i2c     = hi2c;
    dev->address = addr;

    dev->r_shunt     = r_shunt;
    dev->current_lsb = current_lsb;

    dev->config  = DEFAULT_CONFIG;
    dev->calib   = 5120 / (dev->r_shunt * dev->current_lsb);
    dev->timeout = 100;

    HAL_StatusTypeDef status;

    // Calibration
    uint8_t data[2];
    data[0] = (dev->calib >> 8) & 0xFF;
    data[1] = (dev->calib) & 0xFF;

    status = HAL_I2C_Mem_Write(dev->i2c, dev->address, CALIB_ADDR, I2C_MEMADD_SIZE_8BIT, data, REG_SIZE, dev->timeout);

    dev->status = status;
    return status;
}

HAL_StatusTypeDef INA231_GetVals(INA231_t* dev) {
    uint8_t           buf[6];
    HAL_StatusTypeDef status[3];

    status[0] = HAL_I2C_Mem_Read(dev->i2c, dev->address, VOL_ADDR, I2C_MEMADD_SIZE_8BIT, &buf[0], REG_SIZE, dev->timeout);
    status[1] = HAL_I2C_Mem_Read(dev->i2c, dev->address, PWR_ADDR, I2C_MEMADD_SIZE_8BIT, &buf[2], REG_SIZE, dev->timeout);
    status[2] = HAL_I2C_Mem_Read(dev->i2c, dev->address, CUR_ADDR, I2C_MEMADD_SIZE_8BIT, &buf[4], REG_SIZE, dev->timeout);

    if (status[0] != HAL_OK || status[1] != HAL_OK || status[2] != HAL_OK) {
        dev->status = HAL_ERROR;
        return HAL_ERROR;
    }

    dev->v_reg = ((buf[0] << 8) | buf[1]);  // 0x2570
    dev->p_reg = ((buf[2] << 8) | buf[3]);  // 0x12B8
    dev->i_reg = ((buf[4] << 8) | buf[5]);  // 0x2710

    // Test
    dev->v_val = dev->v_reg * VOLTAGE_LSB;
    dev->p_val = dev->p_reg * (25.0f * (dev->current_lsb / 1000.0f));
    dev->i_val = dev->i_reg * dev->current_lsb;

    return HAL_OK;
}

HAL_StatusTypeDef INA231_Reset(INA231_t* dev) {
    uint16_t          config;
    uint8_t           data[2];
    HAL_StatusTypeDef status;

    config = dev->config | RESET_MASK;

    data[0] = (config >> 8) & 0xFF;
    data[1] = config & 0xFF;

    status = HAL_I2C_Mem_Write(dev->i2c, dev->address, 0x00, I2C_MEMADD_SIZE_8BIT, data, REG_SIZE, dev->timeout);

    dev->status = status;
    return status;
}
