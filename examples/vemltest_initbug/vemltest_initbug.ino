#include <Wire.h>
#include <Adafruit_VEML6070.h>

// Pin assignments
#define POWER_PIN (11)    
#define ACK_PIN   (13)  // Note blue LED will turn *off* if ACK is set

Adafruit_VEML6070<TwoWire> uv = Adafruit_VEML6070<TwoWire>();

void setup() {
  Serial.begin(115200);
  Serial.println("VEML6070 Test to Reveal Bug");
  
  pinMode(POWER_PIN, OUTPUT);
  digitalWrite(POWER_PIN, LOW);
  
  digitalWrite(ACK_PIN, INPUT_PULLUP);  // ACK is open-drain

}

bool testAck() {
  bool ack = (digitalRead(ACK_PIN) == LOW);
  if (ack) { Serial.println("ACK is set (low)"); Serial.flush(); }
}

bool i2c_ready(){
  bool ready = (digitalRead(SDA) == HIGH) && (digitalRead(SCL) == HIGH);
  if (!ready) { Serial.println("I2C bus locked"); Serial.flush(); }
  return ready;
}

void loop() {
  // Power DOWN the VEML 6070, and drive the I2C bus
  // to ground to ensure the sensor is actually power-cycled
  digitalWrite(POWER_PIN, LOW);
  pinMode(SDA, OUTPUT);
  digitalWrite(SDA, LOW);
  pinMode(SCL, OUTPUT);
  digitalWrite(SCL, LOW);
  pinMode(SDA, INPUT);
  pinMode(SCL, INPUT);
  delay(100);
  
  
  // Power up and initialize the VEML 6070
  digitalWrite(POWER_PIN, HIGH);
  Wire.begin();
  delay(1000);  // For device to power up

  i2c_ready();
  testAck();
  //if (uv.clearAck()) { Serial.println("Ack was set"); Serial.flush(); }
  i2c_ready();

  Serial.print("init.. "); Serial.flush();
  uv.begin(VEML6070_HALF_T);
  Serial.println("done");


  // Attempt to read the UV value after allowing for measurement delay
  Serial.print("UV: ");
  uint16_t value = uv.readUV();
  Serial.println(value);
}

