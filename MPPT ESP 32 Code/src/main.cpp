#include <Arduino.h>


int dutyCycle; //0 to 255
int inputpin = 1; //GPIO1
int outputpin = 0; //GPIO0
int channel = 0; //0 to 15
int freq = 5000; //I dont know the range, can at least go up to 5khz
int resolution = 8; //

void setup(){
pinMode(inputpin, INPUT); 
  // configure LED PWM functionalitites
  ledcSetup(outputpin, freq, resolution);
  
  // attach the channel to the GPIO to be controlled
  ledcAttachPin(outputpin, channel);

}

void loop(){
dutyCycle = analogRead(inputpin)*255/4096;
ledcWrite(outputpin, dutyCycle); //

}
