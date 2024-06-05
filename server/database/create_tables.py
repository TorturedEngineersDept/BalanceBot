import boto3
from botocore.exceptions import ClientError

# Initialize a session using Amazon DynamoDB
dynamodb = boto3.resource('dynamodb', region_name='eu-west-2')


def delete_table(table_name):
    try:
        table = dynamodb.Table(table_name)
        table.delete()
        table.wait_until_not_exists()
        print(f'Table {table_name} deleted successfully.')
    except ClientError as e:
        if e.response['Error']['Code'] == 'ResourceNotFoundException':
            print(f'Table {table_name} does not exist.')
        else:
            print(f'Error deleting table {table_name}: {e}')


def create_counter_table():
    try:
        counter_table = dynamodb.create_table(
            TableName='Counter',
            KeySchema=[
                {
                    'AttributeName': 'Counter',
                    'KeyType': 'HASH'  # Partition key
                }
            ],
            AttributeDefinitions=[
                {
                    'AttributeName': 'Counter',
                    'AttributeType': 'S'  # String
                }
            ],
            ProvisionedThroughput={
                'ReadCapacityUnits': 5,
                'WriteCapacityUnits': 5
            }
        )

        # Wait table creation
        counter_table.meta.client.get_waiter(
            'table_exists').wait(TableName='Counter')
        print(f'Table {counter_table.table_name} created successfully.')

    except ClientError as e:
        print(f'Error creating Counter table: {e}')


def initialize_counter_table():
    try:
        counter_table = dynamodb.Table('Counter')
        counter_table.put_item(
            Item={
                'Counter': 'BotId',
                'LastBotId': 0
            }
        )
        counter_table.put_item(
            Item={
                'Counter': 'RunId',
                'LastRunId': 0
            }
        )
    except ClientError as e:
        print(f'Error initializing Counter table: {e}')


def create_runid_table():
    try:
        runid_table = dynamodb.create_table(
            TableName='RunID',
            KeySchema=[
                {
                    'AttributeName': 'BotId',
                    'KeyType': 'HASH'  # Partition key
                }
            ],
            AttributeDefinitions=[
                {
                    'AttributeName': 'BotId',
                    'AttributeType': 'N'  # Number
                }
            ],
            ProvisionedThroughput={
                'ReadCapacityUnits': 5,
                'WriteCapacityUnits': 5
            }
        )

        # Wait table creation
        runid_table.meta.client.get_waiter(
            'table_exists').wait(TableName='RunID')
        print(f'Table {runid_table.table_name} created successfully.')

    except ClientError as e:
        print(f'Error creating Counter table: {e}')


def create_botid_table():
    try:
        botid_table = dynamodb.create_table(
            TableName='BotID',
            KeySchema=[
                {
                    'AttributeName': 'MACAddress',
                    'KeyType': 'HASH'  # Partition key
                },
                {
                    'AttributeName': 'BotId',
                    'KeyType': 'RANGE'  # Sort key
                }
            ],
            AttributeDefinitions=[
                {
                    'AttributeName': 'MACAddress',
                    'AttributeType': 'S'  # String
                },
                {
                    'AttributeName': 'BotId',
                    'AttributeType': 'N'  # Number
                }
            ],
            ProvisionedThroughput={
                'ReadCapacityUnits': 5,
                'WriteCapacityUnits': 5
            }
        )

        # Wait table creation
        botid_table.meta.client.get_waiter(
            'table_exists').wait(TableName='BotID')
        print(f'Table {botid_table.table_name} created successfully.')

    except ClientError as e:
        print(f'Error creating BotID table: {e}')


def create_runs_table():
    try:
        runs_table = dynamodb.create_table(
            TableName='Runs',
            KeySchema=[
                {
                    'AttributeName': 'RunId',
                    'KeyType': 'HASH'  # Partition key
                },
                {
                    'AttributeName': 'DataType-Timestamp',
                    'KeyType': 'RANGE'  # Sort key
                }
            ],
            AttributeDefinitions=[
                {
                    'AttributeName': 'RunId',
                    'AttributeType': 'N'  # Number
                },
                {
                    'AttributeName': 'DataType-Timestamp',
                    'AttributeType': 'S'  # String
                }
            ],
            ProvisionedThroughput={
                'ReadCapacityUnits': 5,
                'WriteCapacityUnits': 5
            }
        )

        # Wait table creation
        runs_table.meta.client.get_waiter(
            'table_exists').wait(TableName='Runs')
        print(f'Table {runs_table.table_name} created successfully.')

    except ClientError as e:
        print(f'Error creating BotID table: {e}')


# Clear existing tables if they exist
delete_table('Counter')
delete_table('BotID')
delete_table('RunID')
delete_table('Runs')

# Create new tables
create_counter_table()
create_botid_table()
create_runid_table()
create_runs_table()

# Initialize Counter table
initialize_counter_table()
