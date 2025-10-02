//14 seconds is 1024:2523 so bufSize = 1499

  const int bufSize = 1499;
  float sensorData1[bufSize];
  float sensorData2[bufSize];
  
  int index = 0;
  int stopserial = 0;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  pinMode(A0, INPUT);
  pinMode(A1, INPUT);

}



void loop() {


  // put your main code here, to run repeatedly:
  float currentData = analogRead(A0) * 5.0 / 1023.0;
  float currentData2 = analogRead(A1)*5.0/ 1023.0;

  sensorData1[index] = currentData;
  sensorData2[index] = currentData2;
  index++;


  if (index >= bufSize){
    float sum = 0;
    float sum2 = 0;
    for (int i = 0; i < bufSize; i++){
      sum += sensorData1[i];
      sum2 += sensorData2[i];
    }
    float ave = float(sum) / bufSize;
    float ave2 = float(sum2) / bufSize;
    Serial.print("A0: ");
    Serial.print(ave);
    Serial.print(", A1: ");
    Serial.println(ave2);
    index = 0;
  }

  
  //delay(1);
  //at 10 Hz, no delay gives lots of 0 50, 1 delay gives .33, 10 delay gives .33
}
