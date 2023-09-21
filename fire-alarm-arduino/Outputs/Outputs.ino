#include <LiquidCrystal.h>
#include <stdio.h>
#include <string.h>

#define potPin A0

LiquidCrystal lcd(13,12,6,5,4,3); // initialized the pins for the LCD
int greenPin = 10; //pin 10 connected to green LED.
int redPin = 9; //pin 9 connected to red LED.
int motorPin = 8; //pin 8 connected to green LED.
const int buzzerPin = 7; //pin 7 connected to piezo buzzer.

volatile bool systemOn = false; // declaring bool var that determines if system is initialised or not.

unsigned long long output_interval = 0; //declaring variables used to change interval times between outputs triggering based off potentiometer.
unsigned long time_from_start =0;
unsigned long time_previous =0;

int green_state = 0; // set initial states for outputs as 0/off
int red_state = 0;
int motor_state =0;
int buzzer_state = 0;

void setup(){
  attachInterrupt(0,checkSystem,FALLING);
  lcd.clear(); //clears LCD display
  lcd.begin(16,2);
  lcd.print("System Activated.");
  delay(1000);
  lcd.clear(); //clears LCD display
  
  pinMode(buzzerPin, OUTPUT); // Set buzzer - pin 7 as an output
  pinMode(motorPin, OUTPUT); // Set motor - pin 8 as an output
  pinMode(redPin, OUTPUT); // Set red LED - pin 9 as an output
  pinMode(greenPin, OUTPUT); // Set green LED - pin 10 as an output
  pinMode(potPin,INPUT); // Set potentiometer - analog pin 0 as an input
  
  Serial.begin(9600); // tells Arduino to exchange messages with Serial Monitor at a rate of 9600 bits per second
}


void loop()
{
  if (systemOn == true) //checks is system is initilaised
  {
    while(Serial.available()>0) //checks if there is data to read form serial monitor
    {
      String tempString = Serial.readStringUntil('T'); //reads temp value from serial monitor as a string
      int tempVal = tempString.toInt(); //converts temp string into temp int value
      Serial.println(tempVal); //prints temp int value to serial monitor
      
      String lightString = Serial.readStringUntil('L'); //reads light value from serial monitor as a string
      int lightVal = lightString.toInt(); //converts light string into temp int value
      Serial.println(lightVal); //prints light int value to serial monitor
      
      if (tempVal>= 57 && lightVal>=1000) //checks if temp value greater than 57 and light value greater than 1000
      {
        time_from_start = millis();
        output_interval=0;
        output_interval= output_interval + map(analogRead(potPin),0,1023,100,1000);
        
        if((time_from_start - time_previous) > output_interval) // for all outputs to be under one time interval
        {
          time_previous = time_from_start;
          if(red_state ==0 && buzzer_state ==0 && motor_state ==0) // condition if outputs are off
          {
            // turn the outputs back on
            red_state =1;
            buzzer_state=1;
            motor_state=1;
            green_state=0;
          }
          else
          {
            red_state =0;
            buzzer_state=0;
            motor_state=0;
            green_state=0;
          }
          
         // either on/off depending on previous states
         digitalWrite(greenPin,green_state);
         digitalWrite(redPin,red_state);
         digitalWrite(motorPin,motor_state);
         if (buzzer_state == 1){
            tone(buzzerPin,1000 ,1000); // turns piezo buzzer on
         }
          else{
            digitalWrite(buzzerPin,buzzer_state);
          }
         
         lcd.clear(); //clears LCD display
         lcd.setCursor(0,0); // points cursor to top of LCD display
         lcd.print("Fire Get Out"); // displays "Fire Get Out" on LCD display
         delay((output_interval + 500)); 
         
       }
        
     }
    
     lcd.clear(); //clears LCD display
     lcd.setCursor(0,0); // points cursor to top of LCD display
     lcd.print("Temperature"); // displays "Temperature" on LCD display
     lcd.setCursor(0,1);
     lcd.print(tempVal); // displays temp reading on LCD display
     delay(1000);
     lcd.clear(); //clears LCD display
     lcd.setCursor(0,0);
     lcd.print("Light"); // displays "Light" on LCD display
     lcd.setCursor(0,1);
     lcd.print(lightVal); // displays light reading on LCD display
     delay(1000);
      
    }
  }
  else 
  {
     digitalWrite(greenPin,HIGH); // turns green LED on
  }
}


void checkSystem(){ // function to flip state of boolean var systemOn, to control whether system is on or off.
  if (systemOn == true){
    systemOn = false;
  }
  else{
    systemOn = true;
  }
}
