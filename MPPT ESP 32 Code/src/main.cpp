#include <Arduino.h>


int dutyCycle; //0 to 255
//int inputpin = 1; //GPIO1
int outputpin = 1; //GPIO
int channel = 2; //0 to 15
int freq = 50000; //
int resolution = 8; //
unsigned long timer;

void setup(){
//pinMode(inputpin, INPUT); 
  gpio_reset_pin(GPIO_NUM_1);
  pinMode(outputpin, OUTPUT);
  // configure LED PWM functionalitites
  ledcSetup(outputpin, freq, resolution);
  
  // attach the channel to the GPIO to be controlled
  ledcAttachPin(outputpin, channel);

  timer = micros();

  Serial.begin(9600);

}

void loop(){
//dutyCycle = analogRead(inputpin)*255/4096;



if((micros()-timer)>=10000)
{
  dutyCycle++;
  if(dutyCycle >= 255)
  {
    dutyCycle = 0;
  }
  ledcWrite(outputpin, dutyCycle); //
  Serial.println(dutyCycle);
  timer+=10000;
}

}
