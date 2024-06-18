import boto3
import yaml
import json
from PIL import Image
import numpy as np
from pathlib import Path
import logging

# Initialize S3 client with a specific region
s3 = boto3.client('s3', region_name='eu-west-2')

# Set up logging
logger = logging.getLogger()
logger.setLevel(logging.INFO)
handler = logging.StreamHandler()
formatter = logging.Formatter('%(asctime)s %(levelname)s %(message)s')
handler.setFormatter(formatter)
logger.addHandler(handler)


def ensure_dir_exists(directory):
    directory.mkdir(parents=True, exist_ok=True)


def read_pgm(file_content):
    try:
        header = file_content.readline().decode('ascii').strip()
        if header != 'P5':
            raise ValueError('Not a binary PGM file')
        width, height = [
            int(i) for i in file_content.readline().decode('ascii').split()]
        max_val = int(file_content.readline().decode('ascii'))
        data = np.frombuffer(file_content.read(),
                             dtype=np.uint8).reshape((height, width))
        return data
    except Exception as e:
        logger.error(f"Error reading PGM file: {e}")
        raise


def yaml_to_json(yaml_content, json_file_path):
    try:
        map_data = yaml.safe_load(yaml_content)
        map_image_file = Path(map_data['image']).with_suffix('.png')
        map_resolution = map_data['resolution']
        map_origin = map_data['origin']
        waypoints = map_data.get('waypoints', [])

        map_info = {
            'image': str(map_image_file),
            'resolution': map_resolution,
            'origin': map_origin,
            'waypoints': waypoints
        }

        with open(json_file_path, 'w') as file:
            json.dump(map_info, file, indent=4)
    except Exception as e:
        logger.error(f"Error converting YAML to JSON: {e}")
        raise


def pgm_to_png(pgm_content, png_file_path):
    try:
        pgm_data = read_pgm(pgm_content)
        pgm_image = Image.fromarray(np.uint8(pgm_data))
        pgm_image.save(png_file_path)
    except Exception as e:
        logger.error(f"Error converting PGM to PNG: {e}")
        raise


def lambda_handler(event, context):
    bucket_name = 'ted-balance-bot'
    pgm_key = 'incoming/map.pgm'
    yaml_key = 'incoming/map.yaml'
    output_dir = Path('/tmp/map_processed')
    ensure_dir_exists(output_dir)

    try:
        logger.info(f"Fetching PGM file from S3: {pgm_key}")
        pgm_obj = s3.get_object(Bucket=bucket_name, Key=pgm_key)
        pgm_content = pgm_obj['Body']

        logger.info(f"Fetching YAML file from S3: {yaml_key}")
        yaml_obj = s3.get_object(Bucket=bucket_name, Key=yaml_key)
        yaml_content = yaml_obj['Body'].read().decode('utf-8')

        png_file_path = output_dir / 'map.png'
        json_file_path = output_dir / 'map.json'

        logger.info("Converting PGM to PNG")
        pgm_to_png(pgm_content, png_file_path)

        logger.info("Converting YAML to JSON")
        yaml_to_json(yaml_content, json_file_path)

        logger.info(f"Uploading PNG file to S3: {png_file_path}")
        s3.upload_file(str(png_file_path), bucket_name, 'output/map.png')

        logger.info(f"Uploading JSON file to S3: {json_file_path}")
        s3.upload_file(str(json_file_path), bucket_name, 'output/map.json')

        logger.info("Processing completed successfully")
        return {
            'statusCode': 200,
            'body': 'Processed files and saved outputs: map.png, map.json'
        }

    except s3.exceptions.NoSuchKey as e:
        logger.error(f"File not found: {str(e)}")
        return {
            'statusCode': 404,
            'body': f'File not found: {str(e)}'
        }
    except s3.exceptions.ClientError as e:
        logger.error(f"Client error: {str(e)}")
        return {
            'statusCode': 500,
            'body': f'Client error: {str(e)}'
        }
    except Exception as e:
        logger.error(f"Error: {str(e)}")
        return {
            'statusCode': 500,
            'body': f'Error: {str(e)}'
        }
