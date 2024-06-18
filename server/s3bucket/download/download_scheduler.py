import boto3
from botocore.exceptions import NoCredentialsError, ClientError
import os
import hashlib
import schedule
import time
import signal
import sys


def calculate_file_hash(file_path):
    hash_alg = hashlib.md5()
    try:
        with open(file_path, "rb") as f:
            for chunk in iter(lambda: f.read(4096), b""):
                hash_alg.update(chunk)
    except FileNotFoundError:
        return None
    return hash_alg.hexdigest()


def download_from_s3(bucket_name, object_name, output_path):
    # Initialize S3 client
    s3_client = boto3.client('s3', region_name='eu-west-2')
    temp_output_path = output_path + '.tmp'

    try:
        # Download the file to a temporary location
        s3_client.download_file(bucket_name, object_name, temp_output_path)
        print(f"File {object_name} downloaded to {temp_output_path}")

        # Calculate the hashes of the old and new files
        old_file_hash = calculate_file_hash(output_path)
        new_file_hash = calculate_file_hash(temp_output_path)

        if old_file_hash == new_file_hash:
            print(f"File {object_name} is unchanged. Keeping the old file.")
            os.remove(temp_output_path)
        else:
            print(f"File {object_name} is updated. Replacing the old file.")
            os.replace(temp_output_path, output_path)

    except FileNotFoundError:
        print(f"The file {output_path} was not found")
    except NoCredentialsError:
        print("Credentials not available")
    except ClientError as e:
        print(f"Failed to download {object_name} from {bucket_name}: {e}")
    except Exception as e:
        print(f"An error occurred: {e}")


def download_files():
    # Replace these values with your bucket name and file paths
    bucket_name = 'ted-balance-bot'
    png_object_name = 'output/map.png'
    json_object_name = 'output/map.json'
    incident_object_name = 'output/incident.jpg'
    output_dir = '.'

    # Ensure output directory exists
    if not os.path.exists(output_dir):
        os.makedirs(output_dir)

    # Local paths where files will be saved
    png_output_path = os.path.join(
        output_dir, '../../website/public/data/map_processed/map.png')
    json_output_path = os.path.join(
        output_dir, '../../website/public/data/map_processed/map.json')
    incident_output_path = os.path.join(
        output_dir, '../../website/public/data/incidents/incident.jpg')

    # Download the .png file
    download_from_s3(bucket_name, png_object_name, png_output_path)

    # Download the .json file
    download_from_s3(bucket_name, json_object_name, json_output_path)

    # Download the .jpg file
    download_from_s3(bucket_name, incident_object_name, incident_output_path)


def signal_handler(sig, frame):
    print("Received termination signal. Exiting gracefully...")
    sys.exit(0)


# Register signal handler
signal.signal(signal.SIGINT, signal_handler)
signal.signal(signal.SIGTERM, signal_handler)

# Schedule the download_files function to run every 5 seconds
schedule.every(5).seconds.do(download_files)

print("Scheduler started. Press Ctrl+C to stop.")

# Run the scheduler
while True:
    schedule.run_pending()
    time.sleep(5)
