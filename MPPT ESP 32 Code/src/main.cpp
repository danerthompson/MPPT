#include <Arduino.h>


int dutyCycle; //0 to 255
int inputpin = 1; //GPIO1
int outputpin = 0; //GPIO0
int channel = 0; //0 to 15
int freq = 75000; //I dont know the range, can at least go up to 5khz
int resolution = 8; //
int currentPin = 2; // Current sensor ADC input
float adcErr = 0;
float currentADC;

void setup(){
pinMode(inputpin, INPUT); 
  // configure LED PWM functionalitites
  ledcSetup(outputpin, freq, resolution);
  
  // attach the channel to the GPIO to be controlled
  ledcAttachPin(outputpin, channel);

  Serial.begin(9600);

  adcErr = 2.5 - float(analogRead(currentPin)*3.3/4096);

}

void loop(){
dutyCycle = analogRead(inputpin)*255/4096;
ledcWrite(outputpin, 100); //

Serial.print("Duty cycle: ");
Serial.println(dutyCycle);

currentADC = float(analogRead(currentPin)*3.3/4096)+adcErr;
  Serial.print("Current sensor");
  Serial.println(float(analogRead(currentPin)*3.3/4096)+adcErr);


  Serial.print("Current (AMPS): ");
  Serial.println((2.5-currentADC)/0.4);

}
