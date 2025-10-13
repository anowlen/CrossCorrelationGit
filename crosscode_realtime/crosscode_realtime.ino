//set baud rate for 115200

#define F(string_literal) (reinterpret_cast<const __FlashStringHelper *>(PSTR(string_literal)))


const int bufSize = 1500; //14 seconds of data is 1500
float sensorData1[bufSize];
float sensorData2[bufSize];
int index = 0;

struct crossCorrReturn{
  float highr;
  float highshift;
};



void setup() {
    Serial.begin(115200);
    pinMode(A0,INPUT);
    pinMode(A1,INPUT);

}





void loop() {
  float currentData1 = analogRead(A0)*5.000000/1023.000000;
  float currentData2 = analogRead(A1)*5.0/1023.0;

  sensorData1[index] = currentData1;
  sensorData2[index] = currentData2;
  index++;
  if (index % 100 == 0){
    Serial.println(index);
  }
  //Serial.println(index);
  //Serial.println(currentData1,6);

  if (index >= bufSize){
    /*float average = average_calculate(sensorData1, bufSize); 
    //once index reaches the full buffer, then it calculates the average and prints it out
    Serial.println(average);
    */

    crossCorrReturn result = crossCorr_calculate(sensorData1, sensorData2, bufSize);
    Serial.print("highshift: ");
    Serial.println(result.highshift);
    Serial.print("highr: ");
    Serial.println(result.highr);

    index = 0; //am i clearing this properly -- it doesnt really clear just overwrites
    // once it reaches the buffer size, then it has to collect new data and start over again
  }
  delay(10);
}




crossCorrReturn crossCorr_calculate(float x[], float y[], int n){ //two arrays, and the buffersize
  
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

    int maxShift2 = 100; //this makes it match the matlab
    const float Ts = 0.01; //sample rate
    Serial.println("delay,correlation");  //Serial Plotter


    float highr = -1.0;
    float highshift = 0;
    // Calculate and print cross-correlation
    for (int shift = -maxShift2; shift <= maxShift2; shift++) { //og maxShift
        sxy = 0;

        for (int i = 0; i < n; i++) {
            int j = i + shift;
            if (j < 0 || j >= n) continue;
            //if (j < 0 || j >= n){
              //sxy += (x[i] - mx) * (-my);
            //}
            else{
              sxy += (x[i] - mx) * (y[j] - my); 
            }
            
        }

        r = sxy / denom;

        if (r > highr){
          highr = r;
          highshift = shift *Ts; //og just shift
        }
        float delaySec = shift * Ts; //og not here

        Serial.print(delaySec, 5);
        Serial.print(',');
        Serial.println(r, 5);
        delay(5);
        
      //need to return highr, highshift

        //delay(100); 

    //result.highr = highr;
    //result.highshift = highshift;
    
    }
    result.highr = highr; //correlation
    result.highshift = highshift; // time offset
    return result;
        
}

