import struct
import serial
from time import sleep
from pynput.keyboard import Key, Controller



dir = ['left', 'right', 'up', 'down', 'Stay']

'''
def clean(s):
	t = ''
	for i in range(len(s)):
		if s[i] in range(48, 59):
			t = t + str((s[i]-48))
	return(int(t))
'''

def press(a):
	a = int(a)
	if(a == 0):
		keyboard.press(Key.left)
		sleep(0.1)
		keyboard.release(Key.left)
	elif(a == 1):
		keyboard.press(Key.right)
		sleep(0.1)
		keyboard.release(Key.right)
	elif(a == 2):
		keyboard.press(Key.ctrl)
		keyboard.press(Key.left)
		sleep(0.1)
		keyboard.release(Key.ctrl)
		keyboard.release(Key.left)
	elif(a == 3):
		keyboard.press(Key.ctrl)
		keyboard.press(Key.right)
		sleep(0.1)
		keyboard.release(Key.ctrl)
		keyboard.release(Key.right)
	elif(a == 4):
		keyboard.press(Key.ctrl)
		sleep(0.1)
		keyboard.release(Key.ctrl)


if __name__ == '__main__':
	
	arduino = serial.Serial()
	keyboard = Controller()

	arduino.setDTR(1)
	sleep(2)
	arduino.setDTR(0)
	
	try:
		arduino = serial.Serial('COM9', timeout=1, baudrate=9600)
	except:
		print('Serial not available')

	while 1:
		getVal = (arduino.readline())
		print(getVal)
		try:
			press(getVal)
		except ValueError:
			pass
