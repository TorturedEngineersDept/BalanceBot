import boto3
from botocore.exceptions import NoCredentialsError
import os


def upload_to_s3(bucket_name, file_path, object_name=None):
    if object_name is None:
        # Prepend 'incoming/' to the file name
        object_name = 'incoming/' + os.path.basename(file_path)

    s3_client = boto3.client('s3', region_name='eu-west-2')

    try:
        s3_client.upload_file(file_path, bucket_name, object_name)
        print(f"File {file_path} uploaded to {bucket_name}/{object_name}")
    except FileNotFoundError:
        print(f"The file {file_path} was not found")
    except NoCredentialsError:
        print("Credentials not available")
    except boto3.exceptions.S3UploadFailedError as e:
        print(
            f"Failed to upload {file_path} to {bucket_name}/{object_name}: {e}")


bucket_name = 'ted-balance-bot'
pgm_file_path = 'map.pgm'
yaml_file_path = 'map.yaml'

pgm_file_path = os.path.join(os.getcwd(), pgm_file_path)
yaml_file_path = os.path.join(os.getcwd(), yaml_file_path)

upload_to_s3(bucket_name, pgm_file_path)
upload_to_s3(bucket_name, yaml_file_path)
