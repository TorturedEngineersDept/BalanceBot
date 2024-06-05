import boto3
import paho.mqtt.client as mqtt
import json
from concurrent.futures import ThreadPoolExecutor
from botocore.exceptions import ClientError

# MQTT settings
broker = '18.132.10.124'
port = 1883
topics = ["esp32/battery"]

# DynamoDB settings
dynamodb = boto3.resource('dynamodb', region_name='eu-west-2')
runs = dynamodb.Table('Runs')

# ThreadPoolExecutor for handling database operations
executor = ThreadPoolExecutor(max_workers=10)

# MQTT callbacks


def on_connect(client, userdata, flags, rc):
    print("Connected with result code " + str(rc))
    for topic in topics:
        client.subscribe(topic)


def on_message(client, userdata, msg):
    if msg.topic == "esp32/battery":
        process_battery(msg)
    else:
        print(f"Unknown topic: {msg.topic}")

# ====== HANDLE BATTERY MESSAGE =========


def process_battery(msg):
    try:
        print("Received battery message:", msg.payload.decode())
        data = json.loads(msg.payload.decode())
        run_id = data['RunID']
        timestamp = data['timestamp']
        battery = data['battery']

        # Submit the database operation to the ThreadPoolExecutor
        future = executor.submit(create_battery_entry,
                                 run_id, timestamp, battery)
    except Exception as e:
        print("Error processing battery message:", str(e))


def create_battery_entry(run_id, timestamp, battery):
    try:
        runs.put_item(
            Item={
                'RunID': f'{run_id}',
                'DataType-Timestamp': f'Battery-{timestamp}',
                'Timestamp': timestamp,
                'Battery': battery
            }
        )
        print(f'Battery entry created for RunID: {run_id}')
    except ClientError as e:
        print(
            f'Error creating battery entry for timestamp: {timestamp}, error: {e}')


# Initialize the counter table
def initialize_counters():
    try:
        counter_table.put_item(
            Item={'CounterName': 'BotID', 'LastBotID': 0},
            ConditionExpression='attribute_not_exists(CounterName)'
        )
        print("BotID counter initialized.")
    except ClientError as e:
        if e.response['Error']['Code'] != 'ConditionalCheckFailedException':
            print(
                f"Unexpected error occurred during BotID initialization: {e}")

    try:
        counter_table.put_item(
            Item={'CounterName': 'RunID', 'LastRunID': 0},
            ConditionExpression='attribute_not_exists(CounterName)'
        )
        print("RunID counter initialized.")
    except ClientError as e:
        if e.response['Error']['Code'] != 'ConditionalCheckFailedException':
            print(
                f"Unexpected error occurred during RunID initialization: {e}")


initialize_counters()

client = mqtt.Client()
client.on_connect = on_connect
client.on_message = on_message

client.connect(broker, port, 60)
client.loop_forever()
