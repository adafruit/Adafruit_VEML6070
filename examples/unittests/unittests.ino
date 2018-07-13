// These unit tests require the AUnit unit testing library,
// available within the Arduino IDE. Note that old versions of
// the Arduino IDE, including 1.6.5, may not use a new enough
// C++ compiler for nullptr to be defined.



#include <Wire.h>
#include <Adafruit_VEML6070.h>
#include <AUnitVerbose.h>

// Pin assignments
#define POWER_PIN (13)
#define ACK_PIN   (11)

// Globals
Adafruit_VEML6070 uv = Adafruit_VEML6070();


  
test(0_hello) {
  assertEqual(1, 1);
}


test(1_initialize_and_read) {

  uv.begin(VEML6070_1_T);  // pass in the integration time constant

  for (uint8_t i = 0; i < 10; i++) {
    uint16_t value = uv.readUV();
    Serial.print("UV: ");
    Serial.println(value);
    assertNotEqual(value, (uint16_t) 0xFFFF);
    assertNotEqual(value, (uint16_t) 0);
  }
}

test(2_set_interrupt) {
  Serial.println("UV measurement must exceed 102 for this test to be informative.");
  Serial.println("Interrupt trigger will result in values of 65535.");
  uv.setInterrupt(true, 0);
  uv.begin(VEML6070_4_T);

  uint16_t val = 0;

  // Note that once the interrupt is tripped,
  // values will read as 65535 because the
  // device becomes unresponsive to I2C reads
  for (uint16_t i = 0; i < 10; i++) {
    val = uv.readUV();
    Serial.print("UV: "); 
    Serial.println(val);
  }  
  delay(500);


  bool pin = digitalRead(ACK_PIN);
  bool state = uv.clearAck();
  
  Serial.print("ACK ");
  Serial.println(state ? "was set (as desired)." : "not set, skipping");

  assertTrue(pin ^ state);  // If ACK is in triggered state, pin should be low

  uv.clearAck();
  uv.setInterrupt(false);
  uv.clearAck();
  
  if (!state) { skip(); } // Don't mark as success if we didn't trigger interrupt
}





void setup() {
  delay(1000); // wait for stability on some boards to prevent garbage Serial
  
  Serial.begin(115200); // **WARNING** for unknown reasons, 9600 results in I2C bus lockup
  
  while(!Serial); // for the Arduino Leonardo/Micro only

  pinMode(POWER_PIN, OUTPUT);
  digitalWrite(POWER_PIN, HIGH);
  delay(100);
}

void loop() {
  // Should get:
  // TestRunner summary:
  //    1 passed, 1 failed, 0 skipped, 0 timed out, out of 2 test(s).
  aunit::TestRunner::run();
}