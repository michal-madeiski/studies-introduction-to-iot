#include <LiquidCrystal_I2C.h>
#include <OneWire.h>
#include <DallasTemperature.h>

#define LED_RED 6
#define LED_GREEN 5
#define LED_BLUE 3

#define RED_BUTTON 2
#define GREEN_BUTTON 4

#define DEBOUNCE_PERIOD 10UL

unsigned long blinkRedLedPeriod = 900UL;
int blinkRedLedChangeDirection = 1;
unsigned long blinkGreenLedPeriod = 1000UL;
int blinkGreenLedChangeDirection = 1;
unsigned long blinkBlueLedPeriod = 1100UL;
int blinkBlueLedChangeDirection = 1;

LiquidCrystal_I2C lcd(0x27, 16, 2);


void initRGB()
{
    pinMode(LED_BUILTIN, OUTPUT);
    digitalWrite(LED_BUILTIN, LOW);

    pinMode(LED_RED, OUTPUT);
    digitalWrite(LED_RED, LOW);

    pinMode(LED_GREEN, OUTPUT);
    digitalWrite(LED_GREEN, LOW);

    pinMode(LED_BLUE, OUTPUT);
    digitalWrite(LED_BLUE, LOW);
}

void initButtons()
{
    pinMode(RED_BUTTON, INPUT_PULLUP);
    pinMode(GREEN_BUTTON, INPUT_PULLUP);
}

bool isGreenButtonPressed()
{
    static int debounced_button_state = HIGH;
    static int previous_reading = HIGH;
    static unsigned long last_change_time = 0UL;
    bool isPressed = false;

    int current_reading = digitalRead(GREEN_BUTTON);

    if (previous_reading != current_reading)
    {
        last_change_time = millis();
    }

    if (millis() - last_change_time > DEBOUNCE_PERIOD)
    {
        if (current_reading != debounced_button_state)
        {
            if (debounced_button_state == HIGH && current_reading == LOW)
            {
                isPressed = true;
            }
            debounced_button_state = current_reading;
        }
    }

    previous_reading = current_reading;

    return isPressed;
}

bool isRedButtonPressed()
{
    static int debounced_button_state = HIGH;
    static int previous_reading = HIGH;
    static unsigned long last_change_time = 0UL;
    bool isPressed = false;

    int current_reading = digitalRead(RED_BUTTON);

    if (previous_reading != current_reading)
    {
        last_change_time = millis();
    }

    if (millis() - last_change_time > DEBOUNCE_PERIOD)
    {
        if (current_reading != debounced_button_state)
        {
            if (debounced_button_state == HIGH && current_reading == LOW)
            {
                isPressed = true;
            }
            debounced_button_state = current_reading;
        }
    }

    previous_reading = current_reading;

    return isPressed;
}

void blinkRedLed()
{
    const unsigned long BlinkChangePeriod = blinkRedLedPeriod;
    static int ledState = LOW;
    static unsigned long lastBlinkChange = 0UL;

    if (millis() - lastBlinkChange >= BlinkChangePeriod)
    {
        if (ledState == HIGH)
        {
            ledState = LOW;
        }
        else
        {
            ledState = HIGH;
        }

        digitalWrite(LED_RED, ledState);
        lastBlinkChange += BlinkChangePeriod;
    }
}

void blinkGreenLed()
{
    const unsigned long BlinkChangePeriod = blinkGreenLedPeriod;
    static int ledState = LOW;
    static unsigned long lastBlinkChange = 0UL;

    if (millis() - lastBlinkChange >= BlinkChangePeriod)
    {
        if (ledState == HIGH)
        {
            ledState = LOW;
        }
        else
        {
            ledState = HIGH;
        }

        digitalWrite(LED_GREEN, ledState);
        lastBlinkChange += BlinkChangePeriod;
    }
}

void blinkBlueLed()
{
    const unsigned long BlinkChangePeriod = blinkBlueLedPeriod;
    static int ledState = LOW;
    static unsigned long lastBlinkChange = 0UL;

    if (millis() - lastBlinkChange >= BlinkChangePeriod)
    {
        if (ledState == HIGH)
        {
            ledState = LOW;
        }
        else
        {
            ledState = HIGH;
        }

        digitalWrite(LED_BLUE, ledState);
        lastBlinkChange += BlinkChangePeriod;
    }
}

void changeBlinkPeriod(int index) {
    if (index == 0) {
        blinkRedLedPeriod += 100UL;
        if (blinkRedLedPeriod > 2000UL) {
            blinkRedLedPeriod = 500UL;
        }
    }
    else if (index == 1) {
        blinkGreenLedPeriod += 100UL;
        if (blinkGreenLedPeriod > 2000UL) {
            blinkGreenLedPeriod = 500UL;
        }
    }
    else if (index == 2) {
        blinkBlueLedPeriod += 100UL;
        if (blinkBlueLedPeriod > 2000UL) {
            blinkBlueLedPeriod = 500UL;
        }
    }
}

void printLedColorAndBlinkPeriod(int index) {
    lcd.clear();
    lcd.setCursor(0, 0);
    
    if (index == 0) {
        lcd.print("RED:");
        lcd.setCursor(0, 1);
        lcd.print(blinkRedLedPeriod);
        lcd.print("ms");
    }
    else if (index == 1) {
        lcd.print("GREEN:");
        lcd.setCursor(0, 1);
        lcd.print(blinkGreenLedPeriod);
        lcd.print("ms");
    }
    else if (index == 2) {
        lcd.print("BLUE:");
        lcd.setCursor(0, 1);
        lcd.print(blinkBlueLedPeriod);
        lcd.print("ms");
    }
}

void setup()
{
    lcd.init();
    lcd.backlight();
    lcd.print("RED:");
    lcd.setCursor(0, 1);
    lcd.print(blinkRedLedPeriod);
    lcd.print("ms");
    initRGB();
    initButtons();
}

int setLedIndex = 0;
void loop()
{
  blinkRedLed();
  blinkGreenLed();
  blinkBlueLed();

  if (isGreenButtonPressed()) {
      setLedIndex += 1;
      printLedColorAndBlinkPeriod(setLedIndex % 3);
  }

  if (isRedButtonPressed()) {
      int changeLedIndex = setLedIndex % 3;
      changeBlinkPeriod(changeLedIndex);
      printLedColorAndBlinkPeriod(setLedIndex % 3);
  }
}