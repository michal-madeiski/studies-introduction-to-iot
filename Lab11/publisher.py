#!/usr/bin/env python3
# -*- coding: utf-8 -*-

import time
import datetime
import json
import board
import neopixel
import RPi.GPIO as GPIO
import paho.mqtt.client as mqtt 
from config import * 
from mfrc522 import MFRC522

BROKER = "localhost"   
TOPIC = "lab/rfid"     

pixels = neopixel.NeoPixel(board.D18, 8, brightness=1.0/32, auto_write=False)
MISSED_READS_THRESHOLD = 5

client = mqtt.Client()

def setup_mqtt():
    try:
        client.connect(BROKER)
        print(f"Connected to MQTT broker: {BROKER}")
    except Exception as e:
        print(f"MQTT connection error: {e}")

def buzzer(state):
    GPIO.output(buzzerPin, not state)

def visual_signal_on():
    pixels.fill((0, 0, 255))
    pixels.show()

def visual_signal_off():
    pixels.fill((0, 0, 0))
    pixels.show()

def uid_to_string(uid):
    return "-".join([str(x) for x in uid])

def rfid_loop():
    MIFAREReader = MFRC522()
    last_uid = None
    missed_reads = 0 

    visual_signal_off()
    buzzer(False)
    setup_mqtt() 

    print("\nStart of the RFID Registration System (MQTT Publisher)")
    print("Place the card on the reader...")

    try:
        while True:
            (status_req, TagType) = MIFAREReader.MFRC522_Request(MIFAREReader.PICC_REQIDL)
            read_success = False

            if status_req == MIFAREReader.MI_OK:
                (status_anti, uid) = MIFAREReader.MFRC522_Anticoll()

                if status_anti == MIFAREReader.MI_OK:
                    read_success = True
                    missed_reads = 0
                    current_uid_str = uid_to_string(uid)

                    
                    if current_uid_str != last_uid:
                        now = datetime.datetime.now()
                        timestamp = now.strftime("%Y-%m-%d %H:%M:%S.%f")[:-3]
                        
                        print(f"[READING] UID: {current_uid_str}")

                        payload = {
                            "uid": current_uid_str,
                            "timestamp": timestamp,
                            "device": "RaspberryPi_Lab"
                        }
                        client.publish(TOPIC, json.dumps(payload))
                        print(f"Send: {payload}")
                        
                        buzzer(True)
                        visual_signal_on()
                        time.sleep(0.5)
                        buzzer(False)
                        visual_signal_off()
                        
                        last_uid = current_uid_str
                    else:
                        pass

            if not read_success:
                missed_reads += 1
                if missed_reads >= MISSED_READS_THRESHOLD:
                    last_uid = None
                    missed_reads = MISSED_READS_THRESHOLD

            time.sleep(0.05)

    except KeyboardInterrupt:
        print("\nInterrupted by the user (Ctrl+C)")
        client.disconnect()
    finally:
        visual_signal_off()
        buzzer(False)
        GPIO.cleanup()
        print("\nProgram finished")

if __name__ == "__main__":
    rfid_loop()