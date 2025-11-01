#include <LiquidCrystal_I2C.h>
#include <OneWire.h>
#include <DallasTemperature.h>

#define POTENTIOMETER A0

LiquidCrystal_I2C lcd(0x27, 16, 2);


void setup()
{
    Serial.begin(9600);
    lcd.init();
    lcd.backlight();
    lcd.print("POTENTIOMETER A0");
}


void loop()
{
    static int prev_adc = 0;
    int nadc = analogRead(A0);
    if (abs(prev_adc - nadc) < 2) {
        nadc = prev_adc;
    } else {
        prev_adc = nadc;
    }
    nadc = min(max(0, nadc - 10), 1003);
    double vin = (5.0/1004.0) * nadc;
    lcd.setCursor(0, 1);
    lcd.print("V=");
    lcd.print(vin);
    lcd.print(" ADC=");
    String s_nadc = String(nadc);
    lcd.setCursor(12, 1);
    for (int i = 0; i < (4 - s_nadc.length()); i++) {
        lcd.print(" ");
    }
    lcd.print(nadc);

    Serial.print("V(mV):");
    Serial.print(vin*1000);
    Serial.print("\t");
    Serial.print("ADC:");
    Serial.println(nadc);

    delay(100);
}