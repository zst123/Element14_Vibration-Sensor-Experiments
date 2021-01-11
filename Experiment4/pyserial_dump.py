import serial
import sys

ser = sys.argv[1]
ser = serial.Serial(ser, 57600)
ser.flushInput()
ser.flushOutput()

while True:
	print(ser.read().decode(), end="", flush=True)
