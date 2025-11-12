#include "ButtonControl.h"

#define BUTTON1_PIN 2
#define BUTTON2_PIN 4

ButtonControl button1;
ButtonControl button2;

void onShortPress1() {
  Serial.println("Button 1: Short press");
}

void onLongPress1() {
  Serial.println("Button 1: Long press");
}

void onShortPress2() {
  Serial.println("Button 2: Short press");
}

void onLongPress2() {
  Serial.println("Button 2: Long press");
}

void setup() {
  Serial.begin(9600);

  button1.init(BUTTON1_PIN, 50, 1000);
  button1.setShortPressCallback(onShortPress1);
  button1.setLongPressCallback(onLongPress1);

  button2.init(BUTTON2_PIN, 50, 2000);
  button2.setShortPressCallback(onShortPress2);
  button2.setLongPressCallback(onLongPress2);
}

void loop() {
  button1.runMeInLoop();
  button2.runMeInLoop();
}