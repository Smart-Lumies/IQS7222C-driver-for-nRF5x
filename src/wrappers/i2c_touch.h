/** @file i2c_touch.h
 *
 * @brief See source file.
 *
 * @par
 * COPYRIGHT NOTICE: (c) 2020 Smart Lumies d.o.o.
 * All rights reserved.
 */

#ifndef I2C_TOUCH_H
#define I2C_TOUCH_H

#ifdef __cplusplus
extern "C" {
#endif

//------------------------------ INCLUDES -------------------------------------
#include "nrf_drv_twi.h"
#include <stdbool.h>
#include <stdint.h>

//-------------------------- CONSTANTS & MACROS -------------------------------

//----------------------------- DATA TYPES ------------------------------------

//---------------------- PUBLIC FUNCTION PROTOTYPES ---------------------------
void i2c_touch_init(const nrf_drv_twi_t *mTwi);
int i2c_touch_write_register(uint8_t I2Caddress, uint8_t reg, uint32_t len,
                             uint8_t const *data, bool stop);
int i2c_touch_write_register_16(uint8_t I2Caddress, uint16_t reg, uint32_t len,
                                uint8_t const *data, bool stop);
int i2c_touch_read_register(uint8_t I2Caddress, uint8_t reg, uint32_t len,
                            uint8_t *buff, bool stop);

#ifdef __cplusplus
}
#endif

#endif // I2C_TOUCH_H
