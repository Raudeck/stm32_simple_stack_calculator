#ifndef I2C_H_
#define I2C_H

#include "main.h"

extern void I2C_START_Condition();
extern void I2C_STOP_Condition();
extern void I2C_CMD_Transmission(uint16_t data);

#define INPUT 1
#define OUTPUT 0

#endif 