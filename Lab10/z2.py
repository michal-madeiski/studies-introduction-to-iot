import time
import datetime
import board
import neopixel
import RPi.GPIO as GPIO
from config import * # pylint: disable=unused-wildcard-import
from mfrc522 import MFRC522

pixels = neopixel.NeoPixel(board.D18, 8, brightness=1.0/32, auto_write=False)
MISSED_READS_THRESHOLD = 4

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
                        
                        print(f"[REGISTRATION] UID: {current_uid_str} | Time: {timestamp}")
                        
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
    finally:
        visual_signal_off()
        buzzer(False)
        GPIO.cleanup()
        print("\nProgram finished")

if __name__ == "__main__":
    rfid_loop()