import json
import boto3
from boto3.dynamodb.conditions import Key
from decimal import Decimal

# Initialize the DynamoDB resource
dynamodb = boto3.resource('dynamodb')
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
        # Extract RunID from query parameters
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
                'body': json.dumps({'error': 'RunID query parameter is required'})
            }

        print(f"RunID extracted from query parameters: {run_id}")

        # Ensure RunID is treated as an integer
        run_id = int(run_id)

        # Fetch data from DynamoDB
        response = table.query(
            KeyConditionExpression=Key('RunId').eq(run_id) & Key(
                'DataType-Timestamp').begins_with('Power-')
        )

        items = response.get('Items', [])
        print(f"Items retrieved: {items}")

        # Convert all Decimal objects to int/float
        converted_items = convert_decimal(items)

        # Sort items by Timestamp
        sorted_items = sorted(converted_items, key=lambda x: x['Timestamp'])

        # Extract the required data
        data = [{'Timestamp': item['Timestamp'], 'Power': item['Power']}
                for item in sorted_items]

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


# CORS headers are there to prevent browser from blocking the request
