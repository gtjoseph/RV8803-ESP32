/*
  Fine Tune the Crystal Oscillator in the RV-8803
  By: Andy England
  SparkFun Electronics
  Date: 3/2/2020
  License: This code is public domain but you buy me a beer if you use this and we meet someday (Beerware license).

  Feel like supporting our work? Buy a board from SparkFun!
  https://www.sparkfun.com/products/14642

  This example shows how to calibrate the RTC's oscillator to have it keep even more accurate time

  Hardware Connections:
    Attach the Qwiic Shield to your Arduino/Photon/ESP32 or other
    Plug the RTC into the shield (any port)
    Open the serial monitor at 115200 baud
*/

#include <SparkFun_RV8803.h>

RV8803 rtc;

void setup() {

  Wire.begin();

  Serial.begin(115200);
  Serial.println("Alarm from RTC Example");

  if (rtc.begin() == false) {
    Serial.println("Something went wrong, check wiring");
  }
  else
  {
    Serial.println("RTC online!");
  }
  rtc.disableAllInterrupts();
  rtc.setCalibrationOffset(0); //Zero out any calibration settings we may have
  rtc.setPeriodicTimeUpdateFrequency(CLOCK_OUT_FREQUENCY_1_HZ); //Set our clockout to a 1 Hz square wave,
  rtc.enableHardwareInterrupt(UPDATE_INTERRUPT); //Enable the interrupt
  //We now must measure the frequency on the Clock Out carefully to calibrate our crystal. To start generating a signal on Clock Out, tie CLKOE High.
  //Change measuredFrequency accordingly, note that you can only correct +/-7.6288 ppm
  float measuredFrequency = 1.0000012; //Measured frequency in Hz (CHANGE THIS TO YOUR MEASURED VALUE)
  float newPPM = (measuredFrequency - 1) * 1000000; //Calculate PPM difference between measuredFrequency and our desired 1 Hz wave
  //rtc.setCalibrationOffset(newPPM); //Uncomment this line after you have changed the value of measuredFrequency to load the new calibration into the RTC
  
}

void loop() {
  
}