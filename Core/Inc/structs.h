#ifndef STRUCTS
#define STRUCTS

#include <stdint.h>

#include "INA231.h"
#include "TMP102.h"

#define TMP_ADDR (0x48 << 1)

#define INA_SOL_ADDR  (0x40 << 1)
#define INA_MPPT_ADDR (0x41 << 1)
#define INA_BAT_ADDR  (0x42 << 1)
#define INA_V5_ADDR   (0x43 << 1)
#define INA_V12A_ADDR (0x44 << 1)
#define INA_V12B_ADDR (0x45 << 1)

typedef enum {
    MODE_INIT    = 0x00,
    MODE_PWR_ON  = 0x01,
    MODE_PWR_SAV = 0x02,
    MODE_ERR     = 0x03
} Mode_t;

typedef struct PowerController {
    union {
        struct {
            uint16_t mode : 2;

            uint16_t sol : 1;
            uint16_t solOut : 1;
            uint16_t mpptIn : 1;
            uint16_t bat : 1;

            uint16_t en_v5 : 1;
            uint16_t en_v12a : 1;
            uint16_t en_v12b : 1;

            uint16_t en_fan : 1;

            uint16_t req_stat : 1;
            uint16_t req_ina : 1;
            uint16_t req_all : 1;
            uint16_t req : 2;
        } bits;
        uint16_t raw;
    } state;

    INA231_t inaSol;
    INA231_t inaMppt;
    INA231_t inaBat;
    INA231_t inaV5;
    INA231_t inaV12A;
    INA231_t inaV12B;

    TMP102_t tmp;
} PowerController;

void pc_init(PowerController* pc, I2C_HandleTypeDef* hi2c);
void pc_check_dc_in(PowerController* pc);
void pc_ina_update(PowerController* pc);
void pc_tmp_update(PowerController* pc);
void pc_set_mode(PowerController* pc);
void pc_mode_update(PowerController* pc);
void pc_reply(PowerController* pc);

#endif