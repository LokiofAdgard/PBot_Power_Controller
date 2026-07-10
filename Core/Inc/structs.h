#ifndef STRUCTS
#define STRUCTS

#include <stdint.h>

#include "INA231.h"

typedef enum {
    MODE_STARTUP = 0x00,
    MODE_PWR_ON  = 0x01,
    MODE_PWR_SAV = 0x02,
    MODE_ERR     = 0x03
} Mode_t;

typedef enum {
    REQ_X = 0x00
} Req_t;

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

            uint16_t req : 4;
        } bits;
        uint16_t raw;
    } state;

    INA231_t inaSol;
    INA231_t inaMppt;
    INA231_t inaBat;
    INA231_t inaV5;
    INA231_t inaV12A;
    INA231_t inaV12B;

    uint16_t temperature;
} PowerController;

#endif