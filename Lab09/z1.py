#!/usr/bin/env python3

from config import *
import w1thermsensor
import board
import busio
import adafruit_bme280.advanced as adafruit_bme280
import time

def ds18b20():
    sensor = w1thermsensor.W1ThermSensor()
    temp = sensor.get_temperature()

    result=f'\nDS18B200 \nTemp : {temp} '+chr(176)+'C'
    return result

def bme280():
    i2c = busio.I2C(board.SCL, board.SDA)
    bme280 = adafruit_bme280.Adafruit_BME280_I2C(i2c, 0x76)

    bme280.sea_level_pressure = 1010
    bme280.standby_period = adafruit_bme280.STANDBY_TC_500
    bme280.iir_filter = adafruit_bme280.IIR_FILTER_X16
    bme280.overscan_pressure = adafruit_bme280.OVERSCAN_X16
    bme280.overscan_humidity = adafruit_bme280.OVERSCAN_X1
    bme280.overscan_temperature = adafruit_bme280.OVERSCAN_X2

    result='\nBME280:'+f'\nTemperature: {bme280.temperature:0.1f}' +chr(176)+'C'+f'\nHumidity: {bme280.humidity:0.1f} %'+f'\nPressure: {bme280.pressure:0.1f} hPa'+f'\nAltitude: {bme280.altitude:0.2f} meters'
    return result

def test():
    res1=ds18b20()
    res2=bme280()
    res=res1+res2
    print(res)

if __name__ == "__main__":
    try:
        print("\nProgram started, press Ctrl+C to finish")
        while True:
            time.sleep(0.5)
            test()
            print('\033[8A\r', end='', flush=True)
    except KeyboardInterrupt:
        print("\nInterrupted by the user (Ctrl+C)")
    finally:
        GPIO.cleanup()
        print("\nProgram finished")