#include <LiquidCrystal_I2C.h>
#include <OneWire.h>
#include <DallasTemperature.h>

#define LED_GREEN 5

#define RED_BUTTON 2
#define GREEN_BUTTON 4


void initRGB() {
  pinMode(LED_GREEN, OUTPUT);
  analogWrite(LED_GREEN, 255);
}

void initButtons() {
  pinMode(RED_BUTTON, INPUT_PULLUP);
  pinMode(GREEN_BUTTON, INPUT_PULLUP);
}

void setup() {
  initRGB();
  initButtons();
}

int led_value = 255;

void loop() {
  if (digitalRead(RED_BUTTON) == LOW && led_value > 0) {
    delay(10);
    led_value--;
    analogWrite(LED_GREEN, led_value);
  }

  if (digitalRead(GREEN_BUTTON) == LOW && led_value < 255) {
    delay(10);
    led_value++;
    analogWrite(LED_GREEN, led_value);
  }
}