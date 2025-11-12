#include <util/atomic.h>
#include <LiquidCrystal_I2C.h>
#include <OneWire.h>
#include <DallasTemperature.h>

#define LED_RED 6
#define LED_GREEN 5
#define LED_BLUE 3

#define RED_BUTTON 2
#define GREEN_BUTTON 4

#define ENCODER1 A2
#define ENCODER2 A3

#define DEBOUNCING_PERIOD 100
#define DEBOUNCE_PERIOD 10UL

LiquidCrystal_I2C lcd(0x27, 16, 2);


enum MenuLevel { main_menu, sub_menu };
enum MenuGroup { led_options, display, temperature, about };

volatile bool mainMenu = true;
volatile int encoder1 = HIGH;
volatile int encoder2 = HIGH;
volatile unsigned long encoderTimestamp = 0UL;

int main_menu_idx = 0;
int sub_menu_idx = 0;
MenuLevel curr_lvl = main_menu;
MenuGroup curr_gr; 
char selector = '>';
bool led_power = false;
bool lcd_backlight = true;
int red_intensity = 0;
int green_intensity = 0;
int blue_intensity = 0;
bool temp_is_celsius = true;
int temp_C = 20;
int temp_F = 68;
bool displaying = false;
bool intenisty_changing = false;
bool selector_is_custom = false;

String lcd_backlight_state = "ON ";
String led_power_state = "OFF";
String temp_state = "C";

byte arrow_right[8] = {
  0b00100,
  0b00110,
  0b11111,
  0b11111,
  0b00110,
  0b00100,
  0b00000,
  0b00000
};


