#!/usr/bin/env python3

import time
# import RPi.GPIO as GPIO
# from config import *
import os
import sys
from threading import Timer
from gpiozero import Device
from gpiozero.pins.mock import MockFactory
Device.pin_factory = MockFactory()
sys.path.append(os.path.abspath(os.path.join(os.path.dirname(__file__), '..')))
from config_mock import *

red_led = led1
green_led = led2
blue_led = led3
red_button = buttonRed

def simulate_press():
    print("\n>>>SYMULACJA: naciskanie czerwonego przycisku")
    red_button.pin.drive_low()
    time.sleep(0.1)
    red_button.pin.drive_high()

def red_button_test():
    while not red_button.is_pressed:
    # while GPIO.input(buttonRed) == GPIO.HIGH:
        print('*', end='', flush=True)
        time.sleep(0.1)

if __name__ == "__main__":
    print("Program started")
    Timer(3, simulate_press).start()
    red_button_test()
    print("Program finished")