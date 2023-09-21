int tempSensorPin = A0; //analog pin 0 connected to temperature sensor
int lightSensorPin = A1; //analog pin 1 connected to light sensor 

void setup(){
  
  Serial.begin(9600); // tells Arduino to exchange messages with Serial Monitor at a rate of 9600 bits per second
}

void loop(){

  int lightSensorValue = analogRead(lightSensorPin); //Reads the value from the analog pin connected to the light sensor.
  
  int tempReading = analogRead(tempSensorPin); //Reads the value from the analog pin connected to the temp sensor.
  byte tempSensorValue= map(tempReading,20,358,-40,125); //Maps temp sensor value to 5V scale so readings are correct.
  
  Serial.print(tempSensorValue); //prints temp sensor value to serial monitor.
  Serial.print('T'); //prints char 'T' to serial monitor.
  Serial.print(lightSensorValue); //prints light sensor value to serial monitor.
  Serial.print('L'); //prints char 'L' to serial monitor.
  
  delay(1000); //pauses program for 1000ms.
}
