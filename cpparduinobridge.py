import serial
import sys
import glob
import time

#1421 can be replaced with 1411 for the other port
#ser = serial.Serial('/dev/cu.usbmodem1421',9600)
#ser.write("33333")

s = serial.Serial(port='/dev/tty.usbmodem1411', baudrate=9600)
first_arg = sys.argv[1]
second_arg = sys.argv[2]

print first_arg
print "stepx ^^"
print second_arg
print "stepy^^"

s.write(first_arg)
time.sleep(1)
s.write(second_arg)
'''
def printing(a):
    print 'hello!'
    print a





first_arg = sys.argv[1]
second_arg = sys.argv[2]

def greetings(word1=first_arg, word2=second_arg):
    #print("{} {}".format(word1, word2))
    #print word1
    #a = int(word1)
    s = serial.Serial(port='/dev/tty.usbmodem1411', baudrate=9600)
    s.write(word1)
    #q = s.read()
    #print q
if __name__ == "__main__":
    greetings("1")
    #greetings("Bonjour", "monde")
'''
