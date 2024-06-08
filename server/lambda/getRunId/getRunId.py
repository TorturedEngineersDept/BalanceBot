import json
import boto3
from botocore.exceptions import ClientError
from boto3.dynamodb.conditions import Key
from decimal import Decimal

dynamodb = boto3.resource('dynamodb', region_name='eu-west-2')
counter_table = dynamodb.Table('Counter')
botid_table = dynamodb.Table('BotID')
runid_table = dynamodb.Table('RunID')


def decimal_default(obj):
    if isinstance(obj, Decimal):
        return int(obj)
    raise TypeError


def lambda_handler(event, context):
    # Get the 'mac' parameter from the query string
    mac_address = event.get('queryStringParameters', {}).get('mac')
    if not mac_address:
        return {
            'statusCode': 400,
            'headers': {
                'Access-Control-Allow-Origin': '*',
                'Access-Control-Allow-Headers': 'Content-Type',
                'Access-Control-Allow-Methods': 'GET'
            },
            'body': json.dumps({
                'error': 'MAC address is required'
            })
        }

    try:
        # Check or retrieve BotID for the given MAC address
        response = botid_table.query(
            KeyConditionExpression=Key('MACAddress').eq(mac_address)
        )
        items = response.get('Items', [])

        if items:
            bot_id = items[0]['BotId']
        else:
            # Create a new BotID
            response = counter_table.update_item(
                Key={'Counter': 'BotId'},
                UpdateExpression='SET LastBotId = if_not_exists(LastBotId, :start) + :inc',
                ExpressionAttributeValues={
                    ':inc': 1,
                    ':start': 0
                },
                ReturnValues='UPDATED_NEW'
            )
            bot_id = response['Attributes']['LastBotId']
            botid_table.put_item(
                Item={'MACAddress': mac_address, 'BotId': bot_id})

        # Increment RunId
        response = counter_table.update_item(
            Key={'Counter': 'RunId'},
            UpdateExpression='SET LastRunId = if_not_exists(LastRunId, :start) + :inc',
            ExpressionAttributeValues={
                ':inc': 1,
                ':start': 0
            },
            ReturnValues='UPDATED_NEW'
        )
        run_id = response['Attributes']['LastRunId']

        # Update RunId in RunId table
        runid_table.update_item(
            Key={'BotId': bot_id},
            UpdateExpression='SET RunId = :run_id',
            ExpressionAttributeValues={':run_id': run_id}
        )

        return {
            'statusCode': 200,
            'headers': {
                'Access-Control-Allow-Origin': '*',
                'Access-Control-Allow-Headers': 'Content-Type',
                'Access-Control-Allow-Methods': 'GET'
            },
            'body': json.dumps({
                'BotId': bot_id,
                'RunId': run_id
            }, default=decimal_default)
        }

    except ClientError as e:
        print(f"ClientError: {str(e)}")
        return {
            'statusCode': 500,
            'headers': {
                'Access-Control-Allow-Origin': '*',
                'Access-Control-Allow-Headers': 'Content-Type',
                'Access-Control-Allow-Methods': 'GET'
            },
            'body': json.dumps({
                'error': str(e)
            })
        }

    except Exception as e:
        print(f"Exception: {str(e)}")
        return {
            'statusCode': 500,
            'headers': {
                'Access-Control-Allow-Origin': '*',
                'Access-Control-Allow-Headers': 'Content-Type',
                'Access-Control-Allow-Methods': 'GET'
            },
            'body': json.dumps({
                'error': str(e)
            })
        }
