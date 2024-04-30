import Adafruit_BBIO.GPIO as GPIO
import time

led_pin = "USR3"

GPIO.setup(led_pin, GPIO.OUT)

try:
    
    while True:
        GPIO.output(led_pin, GPIO.HIGH)
        print("LED ON")
        time.sleep(0.5)  

        
        GPIO.output(led_pin, GPIO.LOW)
        print("LED OFF")
        time.sleep(0.5) 

except KeyboardInterrupt:
    GPIO.cleanup()