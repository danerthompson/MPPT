#include <Arduino.h>
#include <Adafruit_ADS1X15.h>
#include <SPI.h>
#include <SD.h>

// address is 0x49
Adafruit_ADS1015 ads;

int dutyCycle = 5; //0 to 255
int inputpin = 1; //GPIO1
int outputpin = 0; //GPIO0
int channel = 0; //0 to 15
int freq = 100000; //I dont know the range, can at least go up to 5khz
int resolution = 8; //
String filename = "/0.txt";

void setup(){
//pinMode(inputpin, INPUT); 
  gpio_reset_pin(GPIO_NUM_1);
  pinMode(outputpin, OUTPUT);
  // configure LED PWM functionalitites
  ledcSetup(outputpin, freq, resolution);
  
  // attach the channel to the GPIO to be controlled
  ledcAttachPin(outputpin, channel);

  Serial.begin(9600);

  if (!ads.begin()) {
    Serial.println("Failed to initialize ADC");
    while (1);
  }
  if (!SD.begin(SS)) {
    Serial.println("Failed to initialize SD card");
    while (1);
  }
  int filenum = 0;
  while (SD.exists(filename)) {
    filename = "/" + String(filenum) + ".txt";
    filenum++;
  }

}

void loop(){
  // Sweep the duty cycle
  dutyCycle += 5;
  if (dutyCycle > 255) {
    dutyCycle = 5;
  }
  

  ledcWrite(outputpin, dutyCycle); //
  Serial.println(dutyCycle);

  int16_t adc0, adc1, adc2, adc3;
  float volts0, volts1, volts2, volts3;
  float v_in, v_out, i_in, i_out, p_in, p_out, eff;

  adc0 = ads.readADC_SingleEnded(0);
  adc1 = ads.readADC_SingleEnded(1);
  adc2 = ads.readADC_SingleEnded(2);
  adc3 = ads.readADC_SingleEnded(3);

  volts0 = ads.computeVolts(adc0);
  volts1 = ads.computeVolts(adc1);
  volts2 = ads.computeVolts(adc2);
  volts3 = ads.computeVolts(adc3);

  v_in = volts3 * 2;
  v_out = volts1 * 2;
  i_in = (volts2 - 0.48) / 0.1;
  i_out = (2.4 - volts0) / 0.4;
  p_in = v_in * i_in;
  p_out = v_out * i_out;
  eff = p_out/p_in;

  Serial.println("-----------------------------------------------------------");
  /*
  Serial.print("AIN0: "); Serial.print(adc0); Serial.print("  "); Serial.print(volts0, 5); Serial.println("V");
  Serial.print("AIN1: "); Serial.print(adc1); Serial.print("  "); Serial.print(volts1, 5); Serial.println("V");
  Serial.print("AIN2: "); Serial.print(adc2); Serial.print("  "); Serial.print(volts2, 5); Serial.println("V");
  Serial.print("AIN3: "); Serial.print(adc3); Serial.print("  "); Serial.print(volts3, 5); Serial.println("V");
  */

  Serial.print("V_in: "); Serial.print(v_in); Serial.print("V, I_in: "); Serial.print(i_in); Serial.print("A, P_in: "); Serial.print(p_in); Serial.println("W");
  Serial.print("V_out: "); Serial.print(v_out); Serial.print("V, I_out: "); Serial.print(i_out); Serial.print("A, P_out: "); Serial.print(p_out); Serial.println("W");
  Serial.print("Efficiency: "); Serial.print(eff*100); Serial.println("%");


  File dir =  SD.open(filename, FILE_APPEND);  // Don't use FILE_WRITE unless you want to lose data
  dir.println(String(v_in) + " " + String(i_in) + " " + String(p_in) + " " + String(v_out)
              + " " + String(i_out) + " " + String(p_out) + " " + String(eff) + " " + String(dutyCycle));     // also a print() function
  dir.close();
  delay(50);
}
