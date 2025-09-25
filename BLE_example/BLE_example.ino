#include <ArduinoBLE.h>

// Create a BLE Service
BLEService customService("180C");  // create UUID

// Create a BLE Characteristic (read + notify)
BLEIntCharacteristic valueCharacteristic("2A56",  // UUID
                                         BLERead | BLENotify); // phone can read value w BLERead // arduino automatically pushes to phone w BLENotify

int counter = 0;

void setup() {
  Serial.begin(115200);

  if (!BLE.begin()) {
    Serial.println("Starting BLE failed!");
    while (1);
  }

  // Set device name
  BLE.setLocalName("UNO_R4_BLE");
  BLE.setAdvertisedService(customService);

  // Add characteristic to service
  customService.addCharacteristic(valueCharacteristic);

  // Add service
  BLE.addService(customService);

  // Initial value
  valueCharacteristic.writeValue(counter);

  // Start advertising
  BLE.advertise();

  Serial.println("BLE device active, waiting for connections...");
}

void loop() {
  // Wait for central device to connect
  BLEDevice central = BLE.central();

  if (central) {
    Serial.print("Connected to: ");
    Serial.println(central.address());

    while (central.connected()) {
      counter++;
      valueCharacteristic.writeValue(counter);

      Serial.print("Sent value: ");
      Serial.println(counter);

      delay(1000); // send every second
    }

    Serial.print("Disconnected from: ");
    Serial.println(central.address());
  }
}
