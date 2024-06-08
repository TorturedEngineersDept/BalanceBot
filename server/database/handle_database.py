# This script should be run as DAEMON on EC2 instance

import boto3
import paho.mqtt.client as mqtt
import json
from concurrent.futures import ThreadPoolExecutor
from botocore.exceptions import ClientError
import os
import signal
import sys

# MQTT settings from environment variables or defaults
broker = os.getenv('MQTT_BROKER', '18.130.87.186')
port = int(os.getenv('MQTT_PORT', 1883))
topics = os.getenv('MQTT_TOPICS', "esp32/battery").split(',')

# DynamoDB settings
dynamodb = boto3.resource('dynamodb', region_name='eu-west-2')
runs = dynamodb.Table('Runs')

# ThreadPoolExecutor for handling database operations
executor = ThreadPoolExecutor(max_workers=10)

# MQTT callbacks


def on_connect(client, userdata, flags, rc):
    print(f"Connected with result code {rc}")
    for topic in topics:
        client.subscribe(topic)


def on_message(client, userdata, msg):
    print(f"Received message on topic {msg.topic}")
    if msg.topic == "esp32/battery":
        process_battery(msg)
    else:
        print(f"Unknown topic: {msg.topic}")

# Handle battery message


def process_battery(msg):
    try:
        print("Processing battery message")
        data = json.loads(msg.payload.decode())
        run_id = data['run_id']
        timestamp = data['timestamp']
        battery = data['battery']

        # Submit the database operation to the ThreadPoolExecutor
        executor.submit(create_battery_entry, run_id, timestamp, battery)
    except Exception as e:
        print(f"Error processing battery message: {str(e)}")


def create_battery_entry(run_id, timestamp, battery):
    try:
        runs.put_item(
            Item={
                'RunId': run_id,
                'DataType-Timestamp': f'Battery-{timestamp}',
                'Timestamp': timestamp,
                'Battery': battery
            }
        )
        print(f'Battery entry created for RunID: {run_id}')
    except ClientError as e:
        print(
            f'Error creating battery entry for timestamp: {timestamp}, error: {e}')

# Handle graceful shutdown


def signal_handler(sig, frame):
    print("Shutting down gracefully...")
    client.disconnect()
    client.loop_stop()
    executor.shutdown(wait=True)
    print("Shutdown complete")
    sys.exit(0)


# Register signal handlers for graceful shutdown
signal.signal(signal.SIGINT, signal_handler)
signal.signal(signal.SIGTERM, signal_handler)

# Main
client = mqtt.Client()
client.on_connect = on_connect
client.on_message = on_message

client.connect(broker, port, 60)
client.loop_forever()
