import boto3
from botocore.exceptions import NoCredentialsError, ClientError
import os


def download_from_s3(bucket_name, object_name, output_path):
    # Initialize S3 client
    s3_client = boto3.client('s3', region_name='eu-west-2')

    try:
        # Download the file
        s3_client.download_file(bucket_name, object_name, output_path)
        print(f"File {object_name} downloaded to {output_path}")
    except FileNotFoundError:
        print(f"The file {output_path} was not found")
    except NoCredentialsError:
        print("Credentials not available")
    except ClientError as e:
        print(f"Failed to download {object_name} from {bucket_name}: {e}")


# Replace these values with your bucket name and file paths
bucket_name = 'ted-balance-bot'
png_object_name = 'output/map.png'
json_object_name = 'output/map.json'
output_dir = '.'

# Ensure output directory exists
if not os.path.exists(output_dir):
    os.makedirs(output_dir)

# Local paths where files will be saved
png_output_path = os.path.join(output_dir, 'map.png')
json_output_path = os.path.join(output_dir, 'map.json')

# Download the .pgm file
download_from_s3(bucket_name, png_object_name, png_output_path)

# Download the .yaml file
download_from_s3(bucket_name, json_object_name, json_output_path)
