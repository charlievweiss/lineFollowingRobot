# this takes readings from an arduino and maps them onto a graph to display
# an image of the object scanned

import serial
import numpy as np
import matplotlib.pyplot as plt
import time


"""
Current model:
Move very slowly. Any detection will turn the robot in that
direction. If it comes across a corner, the robot will be 
moving slowly enough to turn completely.

Status from last night: (hey morning chrl)
Need to check if sending action is actually interpreted by arduino code
Need to send multiple actions in one line and have it interpreted as motor speeds
Need to figure out what the heck kind of motor speeds to use :D:D:D
and what even is PID

"""

class LineFollowing(object):
    def __init__(self):
        self.arduino_com_port = "/dev/ttyACM0"
        self.baud_rate = 9600
        self.serial_port = serial.Serial(self.arduino_com_port, self.baud_rate, timeout=1)
        # IR readings
        self.left_reading = 0
        self.right_reading = 0
        # Motor speeds
        self.left_motor_speed = 0 # 255 is forward
        self.right_motor_speed = 1
        # parameters
        self.sensor_threshold = 1000 #between sensor high (tape) and low (floor)

    # checks if left sensor reads tape
    def check_left_tape(self):
        # above threshold means tape, below means floor
        return left_reading > self.sensor_threshold

    # checks if right sensor reads tape
    def check_right_tape(self):
        # above threshold means tape, below means floor
        return right_reading > self.sensor_threshold

    # based on whether tape detected, will reset motor speeds
    def check_action_required(self):
        # cases:
        if not check_left_tape() and check_right_tape():
            # keep going forward
            print("chuggin' along")
        elif check_left_tape() and not check_right_tape():
            # turn left
            print('turning left')
        elif check_right_tape() and not check_left_tape():
            # turn right
            print('turning right')
        else:
            print("Stop! I'm confused :(")
            # stop! you're confused

    # hypothetically sends motor speeds to arduino over serial
    def send_action(self):
        command_string = '{}{}{}'.format(chr(255),chr(self.left_motor_speed),chr(self.right_motor_speed)
        print(command_string)
        self.serial_port.write(bytes(command_string, 'utf-8'))
        #self.serial_port.write(struct.pack('>BB',self.left_motor_speed,self.right_motor_speed))
        #self.serial_port.write(bytes(str(self.left_motor_speed), 'utf-8'))
        #self.serial_port.write(bytes(str(self.right_motor_speed), 'utf-8'))

    # hypothetically receives sesnor data from arduino over serial
    def get_data(self):
        # ask for a line of data from the serial port, the ".decode()" converts the
        # data from an "array of bytes", to a string
        sensor_data = self.serial_port.readline().decode()
        # check if data was received
        if len(sensor_data.split(",")) > 1:
            sensor_data = sensor_data.split(",")
            self.left_reading, self.right_reading = sensor_data[0], sensor_data[1]
            print(self.left_reading, self.right_reading)
        else:
            time.sleep(.003)


    def run(self):
        # currently troubleshooting serial communications
        while True:
            # get IR data
            self.get_data()
            self.send_action()


if __name__ == '__main__':
    func = LineFollowing()
    func.run()
