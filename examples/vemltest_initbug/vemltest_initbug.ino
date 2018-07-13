#include <Wire.h>
#include <Adafruit_VEML6070.h>

// Pin assignments
#define POWER_PIN (13)    // Note blue LED also here
#define ACK_PIN   (11)

#define VEML6070_ADDR_ARA   (0x0C)      // Alert Resp Address (read to clear condition)


Adafruit_VEML6070 uv = Adafruit_VEML6070();

void setup() {
  Serial.begin(115200);
  Serial.println("VEML6070 Test to Reveal Initializiation Bug");
  
  pinMode(POWER_PIN, OUTPUT);
  digitalWrite(POWER_PIN, LOW);
  
  digitalWrite(ACK_PIN, INPUT_PULLUP);  // ACK is open-drain

  delay(1000);
  //pinMode(SDA, INPUT);
  //pinMode(SCL, INPUT);
}

bool testAck() {
  bool ack = (digitalRead(ACK_PIN) == LOW);
  if (ack) { Serial.println("ACK is low"); }
}

bool i2c_ready(){
  return (digitalRead(SDA) == HIGH) && (digitalRead(SCL) == HIGH);
}

void loop() {

  // Power up and initialize the VEML 6070
  digitalWrite(POWER_PIN, HIGH);
  delay(100);  // For device to power up
  //Wire.begin();
  //Serial.println("Wire initialized");
  
  testAck();
  if (!i2c_ready()) { Serial.println("I2C bus locked"); }
  //clearARA();
  Serial.println("initializing");
  if (uv.clearAck()) { Serial.println("Ack was set"); } else { Serial.println("Ack clear"); }
  uv.begin(VEML6070_HALF_T);
  Serial.println("UV initialized");


  // Attempt to read the UV value after allowing for measurement delay

  Serial.print("UV: ");
  uint16_t value = uv.readUV();
  Serial.println(value);
  
  if (value == 0xFFFF) { 
    Serial.println("Error; trapping execution.");
    while(1); 
  }  

  // Power down the UV sensor, and drive the I2C bus
  // to ground to ensure the sensor is actually power-cycled
  //digitalWrite(POWER_PIN, LOW);
  //pinMode(SDA, OUTPUT);
  //digitalWrite(SDA, LOW);
  //pinMode(SCL, OUTPUT);
  //digitalWrite(SCL, LOW);
  //delay(100);
  //pinMode(SDA, INPUT);
  //pinMode(SCL, INPUT);
  //delay(10);
}