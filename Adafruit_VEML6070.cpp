/*!
 * @file Adafruit_VEML6070.cpp
 *
 * @mainpage Adafruit VEML6070 UV sensor
 *
 * @section intro_sec Introduction
 *
 * Designed specifically to work with the VEML6070 sensor from Adafruit
 * ----> https://www.adafruit.com/products/2899
 *
 * These sensors use I2C to communicate, 2 pins (SCL+SDA) are required
 * to interface with the breakout.
 *
 * Adafruit invests time and resources providing this open source code,
 * please support Adafruit and open-source hardware by purchasing
 * products from Adafruit!
 *
 * @section dependencies Dependencies
 *
 *
 * @section author Author
 *
 * Written by Limor Fried/Ladyada for Adafruit Industries.
 *
 * @section license License
 *
 * BSD license, all text here must be included in any redistribution.
 *
 */

#if (ARDUINO >= 100)
 #include "Arduino.h"
#else
 #include "WProgram.h"
#endif
#include "Wire.h"

#include "Adafruit_VEML6070.h"


/**************************************************************************/
/*! 
    @brief  setup and initialize communication with the hardware
    @param itime the integration time to use for the data
    @param twoWire Optional pointer to the desired TwoWire I2C object. Defaults to &Wire
*/
/**************************************************************************/
void Adafruit_VEML6070::begin(veml6070_integrationtime_t itime, TwoWire *twoWire) {
  _i2c = twoWire;

  //default setting
  _commandRegister.reg = 0x02;
  _commandRegister.bit.IT = itime;

  _i2c->begin();
  _i2c->beginTransmission(VEML6070_ADDR_L);
  _i2c->write(_commandRegister.reg);
  _i2c->endTransmission();
  delay(500);
}

/**************************************************************************/
/*! 
    @brief  read the chips UV sensor
    @return the UV reading as a 16 bit integer
*/
/**************************************************************************/
uint16_t Adafruit_VEML6070::readUV() {
  if (_i2c->requestFrom(VEML6070_ADDR_H, 1) != 1) return -1;
  uint16_t uvi = _i2c->read();
  uvi <<= 8;
  if (_i2c->requestFrom(VEML6070_ADDR_L, 1) != 1) return -1;
  uvi |= _i2c->read();

  return uvi;  
}

/**************************************************************************/
/*! 
    @brief  enter or exit sleep (shutdown) mode. While in sleep mode
      the chip draws ~1uA
    @param state true to enter sleep mode, false to exit
*/
/**************************************************************************/
void Adafruit_VEML6070::sleep(bool state) {
  _commandRegister.bit.SD = state;

  _i2c->beginTransmission(VEML6070_ADDR_L);
  _i2c->write(_commandRegister.reg);
  _i2c->endTransmission();
}