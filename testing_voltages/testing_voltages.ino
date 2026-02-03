
#include <ArduinoBLE.h>
BLEService customService("180C");  //create bLE service
//BLEIntCharacteristic valueCharacteristic("2A56", BLERead | BLENotify); //sends to phone
BLEFloatCharacteristic valueCharacteristic("2A56", BLERead | BLENotify);  //sends to phone

#define F(string_literal) (reinterpret_cast<const __FlashStringHelper *>(PSTR(string_literal)))




void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  pinMode(A0, INPUT);
  pinMode(A1, INPUT);

  //BLE stuff
  if (!BLE.begin()) {
    Serial.println("Starting BLE failed!");
    while (1)
      ;
  }

  BLE.setLocalName("Arduino_R4_BLE");
  BLE.setAdvertisedService(customService);
  customService.addCharacteristic(valueCharacteristic);
  BLE.addService(customService);
  valueCharacteristic.writeValue(0);  //initial value
  BLE.advertise();
  Serial.println("BLE device active, waiting for connections...");

  //lastSample = micros();  //time since program started

}

void loop() {
  BLEDevice central = BLE.central();
  unsigned long nowSample = micros();
  // put your main code here, to run repeatedly:
  int16_t currentData1 = analogRead(A0);
  int16_t currentData2 = analogRead(A1);
  
  float reading1 = currentData1 * (5.0/1023.0) ;
  float reading2 = currentData2 * (5.0/1023.0);

 
      Serial.print("A0:");
      Serial.println(reading1);
      Serial.print("A1:");
      Serial.println(reading2);
  
  delay(5);

  if (central.connected()) {
        //float scale = (float)result.highshift / 1000;  // scale down to seconds for sending
        //valueCharacteristic.writeValue(scale); commenting this out to just send voltages read
        valueCharacteristic.writeValue(float(reading1)); //ruitong verification
        valueCharacteristic.writeValue(float(reading2));
        Serial.print(reading1);
      Serial.print(";");
      Serial.println(reading2);
  }
}

