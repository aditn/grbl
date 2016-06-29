import time
import serial

def float_to_hex(f):
  return hex(struct.unpack('<I', struct.pack('<f', f))[0])

# configure the serial connections (the parameters differs on the device you are connecting to)
ser = serial.Serial(
        port='/dev/ttyACM1',
        baudrate=38400,
        parity=serial.PARITY_NONE,
        stopbits=serial.STOPBITS_ONE,
        bytesize=serial.EIGHTBITS)

ser.isOpen()

print 'Enter your commands below.\r\nInsert "exit" to leave the application.'

en=0;
input=1
while 1 :
  # get keyboard input
  input = raw_input(">> ")
    # Python 3 users
    # input = input(">> ")
  if input == 'exit':
    ser.close()
    exit()
  else:
    # send the character to the device
    # (note that I happend a \r\n carriage return and line feed to the characters - this is requested by my device)
    if (en):
      input = chr(eval(input));
    if (input == 'F'):
      en = 1;
    ser.write(input)
    out = ''
    # let's wait one second before reading output (let's give device time to answer)
    time.sleep(1)
    while ser.inWaiting() > 0:
      out += ser.read(1)
    if out != '':
      print ">>" + out

