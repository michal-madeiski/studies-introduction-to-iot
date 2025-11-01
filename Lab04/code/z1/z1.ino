#include <LiquidCrystal_I2C.h>
#include <OneWire.h>
#include <DallasTemperature.h>

#define LED_RED 6
#define LED_GREEN 5
#define LED_BLUE 3


bool checkIfIsNumber(String s) {
    for (unsigned int i = 0; i < s.length(); i++) {
      if (!isDigit(s[i])) {
        return false;
      }
    }
    return s.length() > 0;
}

bool checkIfNumberIsInRange(int n) {
    if (n >= 0 && n <= 100) {
      return true;
    }
    return false;
}


void setup()
{
    pinMode(LED_RED, OUTPUT);
    analogWrite(LED_RED, 0);

    pinMode(LED_GREEN, OUTPUT);
    analogWrite(LED_GREEN, 0);

    pinMode(LED_BLUE, OUTPUT);
    analogWrite(LED_BLUE, 0);

    Serial.begin(9600);
    while (!Serial)
    {
    }
    Serial.println("ENTER COLOR AND INTENSITY (0-100) SEPARATED BY SPACE");
}

void loop()
{
    if (Serial.available() > 0)
    {   
        String command = Serial.readStringUntil('\n');
        command.trim();
        command.toLowerCase();

        String command1 = "";
        String command2 = "";
        int spaceIndex = command.indexOf(' ');
        if (spaceIndex != -1) {
          command1 = command.substring(0, spaceIndex);
          command2 = command.substring(spaceIndex + 1);
        }

        if (command1 == "red")
        {
            if (checkIfIsNumber(command2)) {
              int number = command2.toInt();
              if (checkIfNumberIsInRange(number)) {
                  int intensity = round((number / 100.0) * 255);
                  analogWrite(LED_RED, intensity);
                  // digitalWrite(LED_GREEN, LOW);
                  // digitalWrite(LED_BLUE, LOW);
                  // Serial.println("RED LED IS ON");
              }
              else 
              {
                Serial.print("NUMBER: ");
                Serial.print(number);
                Serial.println(" IS NOT IN RANGE");
              }
            }
            else 
            {
              Serial.print("UNKNOWN NUMBER: ");
              Serial.println(command2);
            }
        }
        else if (command1 == "green")
        {
            if (checkIfIsNumber(command2)) {
              int number = command2.toInt();
              if (checkIfNumberIsInRange(number)) {
                  int intensity = round((number / 100.0) * 255);
                  analogWrite(LED_GREEN, intensity);
                  // digitalWrite(LED_RED, LOW);
                  // digitalWrite(LED_BLUE, LOW);
                  // Serial.println("GREEN LED IS ON");
              }
              else 
              {
                Serial.print("NUMBER: ");
                Serial.print(number);
                Serial.println(" IS NOT IN RANGE");
              }
            }
            else 
            {
              Serial.print("UNKNOWN NUMBER: ");
              Serial.println(command2);
            }
        }
        else if (command1 == "blue")
        {
            if (checkIfIsNumber(command2)) {
              int number = command2.toInt();
              if (checkIfNumberIsInRange(number)) {
                  int intensity = round((number / 100.0) * 255);
                  analogWrite(LED_BLUE, intensity);
                  // digitalWrite(LED_RED, LOW);
                  // digitalWrite(LED_GREEN, LOW);
                  // Serial.println("BLUE LED IS ON");
              }
              else 
              {
                Serial.print("NUMBER: ");
                Serial.print(number);
                Serial.println(" IS NOT IN RANGE");
              }
            }
            else 
            {
              Serial.print("UNKNOWN NUMBER: ");
              Serial.println(command2);
            }
        }
        else 
        {
          if (command1.length() > 0) 
          {
            Serial.println(String("UNKNOWN COLOR: ") + command1);
          }
          else 
          {
            Serial.println(String("INCORRECT COMMNAND: ") + command);
          }
        }
    }
}