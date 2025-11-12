#include <LiquidCrystal_I2C.h>
#include <OneWire.h>
#include <DallasTemperature.h>

#define LED_RED 6
#define LED_GREEN 5
#define LED_BLUE 3

#define RED_BUTTON 2
#define GREEN_BUTTON 4


void initRGB() {
  pinMode(LED_BUILTIN, OUTPUT);
  digitalWrite(LED_BUILTIN, LOW);

  pinMode(LED_RED, OUTPUT);
  digitalWrite(LED_RED, LOW);

  pinMode(LED_GREEN, OUTPUT);
  digitalWrite(LED_GREEN, LOW);

  pinMode(LED_BLUE, OUTPUT);
  digitalWrite(LED_BLUE, LOW);
}

void initButtons() {
  pinMode(RED_BUTTON, INPUT_PULLUP);
  pinMode(GREEN_BUTTON, INPUT_PULLUP);
}

void setup() {
  initRGB();
  initButtons();
}

int sth_on = 0; 
int color_on = 0;
int last_read_red = HIGH;
int last_read_green = HIGH;

void loop() {
  if (digitalRead(RED_BUTTON) == LOW && last_read_red == HIGH) {
    delay(200);
    if (sth_on == 0) {
      digitalWrite(LED_RED, HIGH);
      color_on = LED_RED;
      sth_on = 1;
    } else {
      digitalWrite(LED_RED, LOW);
      digitalWrite(LED_GREEN, LOW);
      digitalWrite(LED_BLUE, LOW);
      color_on = 0;
      sth_on = 0;
    }
  }
  last_read_red = digitalRead(RED_BUTTON); 
  
  if (digitalRead(GREEN_BUTTON) == LOW && last_read_green == HIGH) {
    delay(200);
    if (color_on == LED_RED) {
      digitalWrite(LED_RED, LOW);
      digitalWrite(LED_GREEN, HIGH);
      color_on = LED_GREEN;
    } else if (color_on == LED_GREEN) {
      digitalWrite(LED_GREEN, LOW);
      digitalWrite(LED_BLUE, HIGH);
      color_on = LED_BLUE;
    } else if (color_on == LED_BLUE) {
      digitalWrite(LED_BLUE, LOW);
      digitalWrite(LED_RED, HIGH);
      color_on = LED_RED;
    }
  }
  last_read_green = digitalRead(GREEN_BUTTON);
}