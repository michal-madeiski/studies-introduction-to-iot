# #!/usr/bin/env python3

import os
import sys
import time
from threading import Timer
from gpiozero import Device
from gpiozero.pins.mock import MockFactory
# import RPi.GPIO as GPIO

sys.path.append(os.path.abspath(os.path.join(os.path.dirname(__file__), '..')))
from config_mock import *
# from config import *

Device.pin_factory = MockFactory()

red_led = led1
green_led = led2
blue_led = led3

red_button = buttonRed

def simulatePress():
    print("\n>>>SYMULACJA: naciskanie zielonego przycisku")
    red_button.pin.drive_low()
    time.sleep(0.1)
    red_button.pin.drive_high()

def redButtonTest():
    while not red_button.is_pressed:
    # while GPIO.input(buttonRed) == GPIO.HIGH:
        print('*', end='', flush=True)
        time.sleep(0.1)

if __name__ == "__main__":
    print("Program started")
    Timer(3, simulatePress).start()
    redButtonTest()
    print("Program finished")