
//14 seconds is 1024:2523 so bufSize = 1499

  const int bufSize = 1499;
  float sensorData[bufSize];
  int index = 0;
  int stopserial = 0;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  pinMode(A0, INPUT);

}



void loop() {


  // put your main code here, to run repeatedly:
  float currentData = analogRead(A0) * 5.0 / 1023.0;

  sensorData[index] = currentData;
  index++;


  if (index >= bufSize){
    float sum = 0;
    for (int i = 0; i < bufSize; i++){
      sum += sensorData[i];
    }
    float ave = float(sum) / bufSize;
    Serial.println(ave);
    index = 0;
  }

  
  //delay(1);
  //at 10 Hz, no delay gives lots of 0 50, 1 delay gives .33, 10 delay gives .33
}
