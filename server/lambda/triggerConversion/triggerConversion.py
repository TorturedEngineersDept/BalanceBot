import boto3
import json
from botocore.exceptions import ClientError

dynamodb = boto3.resource('dynamodb', region_name='eu-west-2')
file_upload_tracker = dynamodb.Table('FileUploadTracker')

s3 = boto3.client('s3')
lambda_client = boto3.client('lambda')


def lambda_handler(event, context):
    print("Received event:", json.dumps(event, indent=2))

    bucket_name = event['Records'][0]['s3']['bucket']['name']
    key = event['Records'][0]['s3']['object']['key']
    file_type = 'pgm' if key.endswith('.pgm') else 'yaml'

    print(f"Bucket: {bucket_name}, Key: {key}, FileType: {file_type}")

    try:
        # Update FileUploadTracker table
        print("Updating FileUploadTracker table...")
        response = file_upload_tracker.update_item(
            Key={'id': 'latest_upload'},
            UpdateExpression=f'SET {file_type}_file = :key',
            ExpressionAttributeValues={':key': key},
            ReturnValues='UPDATED_NEW'
        )
        print("Update response:", response)
    except ClientError as e:
        print(f"ClientError during update: {str(e)}")
        return {
            'statusCode': 500,
            'headers': {
                'Access-Control-Allow-Origin': '*',
                'Access-Control-Allow-Headers': 'Content-Type',
                'Access-Control-Allow-Methods': 'GET'
            },
            'body': json.dumps({'error': str(e)})
        }

    except Exception as e:
        print(f"Exception during update: {str(e)}")
        return {
            'statusCode': 500,
            'headers': {
                'Access-Control-Allow-Origin': '*',
                'Access-Control-Allow-Headers': 'Content-Type',
                'Access-Control-Allow-Methods': 'GET'
            },
            'body': json.dumps({'error': str(e)})
        }

    try:
        # Check if both files are uploaded
        print("Checking if both files are uploaded...")
        response = file_upload_tracker.get_item(
            Key={'id': 'latest_upload'}
        )
        print("Get item response:", response)

        if not isinstance(response, dict):
            print("Unexpected response type:", type(response))
            response = json.loads(response)

        # Ensure the 'Item' key is present and is a dictionary
        if 'Item' not in response or not isinstance(response['Item'], dict):
            print("Invalid item structure:", response)
            raise ValueError("Invalid item structure")

        item = response['Item']
        pgm_file = item.get('pgm_file', None)
        yaml_file = item.get('yaml_file', None)

        print(f"PGM File: {pgm_file}, YAML File: {yaml_file}")

        if pgm_file and yaml_file:
            try:
                # Trigger the processing function
                print("Triggering the convertFiles Lambda function...")
                response = lambda_client.invoke(
                    FunctionName='arn:aws:lambda:eu-west-2:590183976402:function:convertFiles',
                    InvocationType='Event',
                    Payload=json.dumps(
                        {'pgm_file': pgm_file, 'yaml_file': yaml_file})
                )
                print("Invoke response:", response)

                # Reset the FileUploadTracker table entry
                print("Resetting the FileUploadTracker table entry...")
                response = file_upload_tracker.update_item(
                    Key={'id': 'latest_upload'},
                    UpdateExpression='REMOVE pgm_file, yaml_file',
                    ReturnValues='UPDATED_NEW'
                )
                print("Reset response:", response)

            except ClientError as e:
                print(f"ClientError during invoke or reset: {str(e)}")
                return {
                    'statusCode': 500,
                    'headers': {
                        'Access-Control-Allow-Origin': '*',
                        'Access-Control-Allow-Headers': 'Content-Type',
                        'Access-Control-Allow-Methods': 'GET'
                    },
                    'body': json.dumps({'error': str(e)})
                }

            except Exception as e:
                print(f"Exception during invoke or reset: {str(e)}")
                return {
                    'statusCode': 500,
                    'headers': {
                        'Access-Control-Allow-Origin': '*',
                        'Access-Control-Allow-Headers': 'Content-Type',
                        'Access-Control-Allow-Methods': 'GET'
                    },
                    'body': json.dumps({'error': str(e)})
                }

    except ClientError as e:
        print(f"ClientError during get item: {str(e)}")
        return {
            'statusCode': 500,
            'headers': {
                'Access-Control-Allow-Origin': '*',
                'Access-Control-Allow-Headers': 'Content-Type',
                'Access-Control-Allow-Methods': 'GET'
            },
            'body': json.dumps({'error': str(e)})
        }

    except Exception as e:
        print(f"Exception during get item: {str(e)}")
        return {
            'statusCode': 500,
            'headers': {
                'Access-Control-Allow-Origin': '*',
                'Access-Control-Allow-Headers': 'Content-Type',
                'Access-Control-Allow-Methods': 'GET'
            },
            'body': json.dumps({'error': str(e)})
        }

    return {
        'statusCode': 200,
        'body': 'DynamoDB table updated and processing triggered if both files are present'
    }
