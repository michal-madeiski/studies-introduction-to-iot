#include <LiquidCrystal_I2C.h>
#include <OneWire.h>
#include <DallasTemperature.h>

#define LED_RED 6
#define LED_GREEN 5
#define LED_BLUE 3


void change_color(int c1, int c2) {
  for (int i = 0; i <= 255; i++) {
    analogWrite(c1, 255 - i);
    analogWrite(c2, i);
    delay(10);
  }
}

void setup() {
  pinMode(LED_RED, OUTPUT);
  analogWrite(LED_RED, 255);

  pinMode(LED_GREEN, OUTPUT);
  analogWrite(LED_GREEN, 0);

  pinMode(LED_BLUE, OUTPUT);
  analogWrite(LED_BLUE, 0);
}

void loop() {
  change_color(LED_RED, LED_GREEN);
  change_color(LED_GREEN, LED_BLUE);
  change_color(LED_BLUE, LED_RED);
}