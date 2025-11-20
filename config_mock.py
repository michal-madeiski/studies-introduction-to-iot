from gpiozero import LED, Button
from gpiozero.pins.mock import MockFactory
from gpiozero import Device

Device.pin_factory = MockFactory()

led1 = LED(13)
led2 = LED(12)
led3 = LED(19)
led4 = LED(26)

buttonRed = Button(5, pull_up=True)
buttonGreen = Button(6, pull_up=True)

encoderLeft = Button(17, pull_up=True)
encoderRight = Button(27, pull_up=True)

buzzerPin = LED(23)
ws2812pin = 8