ISR(PCINT1_vect)
{
    encoder1 = digitalRead(ENCODER1);
    encoder2 = digitalRead(ENCODER2);
    encoderTimestamp = millis();
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

void display_menu_helper(int selector_row, String first_option, String second_option) {
    int first_cursor = 0;
    int second_cursor = 0;
    if (selector_row == 0) {
        first_cursor = 1;
        lcd.setCursor(0, 0);
    } else if (selector_row == 1) {
        second_cursor = 1;
        lcd.setCursor(0, 1);
    }

    if (selector == '*') {
        lcd.write(byte(0));
    } else {
        lcd.print(selector);
    }

    lcd.setCursor(first_cursor, 0);
    lcd.print(first_option);
    lcd.setCursor(second_cursor, 1);
    lcd.print(second_option);

    if (second_option == "Selector: ") {
        lcd.setCursor(11, 1);
        if (selector == '*') {
            lcd.write(byte(0));
        } else {
            lcd.print(selector);
        }
    }
}

void display_main_menu() {
    lcd.clear();
    switch (main_menu_idx) {
        case 0:
            display_menu_helper(0, "LED options", "Display");
            break;

        case 1:
            display_menu_helper(1, "LED options", "Display");
            break;

        case 2:
            display_menu_helper(0, "Temperature", "About");
            break;

        case 3:
            display_menu_helper(1, "Temperature", "About");
            break;
    }
}

void display_sub_menu() {
    lcd.clear();
    switch (curr_gr) {
        case led_options:
            switch (sub_menu_idx) {
                case 0:
                    display_menu_helper(0, "Power: "+led_power_state, "Red");
                    break;
                case 1:
                    display_menu_helper(1, "Power: "+led_power_state, "Red");
                    break;
                case 2:
                    display_menu_helper(0, "Green", "Blue");
                    break;
                case 3:
                    display_menu_helper(1, "Green", "Blue");
                    break;
            }
            break;
        case display:
            switch (sub_menu_idx) {
                case 0:
                    display_menu_helper(0, "Backlight: "+lcd_backlight_state, "Selector: ");
                    break;
                case 1:
                    display_menu_helper(1, "Backlight: "+lcd_backlight_state, "Selector: ");
                    break;
            }
            break;
        case temperature:
            switch (sub_menu_idx) {
                case 0:
                    display_menu_helper(0, "Sensor IN", "Sensor OUT");
                    break;
                case 1:
                    display_menu_helper(1, "Sensor IN", "Sensor OUT");
                    break;
                case 2:
                    display_menu_helper(0, "Units: "+temp_state, "                ");
                    break;
            }
            break;
        case about:
            lcd.clear();
            lcd.setCursor(0, 0);
            lcd.print("Author:");
            lcd.setCursor(0, 1);
            lcd.print("Michal Madeiski");
            break;
    }
}

void switch_led_power() {
    led_power = !led_power;
    if (led_power) {
        led_power_state = "ON ";
        analogWrite(LED_RED, red_intensity);
        analogWrite(LED_GREEN, green_intensity);
        analogWrite(LED_BLUE, blue_intensity);
    } else {
        led_power_state = "OFF";
        analogWrite(LED_RED, 0);
        analogWrite(LED_GREEN, 0);
        analogWrite(LED_BLUE, 0);
    }
    lcd.setCursor(8, 0);
    lcd.print(led_power_state);
}

void switch_lcd_backlight() {
    lcd_backlight = !lcd_backlight;
    if (lcd_backlight) {
        lcd_backlight_state = "ON ";
        lcd.backlight();
    } else {
        lcd_backlight_state = "OFF";
        lcd.noBacklight();
    }
    lcd.setCursor(12, 0);
    lcd.print(lcd_backlight_state);
}

void switch_selector() {
    lcd.setCursor(11, 1);
    if (selector == '*') {
        lcd.write(byte(0));
    } else {
        lcd.print(selector);
    }
}

void switch_temperature() {
    temp_is_celsius = !temp_is_celsius;
    if (temp_is_celsius) {
        temp_state = "C";
    } else {
        temp_state = "F";
    }
    lcd.setCursor(8, 0);
    lcd.print(temp_state);
}

void display_temperature(bool in) {
    displaying = true;
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print(in ? "Sensor IN:" : "Sensor OUT:");
    lcd.setCursor(0, 1);
    int t = temp_is_celsius ? temp_C : temp_F;
    lcd.print(t);
    lcd.print(temp_is_celsius ? "C" : "F");
}

void change_intensity(int pin, int* intensity, int delta) {
    displaying = true;
    if (intenisty_changing) {
        if (*intensity == 255) {
            *intensity = 0;
        } else {
            *intensity = *intensity + delta;
        }
    }
    if (led_power) {
        analogWrite(pin, *intensity);
    }
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Intensity: ");
    lcd.print(*intensity);
}

void setup() {
    pinMode(LED_RED, OUTPUT);
    pinMode(LED_GREEN, OUTPUT);
    pinMode(LED_BLUE, OUTPUT);
    pinMode(ENCODER1, INPUT_PULLUP);
    pinMode(ENCODER2, INPUT_PULLUP);
    pinMode(RED_BUTTON, INPUT_PULLUP);
    pinMode(GREEN_BUTTON, INPUT_PULLUP);

    lcd.init();
    lcd.backlight();
    lcd.createChar(0, arrow_right);
    display_main_menu();

    PCICR |= (1 << PCIE1);
    PCMSK1 |= (1 << PCINT10);
}

int lastEn1 = LOW;
unsigned long lastChangeTimestamp = 0UL;
void loop()
{

    int en1;
    int en2;
    unsigned long timestamp;

    ATOMIC_BLOCK(ATOMIC_RESTORESTATE)
    {
        en1 = encoder1;
        en2 = encoder2;
        timestamp = encoderTimestamp;
    }

    if (en1 == LOW && timestamp > lastChangeTimestamp + DEBOUNCING_PERIOD)
    {
        if (en2 == HIGH)
        {
            if (curr_lvl == main_menu && main_menu_idx < 3) main_menu_idx++;
            else if (curr_lvl == sub_menu) {
                if (curr_gr == led_options && sub_menu_idx < 3) sub_menu_idx++;
                else if (curr_gr == display && sub_menu_idx < 1) sub_menu_idx++; 
                else if (curr_gr == temperature && sub_menu_idx < 2) sub_menu_idx++; 
            }
        }
        else
        {
            if (curr_lvl == main_menu && main_menu_idx > 0) main_menu_idx--;
            else if (curr_lvl == sub_menu && sub_menu_idx > 0) sub_menu_idx--;
        }
        lastChangeTimestamp = timestamp;
        if (curr_lvl == main_menu) display_main_menu();
        else display_sub_menu();
    }

    if (isGreenButtonPressed()) {
        if (curr_lvl == main_menu) {
            curr_lvl = sub_menu;
            curr_gr = (MenuGroup)main_menu_idx;
            sub_menu_idx = 0;
            display_sub_menu();
        } else {
            if (curr_gr == led_options) {
                if (displaying) {
                    intenisty_changing = true;
                }
                switch (sub_menu_idx) {
                    case 0:
                        switch_led_power();
                        break;
                    case 1:
                        change_intensity(6, &red_intensity, 15);
                        break;
                    case 2:
                        change_intensity(5, &green_intensity, 15);
                        break;
                    case 3:
                        change_intensity(3, &blue_intensity, 15);
                        break;
                }
            } else if (curr_gr == display) {
                switch (sub_menu_idx) {
                    case 0:
                        switch_lcd_backlight();
                        break;
                    case 1:
                        switch (selector) {
                            case '>':
                                selector = '-';
                                break;
                            case '-':
                                selector = '*';
                                break;
                            case '*':
                                selector = '>';
                                break;
                        }
                        switch_selector();
                        break;
                }
            } else if (curr_gr == temperature) {
                switch (sub_menu_idx) {
                    case 0:
                        display_temperature(true);
                        break;
                    case 1:
                        display_temperature(false);
                        break;
                    case 2:
                        switch_temperature();
                        break;
                }
            }
        }
    }

    if (isRedButtonPressed()) {
        if (curr_lvl == sub_menu) {
            if (displaying == true) {
                display_sub_menu();
                displaying = false;
                intenisty_changing = false;
            } else {
                curr_lvl = main_menu;
                display_main_menu();
            }
        }
    }
    lastEn1 = en1;
}