import yaml
import json
from PIL import Image
import numpy as np
from pathlib import Path

def ensure_dir_exists(directory):
    directory.mkdir(parents=True, exist_ok=True)

def read_pgm(filename):
    with filename.open('rb') as file:  # Open the file in binary mode
        header = file.readline().decode('ascii').strip()
        assert header == 'P5', 'Not a binary PGM file'
        width, height = [int(i) for i in file.readline().decode('ascii').split()]
        max_val = int(file.readline().decode('ascii'))
        # Read the rest of the data
        data = np.fromfile(file, dtype=np.uint8).reshape((height, width))
        return data

def yaml_to_json(yaml_file, json_file):
    # Load the YAML file
    with yaml_file.open('r') as file:
        map_data = yaml.safe_load(file)

    # Extract map metadata
    map_image_file = yaml_file.parent / map_data['image']
    map_resolution = map_data['resolution']
    map_origin = map_data['origin']

    # Define the output PNG file
    png_image_file = json_file.parent / map_image_file.with_suffix('.png').name

    # Extract waypoints (assuming waypoints are stored in the YAML file)
    waypoints = map_data.get('waypoints', [])

    # Save waypoints and map metadata to a JSON file
    map_info = {
        'image': str(png_image_file.relative_to(json_file.parent.parent)),
        'resolution': map_resolution,
        'origin': map_origin,
        'waypoints': waypoints
    }

    with json_file.open('w') as file:
        json.dump(map_info, file, indent=4)

def pgm_to_png(pgm_file, png_file):
    # Read the PGM file
    pgm_data = read_pgm(pgm_file)

    # Convert the PGM data to an image and save as PNG
    pgm_image = Image.fromarray(np.uint8(pgm_data))
    pgm_image.save(png_file)

def process_map():
    # Define the input and output directories
    data_dir = Path(__file__).resolve().parent.parent / 'public' / 'data'
    input_dir = data_dir / 'map_raw'
    output_dir = data_dir / 'map_processed'

    # Ensure output directories exist
    ensure_dir_exists(output_dir)

    yaml_file = input_dir / 'map.yaml'
    json_file = output_dir / 'map.json'
    yaml_to_json(yaml_file, json_file)
    
    pgm_file = input_dir / 'map.pgm'
    png_file = output_dir / 'map.png'
    pgm_to_png(pgm_file, png_file)

    print("Map processed and JSON file created.")

if __name__ == '__main__':
    process_map()
