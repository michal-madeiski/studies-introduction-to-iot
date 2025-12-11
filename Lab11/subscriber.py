#!/usr/bin/env python3
# -*- coding: utf-8 -*-

import paho.mqtt.client as mqtt
import sqlite3
import json
import time
import os

BROKER = "localhost" 
TOPIC = "lab/rfid"

def on_connect(client, userdata, flags, rc):
    print(f"Connected to broker (code: {rc})")
    client.subscribe(TOPIC) 
    print(f"Listening to a channel: {TOPIC}")

def on_message(client, userdata, msg):
    try:
        payload_str = msg.payload.decode("utf-8") 
        print(f"\n[RECEIVED] {payload_str}")
        
        data = json.loads(payload_str)
                
    except Exception as e:
        print(f"Message processing error: {e}")

def run_subscriber():
    
    client = mqtt.Client()
    client.on_connect = on_connect
    client.on_message = on_message

    try:
        client.connect(BROKER)
        client.loop_forever()
    except KeyboardInterrupt:
        print("\nReceiver stopped")
        client.disconnect()
    except ConnectionRefusedError:
        print("Unable to connect to broker")

if __name__ == "__main__":
    run_subscriber()