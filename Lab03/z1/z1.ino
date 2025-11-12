#include <LiquidCrystal_I2C.h>
#include <OneWire.h>
#include <DallasTemperature.h>

#define LED_RED 6

#define RED_BUTTON 2
#define GREEN_BUTTON 4

#define DEBOUNCE_PERIOD 10UL

LiquidCrystal_I2C lcd(0x27, 16, 2);


void initRGB()
{
    pinMode(LED_BUILTIN, OUTPUT);
    digitalWrite(LED_BUILTIN, LOW);

    pinMode(LED_RED, OUTPUT);
    digitalWrite(LED_RED, LOW);
}

void initButtons()
{
    pinMode(RED_BUTTON, INPUT_PULLUP);
    pinMode(GREEN_BUTTON, INPUT_PULLUP);
}

void printCounter(int count)
{
    lcd.setCursor(0, 0);
    lcd.print("Counter Value:");
    lcd.setCursor(0, 1);
    lcd.print("                ");
    lcd.setCursor(0, 1);
    lcd.print(count);
    Serial.print("Counter value: ");
    Serial.println(count);
}

void printDisplayMsg(int blinks)
{   
    lcd.setCursor(0, 0);
    lcd.print("                ");
    lcd.setCursor(0, 0);
    lcd.print("DISPLAYING...");
    lcd.setCursor(0, 1);
    lcd.print("                ");
    lcd.setCursor(0, 1);
    lcd.print("counter: ");
    lcd.print(blinks);
    Serial.print("DISPLAYING... counter value: ");
    Serial.println(blinks);
}

void displayCounterOnRedLed(int count)
{
    const unsigned long BlinkChangePeriod = 1000UL;
    static bool isBlinking = false;
    static int targetBlinks = 0;
    static int currentBlinks = 0;
    static int ledState = LOW;
    static unsigned long lastBlinkChange = 0UL;

    if (count > 0 && !isBlinking)
    {
        isBlinking = true;
        targetBlinks = count;
        currentBlinks = 0;
        ledState = HIGH;
        digitalWrite(LED_RED, ledState);
        lastBlinkChange = millis();
    }

    if (!isBlinking)
    {
        return;
    }

    unsigned long currentTime = millis();
    if (currentTime - lastBlinkChange >= BlinkChangePeriod)
    {
        if (ledState == HIGH)
        {
            ledState = LOW;
            currentBlinks++;
        }
        else
        {
            ledState = HIGH;
        }
        digitalWrite(LED_RED, ledState);
        lastBlinkChange += BlinkChangePeriod;

        if (currentBlinks >= targetBlinks)
        {
            isBlinking = false;
            digitalWrite(LED_RED, LOW);
        }
    }
}

bool isGreenButtonReleased()
{
    static int debounced_button_state = HIGH;
    static int previous_reading = HIGH;
    static unsigned long last_change_time = 0UL;
    bool isReleased = false;

    int current_reading = digitalRead(GREEN_BUTTON);

    if (previous_reading != current_reading)
    {
        last_change_time = millis();
    }

    if (millis() - last_change_time > DEBOUNCE_PERIOD)
    {
        if (current_reading != debounced_button_state)
        {
            if (debounced_button_state == LOW && current_reading == HIGH)
            {
                isReleased = true;
            }
            debounced_button_state = current_reading;
        }
    }

    previous_reading = current_reading;

    return isReleased;
}

bool isRedButtonReleased()
{
    static int debounced_button_state = HIGH;
    static int previous_reading = HIGH;
    static unsigned long last_change_time = 0UL;
    bool isReleased = false;

    int current_reading = digitalRead(RED_BUTTON);

    if (previous_reading != current_reading)
    {
        last_change_time = millis();
    }

    if (millis() - last_change_time > DEBOUNCE_PERIOD)
    {
        if (current_reading != debounced_button_state)
        {
            if (debounced_button_state == LOW && current_reading == HIGH)
            {
                isReleased = true;
            }
            debounced_button_state = current_reading;
        }
    }

    previous_reading = current_reading;

    return isReleased;
}


void setup()
{
    Serial.begin(9600);
    lcd.init();
    lcd.backlight();
    initRGB();
    initButtons();
}

static int counter = 0;
void loop()
{
    bool greenReleased = isGreenButtonReleased();
    bool redReleased = isRedButtonReleased();
    static bool bothPressedPrevious = false;
    bool bothPressed = (digitalRead(RED_BUTTON) == LOW && digitalRead(GREEN_BUTTON) == LOW);

    int blinks = 0;

    if (bothPressed && !bothPressedPrevious)
    {
        blinks = counter;
        printDisplayMsg(blinks);
    }

    else if (greenReleased)
    {
        counter++;
        printCounter(counter);
    }

    else if (redReleased)
    {
        counter--;
        printCounter(counter);
    }

    bothPressedPrevious = bothPressed;
    displayCounterOnRedLed(blinks);
}