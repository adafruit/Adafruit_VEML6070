#include <Wire.h>
#include <Adafruit_VEML6070.h>

// Pin assignments
#define LED_PIN   (13)
#define POWER_PIN (12)
#define ACK_PIN   (11)

#define VEML6070_ADDR_ARA   (0x0C)      // Alert Resp Address (read to clear condition)


Adafruit_VEML6070 uv = Adafruit_VEML6070();

void setup() {
  Serial.begin(9600);
  Serial.println("VEML6070 Test to Reveal Initializiation Bug");
  pinMode(LED_PIN, OUTPUT);
  digitalWrite(LED_PIN, LOW);
  
  pinMode(POWER_PIN, OUTPUT);
  digitalWrite(POWER_PIN, LOW);
  
  digitalWrite(ACK_PIN, INPUT_PULLUP);  // ACK is open-drain

  //pinMode(SDA, INPUT);
  //pinMode(SCL, INPUT);
}

bool testAck() {
  bool ack = (digitalRead(ACK_PIN) == LOW);
  if (ack) { Serial.println("ACK is low"); }
}

void clearARA() {
    Wire.begin();
    // Read 0 bytes, sending stop command (this is a hack to fit SMBus protocol)
    if (Wire.requestFrom(VEML6070_ADDR_ARA, 0, true)) {
      //Wire.read();
    }
}

void loop() {

  // Power up and initialize the VEML 6070
  digitalWrite(POWER_PIN, HIGH);
  delay(500);  // For device to power up
  //Wire.begin();
  //Serial.println("Wire initialized");
  
  //testAck();
  //clearARA();
  //Serial.println("checked ARA");
  
  uv.begin(VEML6070_1_T);
  delay(150);   // 1_T is ~125ms; allow error margin for first measurement
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
  digitalWrite(POWER_PIN, LOW);
  //pinMode(SDA, OUTPUT);
  //digitalWrite(SDA, LOW);
  //pinMode(SCL, OUTPUT);
  //digitalWrite(SCL, LOW);
  delay(100);
  //pinMode(SDA, INPUT);
  //pinMode(SCL, INPUT);
  //delay(10);
}