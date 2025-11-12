#include <LiquidCrystal_I2C.h>
#include <OneWire.h>
#include <DallasTemperature.h>

#define GREEN_BUTTON 4

#define DEBOUNCE_PERIOD 10UL

LiquidCrystal_I2C lcd(0x27, 16, 2);
OneWire oneWire(A1);
DallasTemperature tempSensors(&oneWire);


enum LCDpage { curr_temps, min_max_temps };
LCDpage lcd_state = curr_temps;

const unsigned long TEMP_UPDATE_INTERVAL = 1000;
unsigned long last_temp_update = 0;

float min_tempOut;
float max_tempOut;
bool first_read_done = false;


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

void display_curr_temps() {
    tempSensors.requestTemperatures();
    float tempIn = tempSensors.getTempCByIndex(1);
    float tempOut = tempSensors.getTempCByIndex(0);

    if (!first_read_done) {
        min_tempOut = tempOut;
        max_tempOut = tempOut;
        first_read_done = true;
    }

    if (tempOut < min_tempOut) min_tempOut = tempOut;
    if (tempOut > max_tempOut) max_tempOut = tempOut;

    char buffer[40];
    sprintf(buffer, "Temp  IN: %6s", String(tempIn, 2).c_str());
    lcd.setCursor(0, 0);
    lcd.print(buffer);
    sprintf(buffer, "Temp OUT: %6s", String(tempOut, 2).c_str());
    lcd.setCursor(0, 1);
    lcd.print(buffer);
}

void display_min_max_temps() {
    lcd.clear();
    char buffer[40];
    sprintf(buffer, "MIN: %11s", String(min_tempOut, 2).c_str());
    lcd.setCursor(0, 0);
    lcd.print(buffer);
    sprintf(buffer, "MAX: %11s", String(max_tempOut, 2).c_str());
    lcd.setCursor(0, 1);
    lcd.print(buffer);
}

void setup()
{
    pinMode(GREEN_BUTTON, INPUT_PULLUP);

    tempSensors.begin();

    lcd.init();
    lcd.backlight();
    lcd.clear();

    display_curr_temps();
}

void loop()
{
    if (isGreenButtonReleased()) {
        if (lcd_state == curr_temps) {
            lcd_state = min_max_temps;
            display_min_max_temps();
        }
        else if (lcd_state == min_max_temps) {
        lcd_state = curr_temps;
            last_temp_update = millis() - TEMP_UPDATE_INTERVAL; 
        }
    }
    if (lcd_state == curr_temps && (millis() - last_temp_update >= TEMP_UPDATE_INTERVAL)) {
        last_temp_update = millis();
        display_curr_temps();
    }
}