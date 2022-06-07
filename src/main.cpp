#include <Arduino.h>
#include <ESP8266WiFi.h>
#include "AudioFileSourcePROGMEM.h"
#include "AudioGeneratorWAV.h"
#include "AudioOutputI2SNoDAC.h"
#include <iostream>
#include <cmath>
#include <fstream>
#include "spi_flash.h"
// #include <FS.h>
// #include <SdFat.h>
#include <SD.h>
#include <sstream>
#include <string>

using namespace std;


//WiFI===========================================================

const char* ssid = "Dialog 4G 140";
const char* password = "";

#define WifiConnected 5

//Vibration Motor================================================

#define motorInput 5

//WAV File Meta Data=============================================

// const int sampleRate = 44100;
const int sampleRate = 16000;
const int bitDepth = 16;

// Mic Input ====================================================

#define micInput A0

//===============================================================

int getAnalogSignal(){
    // int count = 0;
    //int reading;
    // while (count < 120)
    // {
    //   reading = analogRead(micInput);
    //   Serial.println(reading);
    //   delay(100);
    //   count++;
    // }
    return analogRead(micInput);
}

class sineOscillator{
  float frequency, amplitude, angle = 0.0f ,offset = 0.0;

  public:
    sineOscillator(float freq, float amp) : frequency(freq), amplitude(amp) {
      offset = 2 * PI * frequency / sampleRate;
    }
    float process(){
      auto sample = amplitude * sin(angle);
      angle += offset;
    
      return sample;
    }

};

// void writeToFile(ofstream &file, int value, int size){
//   file.write(reinterpret_cast<const char*> (&value), size);
// }

// int createWAV(){
//   int duration =5;
//   File audioFile;
//   audioFile = SPIFFS.open("waveform.wav", "w");
//   sineOscillator soc(440, 0.5);

//   //File file = SPIFFS.open

//   //Header chunk
//   audioFile.print("RIFF");
//   audioFile.print("----");
//   audioFile.print("WAVE");


//   //Format chunk
//   audioFile.print("fmt ");
  
//   writeToFile(audioFile, 16, 4);  //Size
//   writeToFile(audioFile, 1, 2);   //compression code
//   writeToFile(audioFile, 1, 2);   //Number of channels
//   writeToFile(audioFile, sampleRate, 4);    //Sample Rate
//   writeToFile(audioFile, sampleRate * bitDepth / 8, 4);  //Byte Rate
//   writeToFile(audioFile, bitDepth / 8, 2);   //Block Align
//   writeToFile(audioFile, bitDepth, 2); //Bit Depth

//   //Data Chunk
//   audioFile << "data";
//   audioFile << "----";

//   int preAudioPosition = audioFile.tellp();
//   auto maxAmplitude = pow(2, bitDepth - 1) - 1;
//   for (int i = 0; i < sampleRate * duration; i++)
//   {
//     auto sample = soc.process();
//     int intSample = static_cast<int> (sample * maxAmplitude);
//     writeToFile(audioFile, intSample, 2);
//   }

//   int postAudioPosition = audioFile.tellp();

//   audioFile.seekp(preAudioPosition - 4);
//   writeToFile(audioFile, postAudioPosition - preAudioPosition, 4);

//   audioFile.seekp(4, ios::beg);
//   writeToFile(audioFile, postAudioPosition - 8, 4);

//   audioFile.close();
//   return 0;  
// }

void writeToFile(ofstream &file, int value, int size){
  file.write(reinterpret_cast<const char*> (&value), size);
}

void createFile(){
  //File audioFile = SD.open("waveform.wav", FILE_WRITE);
  File newFile = SD.open("Testing.txt", "w");
  newFile.write("RIFF");

  
  Serial.println("Done writting");
  newFile.close();
}

void convNwrite(stringstream &ss , int value , int size){
  unsigned char a = 0;
  unsigned char b = 0;
  unsigned char c = 0;
  unsigned char d = 0;

  a |= value;
  ss << a;

  b |= (value >> 8);
  ss << b;

  if(size == 4){
    c |= (value >> 16);
    ss << c;

    d |= (value >> 24);
    ss << d;
  }
}

