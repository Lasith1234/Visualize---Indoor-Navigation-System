#include <Arduino.h>
#include <ESP8266WiFi.h>
#include "AudioFileSourcePROGMEM.h"
#include "AudioGeneratorWAV.h"
#include "AudioOutputI2SNoDAC.h"

//WiFI===========================================================

const char* ssid = "Dialog 4G 140";
const char* password = "";

#define WifiConnected 5

//Vibration Motor================================================

#define motorInput 14


void wifiConnect(){
  Serial.println("Connecting");
  WiFi.begin(ssid,password);

  while (WiFi.status() != WL_CONNECTED)
  {
    delay(500);
    Serial.print(".");
  }
  Serial.println();

  digitalWrite(WifiConnected,HIGH);
  Serial.println("Connection Complete");

}

void moveForward(){
  analogWrite(motorInput,225);
  delay(800);
  analogWrite(motorInput,0);
}

void turnLeft(){
  for (int i = 0; i < 1; i++)
  {
    analogWrite(motorInput,225);
    delay(100);
    analogWrite(motorInput,0);
    delay(100);
  }
  analogWrite(motorInput,225);
  delay(500);
  analogWrite(motorInput,0); 
}

void turnRight(){
  analogWrite(motorInput,225);
  delay(500);
  analogWrite(motorInput,0); 
  delay(500);
  for (int i = 0; i < 2; i++)
  {
    analogWrite(motorInput,225);
    delay(150);
    analogWrite(motorInput,0);
    delay(150);
  }

}

void moveBack(){
  for (int i = 0; i < 2; i++)
  {
    analogWrite(motorInput,225);
    delay(300);
    analogWrite(motorInput,0);
    delay(300);
  }

  for (int i = 0; i < 2; i++)
  {
    analogWrite(motorInput,225);
    delay(100);
    analogWrite(motorInput,0);
    delay(100);
  }

}

void playSpeaker(){

  AudioGeneratorWAV *wav;
  AudioFileSourcePROGMEM *file;
  AudioOutputI2SNoDAC *out;

  audioLogger = &Serial;
  file = new AudioFileSourcePROGMEM("", sizeof(""));
  out = new AudioOutputI2SNoDAC();
  wav = new AudioGeneratorWAV();
  wav -> begin(file,out);

  while (1)
  {
    if(wav -> isRunning()){
      if(!wav -> loop()){
        wav -> stop();
      }
    }else{
      break;
    }
  }
  


   
}

void setup() {
  pinMode(WifiConnected,OUTPUT);
  pinMode(motorInput,OUTPUT);

  Serial.begin(9600);

  // moveForward();
  // delay(2000);
  // turnLeft();
  // delay(2000);
  turnRight();
  // delay(2000);
  // moveBack();

}

void loop() {
  // put your main code here, to run repeatedly:
}