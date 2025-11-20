//set baud rate for 115200
#include <ArduinoBLE.h>
BLEService customService("180C");  //create bLE service
//BLEIntCharacteristic valueCharacteristic("2A56", BLERead | BLENotify); //sends to phone
BLEFloatCharacteristic valueCharacteristic("2A56", BLERead | BLENotify);  //sends to phone

#define F(string_literal) (reinterpret_cast<const __FlashStringHelper *>(PSTR(string_literal)))


//making bufSize smaller to make it work (og 2000)
const int bufSize = 200;  //
//sample around like 3-4 seconds for 5Hz
//5Hz --> 5 waves every second
// 17 seconds of this is way overkill, i would be taking like 85 samples


const unsigned long sampleInterval = 4000;  // 5000 = 5 ms
float Ts = .004;                            // sample every .005 seconds (5 ms)
unsigned long lastSample = 0;


//float sensorData1[bufSize];
int16_t sensorData1[bufSize];
//float sensorData2[bufSize];
int16_t sensorData2[bufSize];
int index = 0;

struct crossCorrReturn {
  //float highr;
  int16_t highr;
  //float highshift;
  int16_t highshift;
};



void setup() {
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

  lastSample = micros();  //time since program started
}





void loop() {

  BLEDevice central = BLE.central();
  unsigned long nowSample = micros();

  if (nowSample - lastSample >= sampleInterval) {  //are we past the sampling interval
    lastSample += sampleInterval;

    int16_t currentData1 = analogRead(A0);
    int16_t currentData2 = analogRead(A1);

    sensorData1[index] = currentData1;
    sensorData2[index] = currentData2;

    //convert them to voltages
    float reading1 = currentData1 * (5.0 / 1023.0);
    float reading2 = currentData2 * (5.0 / 1023.0);

    /*
    Serial.print("A0:");
    Serial.println(reading1);
    Serial.print("A1:");
    Serial.println(reading2);
    */

    
    Serial.print(reading1);
    Serial.print(";");
    Serial.println(reading2);
    



    /* these are uncoverted voltages
    Serial.print(sensorData1[index]);
    Serial.print(";");
    //Serial.println(sensorData2[index], 3)
    Serial.println(sensorData2[index]);
   // i think this is slowing it down
   */

    index++;
    /*
    if (index % 100 == 0) { //this makes sure that it is actually getting values
      Serial.println(index);
    }
    */


    if (index >= bufSize) {

      crossCorrReturn result = crossCorr_calculate(sensorData1, sensorData2, bufSize, Ts);
      Serial.print("highshift: (milli*1000)");
      Serial.println(result.highshift);
      Serial.print("highr: (*1000)");
      Serial.println(result.highr);

      if (central.connected()) {
        float scale = (float)result.highshift / 1000;  // scale down to seconds for sending
        valueCharacteristic.writeValue(scale);
      }
      index = 0;  //am i clearing this properly -- it doesnt really clear just overwrites
      // once it reaches the buffer size, then it has to collect new data and start over again
    }
  }
}




crossCorrReturn crossCorr_calculate(int16_t x[], int16_t y[], int n, float Ts) {  //two arrays, and the buffersize

  crossCorrReturn result;

  //this is all just copied over from other cross correlation
  float mx = 0, my = 0, sx = 0, sy = 0, sxy, r;

  // Calculate mean
  for (int i = 0; i < n; i++) {
    mx += x[i];
    my += y[i];
  }
  mx /= n;
  my /= n;

  // calculate denominator
  for (int i = 0; i < n; i++) {
    sx += (x[i] - mx) * (x[i] - mx);
    sy += (y[i] - my) * (y[i] - my);
  }
  double denom = sqrt(sx * sy);

  int maxShift2 = 100;  //this makes it match the matlab
  //const float Ts = 0.01; //sample rate for 100 Hz (delay 10)
  //const float Ts = 0.005;               //sample rate for every 5 ms (200 Hz)
  //const float Ts = .015;                //sample rate for every 15ms (67 Hz)

  Serial.println("delay,correlation");  //Serial Plotter


  float highr = -1.0;
  float highshift = 0;
  // Calculate and print cross-correlation
  for (int shift = -maxShift2; shift <= maxShift2; shift++) {  //og maxShift
    sxy = 0;

    for (int i = 0; i < n; i++) {
      int j = i + shift;
      if (j < 0 || j >= n) continue;
      //if (j < 0 || j >= n){
      //sxy += (x[i] - mx) * (-my);
      //}
      else {
        sxy += (x[i] - mx) * (y[j] - my);
      }
    }

    r = sxy / denom;

    if (r > highr) {
      highr = r;
      highshift = shift * Ts;  //og just shift
    }
    float delaySec = shift * Ts;  //og not here
  }
  result.highr = (int16_t)(highr * 1000.0);        //correlation
  result.highshift = (int16_t)(highshift * 1000);  // time offset times 1000
  return result;
}
