import time
import os

LEDs=4
LEDPATH='/sys/class/leds/beaglebone:green:usr'


f = []
for i in range(LEDs):
    f.append(open(LEDPATH+str(i)+"/brightness", "w"))

# Sequence
while True:
    for i in range(LEDs):
        f[i].seek(0)
        f[i].write("1")
        time.sleep(0.5)
    for i in range(LEDs):
        f[i].seek(0)
        f[i].write("0")
        time.sleep(0.5)
