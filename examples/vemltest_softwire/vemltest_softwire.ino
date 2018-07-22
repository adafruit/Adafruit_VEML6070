// This variant on the test example shows how to use
// the SoftWire library instead of the Wire library. 

// Some extra initialization is required, including 
// creating an external 1-byte i2c buffer; it can be
// just 1 byte, and shared between TX and RX, because
// we're really using the SMBus protocol and only ever
// reading or writing 1 byte in atomic operations.

// The benefit of using the SoftWire library is that it
// uses timeouts so it will never hang your code, and 
// it does not experience bus lockups if the VEML6070's
// power is cycled. See unittests for example where 
// this is needed.

#include <SoftWire.h>
#include "Adafruit_VEML6070.h"


#define BUFFER_LENGTH   1
uint8_t i2cBuf[BUFFER_LENGTH] = {0};
SoftWire i2c(SDA, SCL);
Adafruit_VEML6070<SoftWire> uv = Adafruit_VEML6070<SoftWire>(&i2c);


void setup() {
  Serial.begin(9600);
  Serial.println("VEML6070 Test");

  i2c.setRxBuffer(i2cBuf, BUFFER_LENGTH);
  i2c.setTxBuffer(i2cBuf, BUFFER_LENGTH);
  i2c.setTimeout_ms(10);
  
  uv.begin(VEML6070_1_T);  // pass in the integration time constant
}


void loop() {
  Serial.print("UV light level: "); Serial.println(uv.readUV());
  
  delay(1000);
}
