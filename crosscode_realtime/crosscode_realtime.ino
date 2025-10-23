//set baud rate for 115200

#define F(string_literal) (reinterpret_cast<const __FlashStringHelper *>(PSTR(string_literal)))


//const int bufSize = 1500; //14 seconds of data is 1500 at 10 ms (?) this is when it would stop printing before -->

const int bufSize = 2000;
float sensorData1[bufSize];
float sensorData2[bufSize];
int index = 0;

struct crossCorrReturn {
  float highr;
  float highshift;
};



void setup() {
  Serial.begin(115200);
  pinMode(A0, INPUT);
  pinMode(A1, INPUT);
}





void loop() {
  float currentData1 = analogRead(A0) * 5.000000 / 1023.000000;
  float currentData2 = analogRead(A1) * 5.0 / 1023.0;

  sensorData1[index] = currentData1;
  sensorData2[index] = currentData2;
  Serial.print(sensorData1[index],3);
  Serial.print(",");
  Serial.println(sensorData2[index],3);
  index++;
  
  /*
  if (index % 100 == 0) { //this makes sure that it is actually getting values
    Serial.println(index);
  }
  */

  //Serial.println(index);
  //Serial.println(currentData1,6);

  if (index >= bufSize) {
    /*float average = average_calculate(sensorData1, bufSize); 
    //once index reaches the full buffer, then it calculates the average and prints it out
    Serial.println(average);
    */

    crossCorrReturn result = crossCorr_calculate(sensorData1, sensorData2, bufSize);
    Serial.print("highshift: ");
    Serial.println(result.highshift,4);
    Serial.print("highr: ");
    Serial.println(result.highr,4);

    index = 0;  //am i clearing this properly -- it doesnt really clear just overwrites
    // once it reaches the buffer size, then it has to collect new data and start over again
  }
  //delay(10); //makes it roughly sample every 10 ms plus some overhead --> less than 100 Hz

  //delay(5);  //if sample every 5 ms --> 200 Hz (have to change Ts in function)
  delay(15); //sample for every 15 ms
}




crossCorrReturn crossCorr_calculate(float x[], float y[], int n) {  //two arrays, and the buffersize

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
  const float Ts = .015; //sample rate for every 15ms (67 Hz)
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
  result.highr = highr;          //correlation
  result.highshift = highshift;  // time offset
  return result;
}
