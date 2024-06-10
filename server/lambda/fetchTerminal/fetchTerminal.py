import json
import boto3
from boto3.dynamodb.conditions import Key
from decimal import Decimal

# Initialize the DynamoDB resource
dynamodb = boto3.resource('dynamodb', region_name='eu-west-2')
table = dynamodb.Table('Runs')  # Replace with your table name

# Helper function to convert DynamoDB items to JSON serializable format


def convert_decimal(obj):
    if isinstance(obj, list):
        return [convert_decimal(i) for i in obj]
    elif isinstance(obj, dict):
        return {k: convert_decimal(v) for k, v in obj.items()}
    elif isinstance(obj, Decimal):
        return int(obj) if obj % 1 == 0 else float(obj)
    else:
        return obj


def lambda_handler(event, context):
    try:
        # Extract RunId from query parameters
        query_params = event.get('queryStringParameters', {})
        run_id = query_params.get('RunId')

        if not run_id:
            return {
                'statusCode': 400,
                'headers': {
                    'Access-Control-Allow-Origin': '*',
                    'Access-Control-Allow-Headers': 'Content-Type',
                    'Access-Control-Allow-Methods': 'GET'
                },
                'body': json.dumps({'error': 'RunId query parameter is required'})
            }

        print(f"RunId extracted from query parameters: {run_id}")

        # Ensure RunId is treated as an integer
        run_id = int(run_id)

        # Fetch data from DynamoDB for CLI messages
        cli_response = table.query(
            KeyConditionExpression=Key('RunId').eq(run_id) & Key(
                'DataType-Timestamp').begins_with('CLI-'),
            ScanIndexForward=False  # False to sort by descending order
        )

        # Fetch data from DynamoDB for Debug messages
        debug_response = table.query(
            KeyConditionExpression=Key('RunId').eq(run_id) & Key(
                'DataType-Timestamp').begins_with('Debug-'),
            ScanIndexForward=False  # False to sort by descending order
        )

        cli_items = cli_response.get('Items', [])
        debug_items = debug_response.get('Items', [])

        # Combine the items
        items = cli_items + debug_items
        print(f"Items retrieved: {items}")

        # Convert all Decimal objects to int/float
        converted_items = convert_decimal(items)

        # Sort items by Timestamp in ascending order
        sorted_items = sorted(converted_items, key=lambda x: x['Timestamp'])

        # Keep only the last 10 items
        last_10_items = sorted_items[-10:]

        # Extract the required data
        data = [{'Timestamp': item['Timestamp'], 'MessageType': item['MessageType'],
                 'Message': item['Message']} for item in last_10_items]

        return {
            'statusCode': 200,
            'headers': {
                'Access-Control-Allow-Origin': '*',
                'Access-Control-Allow-Headers': 'Content-Type',
                'Access-Control-Allow-Methods': 'GET'
            },
            'body': json.dumps(data)
        }
    except Exception as e:
        print(f"Error fetching data from DynamoDB: {str(e)}")
        return {
            'statusCode': 500,
            'headers': {
                'Access-Control-Allow-Origin': '*',
                'Access-Control-Allow-Headers': 'Content-Type',
                'Access-Control-Allow-Methods': 'GET'
            },
            'body': json.dumps({'error': str(e)})
        }
