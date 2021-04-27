#!/usr/bin/python3

# Read from serial device and store all data to a file
# usage:
#       read_serial.py <port> <baudrate> <outfile>
#
# ex)   ./read_serial.py /dev/ttyUSB0 115200
import serial
import sys

if len(sys.argv) != 4:
    print("usage: read_serial.py <port> <baudrate> <outfile>")
    exit(-1)

s = serial.Serial(sys.argv[1], int(sys.argv[2]))
with open(sys.argv[3], "wb") as f:
    f.write(s.read())
