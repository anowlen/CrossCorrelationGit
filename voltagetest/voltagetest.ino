void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  pinMode(A0,INPUT);
  //pinMode(A1,INPUT);

}

void loop() {
  // put your main code here, to run repeatedly:
  float currentData1 = analogRead(A0)*5.0/1023.0;

  
  Serial.println(currentData1,6);
  delay(100);

}
