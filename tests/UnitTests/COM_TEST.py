import serial

ser = serial.Serial('COM9', 115200)
print(ser.readline())