int CreateFile2(string data, int chunkSize, int subSzie){

  stringstream inputString;
  string finalInput;


  //Header chunk
  inputString << "RIFF";
  convNwrite(inputString, chunkSize, 4);
  inputString << "WAVE";

  //Format chunk
  inputString << "fmt ";
  convNwrite(inputString, 16, 4);
  convNwrite(inputString, 1, 2);  //compression code
  convNwrite(inputString, 1, 2);   //Number of channels
  convNwrite(inputString, sampleRate, 4);    //Sample Rate
  convNwrite(inputString, sampleRate * bitDepth / 8, 4);  //Byte Rate
  convNwrite(inputString, bitDepth / 8, 2);   //Block Align
  convNwrite(inputString, bitDepth, 2); //Bit Depth

  //Data Chunk
  inputString << "data";
  convNwrite(inputString, subSzie, 4);

  inputString << data;
  inputString >> finalInput;



}

int createWAV(){
  int duration =2;
  ofstream audioFile;
  string data;
  stringstream dataStream;

  audioFile.open("data/waveform.wav", ios::binary);

  int preAudioPosition = audioFile.tellp();
  
  for (int i = 0; i < sampleRate * duration; i++)
  {
    //auto sample = soc.process();
    auto sample = getAnalogSignal();
    convNwrite(dataStream, sample, 2);
  
  }

  dataStream >> data;

  int postAudioPosition = audioFile.tellp();
  CreateFile2(data, postAudioPosition - preAudioPosition, postAudioPosition - 8);
  audioFile.close();

  return 0;   
}

// int createWAV(){
//   int duration =2;
//   ofstream audioFile;

//   string data;
//   stringstream dataStream;

//   audioFile.open("data/waveform.wav", ios::binary);
//   sineOscillator soc(440, 0.5);

//   //File file = SPIFFS.open

//   //Header chunk
//   audioFile << "RIFF";
//   audioFile << "----";
//   audioFile << "WAVE";

//   //Format chunk
//   audioFile << "fmt ";
//   writeToFile(audioFile, 16, 4);  //Size
//   writeToFile(audioFile, 1, 2);   //compression code
//   writeToFile(audioFile, 1, 2);   //Number of channels
//   writeToFile(audioFile, sampleRate, 4);    //Sample Rate
//   writeToFile(audioFile, sampleRate * bitDepth / 8, 4);  //Byte Rate
//   writeToFile(audioFile, bitDepth / 8, 2);   //Block Align
//   writeToFile(audioFile, bitDepth, 2); //Bit Depth

//   //Data Chunk
//   audioFile << "data";
//   audioFile << "----";

//   int preAudioPosition = audioFile.tellp();
//   auto maxAmplitude = pow(2, bitDepth - 1) - 1;
//   for (int i = 0; i < sampleRate * duration; i++)
//   {
//     //auto sample = soc.process();
//     auto sample = getAnalogSignal();
//     int intSample = static_cast<int> (sample * maxAmplitude);
//     convNwrite(dataStream, intSample, 2);
//     //writeToFile(audioFile, intSample, 2);
//   }

//   dataStream >> data;

//   int postAudioPosition = audioFile.tellp();

//   audioFile.seekp(preAudioPosition - 4);
//   writeToFile(audioFile, postAudioPosition - preAudioPosition, 4);

//   audioFile.seekp(4, ios::beg);
//   writeToFile(audioFile, postAudioPosition - 8, 4);

//   audioFile.close();

//   CreateFile2(data, postAudioPosition - preAudioPosition, postAudioPosition - 8);
//   return 0;   
// }

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
  //sd.begin();

  if (!SD.begin(15))
  {
    Serial.println("SD Card Initialization Failed !");
    while(1);
  }
  
  Serial.println("Initialization Done!");
  createFile();
  //createWAV();

  // if (!SPIFFS.begin())
  //   {
  //     Serial.println("Error: mounting SPIFFS");
  //   }
  //createWAV();

  //getAnalogSignal();

  //File root = SPIFFS.open("/");
 
  //File file = root.openNextFile();
 
  // while(file){
 
  //     Serial.print("FILE: ");
  //     Serial.println(file.name());
 
  //     file = root.openNextFile();
  // }



  //Vibration Motor Sample Test

  // moveForward();
  // delay(2000);
  // turnLeft();
  // delay(2000);
  // turnRight();
  // delay(2000);
  // moveBack();

}

void loop() {
  // put your main code here, to run repeatedly:
}