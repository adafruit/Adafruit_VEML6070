// These unit tests require the AUnit unit testing library,
// available within the Arduino IDE. Note that old versions of
// the Arduino IDE, including 1.6.5, may not use a new enough
// C++ compiler for nullptr to be defined.



#include <Wire.h>
#include <Adafruit_VEML6070.h>
#include <AUnitVerbose.h>

// Pin assignments
#define LED_PIN   (13)
#define POWER_PIN (12)
#define ACK_PIN   (11)

// Globals
Adafruit_VEML6070 uv = Adafruit_VEML6070();


bool reset_state() {
  //while(uv.clearAck()) { Serial.println("ACK was set"); }
  
  pinMode(LED_PIN, OUTPUT);
  digitalWrite(LED_PIN, LOW);

  pinMode(POWER_PIN, OUTPUT);
  digitalWrite(POWER_PIN, LOW);

  pinMode(ACK_PIN, INPUT);
  digitalWrite(ACK_PIN, INPUT_PULLUP);

  delay(100);

  // Verify I2C bus is in free state
  return (digitalRead(SDA) == HIGH) && (digitalRead(SCL) == HIGH);
}

  
test(0_hello) {
  assertEqual(1, 1);
}


test(1_initialize_and_read) {
  assertTrue(reset_state());

  digitalWrite(POWER_PIN, HIGH);
  delay(10);

  uv.begin(VEML6070_1_T);  // pass in the integration time constant

  for (uint8_t i = 0; i < 10; i++) {
    uint16_t value = uv.readUV();
    assertNotEqual(value, (uint16_t) 0xFFFF);
    assertNotEqual(value, (uint16_t) 0);
  }
}


test(2_read_with_power_cycles) {
  for (uint16_t i = 0; i < 10; i++) {
    assertTrue(reset_state());
    digitalWrite(POWER_PIN, HIGH);
    delay(10);
    uv.begin(VEML6070_1_T);  // pass in the integration time constant

    uint16_t value = uv.readUV();
    assertNotEqual(value, (uint16_t) 0xFFFF);
    assertNotEqual(value, (uint16_t) 0);
  }
}


test(3_set_interrupt) {
  assertTrue(reset_state());
  digitalWrite(POWER_PIN, HIGH);
  delay(10);
  
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

  if (!state) { skip(); } // Don't mark as success if we didn't trigger interrupt

  uv.clearAck();
  uv.setInterrupt(false);
}


test(4_show_random_failure) {
  assertTrue(reset_state());

  digitalWrite(POWER_PIN, HIGH);
  delay(10);
  uv.begin(VEML6070_4_T);
  uv.setInterrupt(true, 0);

  while(uv.readUV() != 0xFFFF);   // Wait for interrupt (ensure bright sunlight!)

  // Power cycle & cold start
  assertTrue(reset_state());
  digitalWrite(POWER_PIN, HIGH);
  delay(10);
  uv.begin(VEML6070_4_T);

  // This will fail, because with the interrupt triggered the device won't respond
  // to I2C reads of the result address.
  assertNotEqual(uv.readUV(), (uint16_t) 65535);
}


void setup() {
  delay(1000); // wait for stability on some boards to prevent garbage Serial
  
  Serial.begin(115200); // **WARNING** for unknown reasons, 9600 results in I2C bus lockup
  
  while(!Serial); // for the Arduino Leonardo/Micro only
}

void loop() {
  // Should get:
  // TestRunner summary:
  //    1 passed, 1 failed, 0 skipped, 0 timed out, out of 2 test(s).
  aunit::TestRunner::run();
}