#!/usr/bin/env python3
import rclpy
import time
from rclpy.node import Node
from geometry_msgs.msg import Twist
import serial
import json

# Open the serial port
ser = serial.Serial('/dev/ttyUSB0', 115200, timeout=1)

time.sleep(1)

class CmdVelSubscribe(Node):
    def __init__(self,name):
        super().__init__(name)
        # Create the subscriber
        self.command_subscribe_ = self.create_subscription(Twist,"cmd_vel",self.command_callback,10)

    def encode(self, linear, angular):
        # Stop, Speed = 31
        encoded = 0b00011111

        if linear > 0:
            # Go forward = 0b000
            encoded |= (0b000 << 5)
        elif linear < 0:
            # Go backwards
            encoded |= (0b001 << 5)
        elif angular > 0:
            # Go right
            encoded |= (0b011 << 5)
        elif angular < 0:
            # Go left
            encoded |= (0b010 << 5)
        else:
            # Stop
            encoded = 0b10000000

        return encoded

    def command_callback(self,msg):
        print("msg.linear.x:  ", msg.linear.x)
        print("msg.angular.z:  ", msg.angular.z)
        angular_velocity = msg.angular.z
        # Prevents rotating in place at speeds too low to rotate
        if msg.linear.x == 0:
            if 0<angular_velocity<0.2:
                angular_velocity = 0.2
            elif -0.2<angular_velocity<0:
                angular_velocity = -0.2

        encoded = self.encode(msg.linear.x, msg.angular.z)
        print(bin(encoded))
        ser.write(encoded.to_bytes(1, 'big'))

def main(args=None):
    rclpy.init(args=args) # Initialize rclpy
    node = CmdVelSubscribe("robot_control")  # Create a node
    rclpy.spin(node) # Keep the nodes running and test whether receive the command to exit (Ctrl+C)
    rclpy.shutdown() # turn off rclpy
    # Turn off the serial port
    ser.close()
