"""
This should be ran as a service on the Raspberry Pi.

This script will publish the Raspberry Pi's IP address to two MQTT brokers.
"""

#!/usr/bin/env python3

import paho.mqtt.client as mqtt
import os
import socket
import requests
import time

def get_inet_address():
    try:
        # Connect to a remote server on the internet
        remote_server = "www.google.com"
        with socket.socket(socket.AF_INET, socket.SOCK_DGRAM) as s:
            s.connect((remote_server, 80))
            inet_address = s.getsockname()[0]
    except Exception:
        # Unable to get the inet address locally, try using an external service
        try:
            inet_address = requests.get('https://api.ipify.org').text
        except Exception:
            inet_address = None
    return inet_address

# Publisher
while True:
    client = mqtt.Client()

    # Publish to AWS
    client.connect("35.176.179.237", 1883, 60)
    client.publish("rpi/connection", get_inet_address())
    client.disconnect()

    # Publish to own server
    client.connect("51.182.207.41", 1883, 60)
    client.publish("rpi/connection", get_inet_address())
    client.disconnect()

    time.sleep(10)
