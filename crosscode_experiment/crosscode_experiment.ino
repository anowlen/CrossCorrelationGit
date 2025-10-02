//set baud rate for 115200

#define F(string_literal) (reinterpret_cast<const __FlashStringHelper *>(PSTR(string_literal)))
#include "arrays.h"

void setup() {
    Serial.begin(115200);
    delay(3000);
    Serial.println("testing1");

    
pinMode(A0,INPUT);
pinMode(A1,INPUT);

    float x[numValues]; //copy values from header file into array
    for (int i = 0; i < numValues; i++) {
    x[i] = array1[i];
    }
    Serial.println("testing again");

    float y[numValues];
    for (int i = 0; i < numValues; i++) {
    y[i] = array2[i];
    }

    
    int n = sizeof(x) / sizeof(x[0]); 

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

    //int maxShift = n / 2;  // Maximum shift range ->have to divide by 2
    int maxShift2 = 100; //this makes it match the matlab
    maxShift2 = n/2; //both of these work but the n/2 takes so much longer to run
    const float Ts = 0.01; //sample rate
    //Serial.println("delay,correlation");  //Serial Plotter
    //Serial.println("testing2");

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
        //Serial.print("Denom: ");
        //Serial.println(denom);

        if (r > highr){
          highr = r;
          highshift = shift *Ts; //og just shift
        }
        float delaySec = shift * Ts; //og not here
        //shift = shift * .01;
        // serial plotter show shift and cros correlation
        //Serial.print((delaySec));//unshow this on the graph, og shift
        /*
        Serial.print(shift*Ts); //same as line above, trying to save SRAM
        Serial.print(F(","));  
        Serial.println(r,3);  // Correlation coefficient (y-axis)
        */
        //Serial.println("testing1");

        ///*Commenting this out to see if it will print out other stuff
        Serial.print(delaySec, 2);
        Serial.print(F(','));
        Serial.println(r, 5);
        delay(5);
        //*/
        

        delay(100); 
    }
    Serial.print(F("highest r: "));
    Serial.println(highr,5);
    Serial.println(F("corresponding shift: "));
    Serial.println(highshift,5);
}





void loop() {
  
}
