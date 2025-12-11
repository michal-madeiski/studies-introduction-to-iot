#!/usr/bin/env python3

import time
from PIL import Image, ImageDraw, ImageFont
import lib.oled.SSD1331 as SSD1331
import board
import busio
import adafruit_bme280.advanced as adafruit_bme280

def read_bme280():
    i2c = busio.I2C(board.SCL, board.SDA)
    bme280 = adafruit_bme280.Adafruit_BME280_I2C(i2c, 0x76)

    bme280.sea_level_pressure = 1020
    bme280.standby_period = adafruit_bme280.STANDBY_TC_500
    bme280.iir_filter = adafruit_bme280.IIR_FILTER_X16
    bme280.overscan_pressure = adafruit_bme280.OVERSCAN_X16
    bme280.overscan_humidity = adafruit_bme280.OVERSCAN_X1
    bme280.overscan_temperature = adafruit_bme280.OVERSCAN_X2

    return {
        "temperature": bme280.temperature,
        "humidity": bme280.humidity,
        "pressure": bme280.pressure,
        "altitude": bme280.altitude
    }

def draw_on_oled(disp, values):

    image = Image.new("RGB", (disp.width, disp.height), "WHITE")
    draw = ImageDraw.Draw(image)

    font = ImageFont.truetype('./lib/oled/Font.ttf', 13)

    icon_temp = Image.open('./icons/temperature.png')
    icon_humidity = Image.open('./icons/humidity.png')
    icon_pressure = Image.open('./icons/pressure.png')
    icon_altitude = Image.open('./icons/altitude.png')

    y = 0

    image.paste(icon_temp, (0, y))
    draw.text((20, y), f"{values['temperature']:.1f}"+chr(176)+"C", font=font, fill="BLACK")
    y += 16

    image.paste(icon_humidity, (0, y))
    draw.text((20, y), f"{values['humidity']:.1f} %", font=font, fill="BLACK")
    y += 16

    image.paste(icon_pressure, (0, y))
    draw.text((20, y), f"{values['pressure']:.1f} hPa", font=font, fill="BLACK")
    y += 16

    image.paste(icon_altitude, (0, y))
    draw.text((20, y), f"{values['altitude']:.1f} m", font=font, fill="BLACK")

    disp.ShowImage(image, 0, 0)

def main():
    disp = SSD1331.SSD1331()
    disp.Init()
    disp.clear()

    print("\nProgram started, press Ctrl+C to finish")

    try:
        while True:
            values = read_bme280()
            draw_on_oled(disp, values)
            time.sleep(2.0)

    except KeyboardInterrupt:
        print("\nInterrupted by the user (Ctrl+C)")
    finally:
        disp.clear()
        disp.reset()
        print("\nProgram finished")

if __name__ == "__main__":
    main()