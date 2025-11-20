#!/usr/bin/env python3

import os
import sys
import time
from threading import Timer
from gpiozero import LED, Button
from gpiozero import Device
from gpiozero.pins.mock import MockFactory
# from config import *

sys.path.append(os.path.abspath(os.path.join(os.path.dirname(__file__), '..')))
from config_mock import *

Device.pin_factory = MockFactory()

red_led = led1
green_led = led2
blue_led = led3

green_button = buttonGreen

def light_rgb_on_button():
    print("Naciśnij zielony przycisk")
    
    while True:
        if green_button.is_pressed:
            print("Zielony przycisk naciśnięty")
        
            red_led.on()
            green_led.on()
            blue_led.on()
            time.sleep(1)
            red_led.off()
            green_led.off()
            blue_led.off()

def simulate_press():
    print(">>>SYMULACJA: naciskanie zielonego przycisku")
    green_button.pin.drive_low()
    time.sleep(0.1)
    green_button.pin.drive_high()


if __name__ == "__main__":
    Timer(2, simulate_press).start()
    light_rgb_on_button()