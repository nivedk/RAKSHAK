import serial
import sys
import glob
import time

# replace the portname with the port your arduino is connected ot
s = serial.Serial(port='/dev/tty.usbmodem1411', baudrate=9600)
first_arg = sys.argv[1]
second_arg = sys.argv[2]

print first_arg
print "stepx ^^"
print second_arg
print "stepy^^"

s.write(first_arg)
#The delay was kept because the arduino was messing up otherwise
time.sleep(1)
s.write(second_arg)
