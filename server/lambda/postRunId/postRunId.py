import json
import boto3
from botocore.exceptions import ClientError
from decimal import Decimal

dynamodb = boto3.resource('dynamodb', region_name='eu-west-2')
identifiers_table = dynamodb.Table('RunID')


def decimal_default(obj):
    if isinstance(obj, Decimal):
        return int(obj)
    raise TypeError


def lambda_handler(event, context):
    try:
        # Get the 'botId' parameter from the POST request body
        body = json.loads(event['body'])
        bot_id = body.get('BotId')

        if not bot_id:
            return {
                'statusCode': 400,
                'body': json.dumps({
                    'error': 'BotID is required'
                })
            }

        # Check or retrieve RunID for the given BotID
        response = identifiers_table.get_item(Key={'BotId': int(bot_id)})

        if 'Item' in response:
            run_id = response['Item']['RunId']
        else:
            return {
                'statusCode': 404,
                'body': json.dumps({
                    'error': 'BotID not found'
                })
            }

        return {
            'statusCode': 200,
            'body': json.dumps({
                'RunId': run_id
            }, default=decimal_default)
        }
    except ClientError as e:
        print(f"ClientError: {str(e)}")
        return {
            'statusCode': 500,
            'body': json.dumps({
                'error': str(e)
            })
        }
    except Exception as e:
        print(f"Exception: {str(e)}")
        return {
            'statusCode': 500,
            'body': json.dumps({
                'error': str(e)
            })
        }
