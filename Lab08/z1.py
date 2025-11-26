#!/usr/bin/env python3

from config import *
import RPi.GPIO as GPIO
import time

leds_pwm = []
current_led = 0
current_brightness = [0, 0, 0, 0]

def green_button_callback(channel):
    global current_led
    leds_pwm[current_led].ChangeDutyCycle(0)
    current_led = (current_led + 1) % 4
    leds_pwm[current_led].ChangeDutyCycle(current_brightness[current_led])
    print(f"Wybrano diodę: LED{current_led + 1}, Jasność: {current_brightness[current_led]}%")

def setupPWM():
    leds = [led1, led2, led3, led4]
    for led in leds:
        led_pwm = GPIO.PWM(led, 100)
        led_pwm.start(0)
        leds_pwm.append(led_pwm)

def encoder_callback(channel):
    left_state = GPIO.input(encoderLeft)
    right_state = GPIO.input(encoderRight)

    if channel == encoderLeft:
        if left_state == GPIO.LOW and right_state == GPIO.HIGH:
            current_brightness[current_led] = max(0, current_brightness[current_led] - 5)
        else:
            current_brightness[current_led] = min(100, current_brightness[current_led] + 5)
    elif channel == encoderRight:
        if right_state == GPIO.LOW and left_state == GPIO.HIGH:
            current_brightness[current_led] = min(100, current_brightness[current_led] + 5)
        else:
            current_brightness[current_led] = max(0, current_brightness[current_led] - 5)
    
    leds_pwm[current_led].ChangeDutyCycle(current_brightness[current_led])
    print(f"Aktualna dioda {current_led + 1}, aktualna intensywność {current_brightness[current_led]}%")

def cleanup():
    print("\nCzyszczenie zasobów...")
    for pwm in leds_pwm:
        pwm.stop()
    GPIO.cleanup()
    print("Zasoby wyczyszczone.")

def main():
    try:
        setupPWM()
        
        GPIO.add_event_detect(buttonGreen, GPIO.FALLING, callback=green_button_callback, bouncetime=300)
        GPIO.add_event_detect(encoderLeft, GPIO.BOTH, callback=encoder_callback, bouncetime=50)
        GPIO.add_event_detect(encoderRight, GPIO.BOTH, callback=encoder_callback, bouncetime=50)
        
        print("Program uruchomiony. Naciśnij Ctrl+C aby zakończyć.")
        print(f"Aktualna dioda: LED{current_led + 1}")
        
        while True:
            time.sleep(0.1)
            
    except KeyboardInterrupt:
        print("\nPrzerwano przez użytkownika (Ctrl+C)")
    finally:
        cleanup()

if __name__ == "__main__": 
    main()