/*!
 * @file Adafruit_VEML6070.h
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
 * Written by Limor Fried/Ladyada for Adafruit Industries.  
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
#include "SoftWire.h"


#define VEML6070_ADDR_H     (0x39) ///< High measurement byte address
#define VEML6070_ADDR_L     (0x38) ///< Low measurement byte & command address
#define VEML6070_ADDR_ARA   (0x0C) ///< Alert Resp Address (read to clear condition)

#define _DEBUG_                    ///< Enables serial output for troubleshooting

/**************************************************************************/
/*! 
    @brief  integration time definitions
*/
/**************************************************************************/
typedef enum veml6070_integrationtime {
  VEML6070_HALF_T,
  VEML6070_1_T,
  VEML6070_2_T,
  VEML6070_4_T,
} veml6070_integrationtime_t;


/**************************************************************************/
/*! 
    @brief  Class that stores state and functions for interacting with VEML6070 sensor IC
*/
/**************************************************************************/
template <class I2C>
class Adafruit_VEML6070 {
 public:
  Adafruit_VEML6070(I2C *i2c);

  void begin(veml6070_integrationtime_t itime);
  void setInterrupt(bool state, bool level = 0);
  bool clearAck();
  uint16_t readUV(void);
  void waitForNext(void);
  void sleep(bool state);
 private:
  void writeCommand(void);
  I2C *_i2c;

  typedef union {
    struct {
      uint8_t SD:1;
      uint8_t :1;
      uint8_t IT:2;
      uint8_t ACK_THD:1;
      uint8_t ACK:1;
    } bit;
    uint8_t reg;

  } commandRegister;

  commandRegister _commandRegister;
};
