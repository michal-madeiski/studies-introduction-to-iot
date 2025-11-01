#include <LiquidCrystal_I2C.h>
#include <OneWire.h>
#include <DallasTemperature.h>


void setup() {
  pinMode(LED_BUILTIN, OUTPUT);
}

void loop() {
  digitalWrite(LED_BUILTIN, HIGH); // turn the LED on (HIGH is the voltage level)
  delay(3000);                     // wait
  digitalWrite(LED_BUILTIN, LOW);  // turn the LED off by making the voltage LOW
  delay(3000);                     // wait
}