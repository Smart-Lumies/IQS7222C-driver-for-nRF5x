/** @file i2c_touch.c 
*
* @brief A description of the module's purpose.
*
* @par
* COPYRIGHT NOTICE: (c) 2020 Smart Lumies d.o.o.
* All rights reserved.
*/

//------------------------------ INCLUDES -------------------------------------
#include <i2c_touch.h>
#include "i2c.h"
#include <stdio.h>
#include "nrf_error.h"
#include "sdk_errors.h"

//-------------------------------- MACROS -------------------------------------

//----------------------------- DATA TYPES ------------------------------------

//--------------------- PRIVATE FUNCTION PROTOTYPES ---------------------------

//----------------------- STATIC DATA & CONSTANTS -----------------------------

static nrf_drv_twi_t *pTwi;
//------------------------------ GLOBAL DATA ----------------------------------

//---------------------------- PUBLIC FUNCTIONS -------------------------------
void i2c_touch_init(const nrf_drv_twi_t *mTwi)
{

	if (mTwi != NULL)
	{
		pTwi = mTwi;
	}
}

int i2c_touch_write_register(uint8_t I2Caddress, uint8_t reg, uint32_t len, uint8_t const *data, bool stop)
{
	// Data to be sent over TWI is {reg,data} -- reg = internal register of Sensor to which data is written

	ret_code_t retCode;
	uint8_t buff[len + 1];
	buff[0] = reg;
	memcpy(buff + sizeof(reg), data, len);
	retCode = nrf_drv_twi_tx(pTwi, I2Caddress, buff, sizeof(buff), stop);
	return retCode;
}

int i2c_touch_write_register_16(uint8_t I2Caddress, uint16_t reg, uint32_t len, uint8_t const *data, bool stop)
{
	// Data to be sent over TWI is {reg,data} -- reg = internal register of Sensor to which data is written

	ret_code_t retCode;
	uint8_t buff[len + sizeof(reg)];
	memcpy(buff, reg, sizeof(reg));
	memcpy(buff + sizeof(reg), data, len);
	retCode = nrf_drv_twi_tx(pTwi, I2Caddress, buff, sizeof(buff), stop);
	return retCode;
}

int i2c_touch_read_register(uint8_t I2Caddress, uint8_t reg, uint32_t len, uint8_t *buff, bool stop)
{
	ret_code_t retCode;
	retCode = nrf_drv_twi_tx(pTwi, I2Caddress, &reg, 1, stop);
	retCode = nrf_drv_twi_rx(pTwi, I2Caddress, buff, len);
	return retCode;
}

//--------------------------- PRIVATE FUNCTIONS -------------------------------

//--------------------------- INTERRUPT HANDLERS ------------------------------
