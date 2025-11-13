
void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  pinMode(A0, INPUT);
  pinMode(A1, INPUT);

}

void loop() {
  // put your main code here, to run repeatedly:
  int16_t currentData1 = analogRead(A0);
  int16_t currentData2 = analogRead(A1);
  
  float reading1 = currentData1 * (5.0/1023.0) ;
  float reading2 = currentData2 * (5.0/1023.0);

 
      Serial.print("A0:");
      Serial.println(reading1);
      Serial.print("A1:");
      Serial.println(reading2);
  
  delay(1000);

}

