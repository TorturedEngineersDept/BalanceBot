import yaml
import json
from PIL import Image
import numpy as np
from pathlib import Path

# Define the input and output directories
input_dir = Path(__file__).resolve().parent.parent / 'website' /'data' / 'map_raw'
output_images_dir = Path(__file__).resolve().parent.parent / 'website' /'data' / 'map_processed'
output_data_dir = output_images_dir

# Ensure output directories exist
output_images_dir.mkdir(parents=True, exist_ok=True)
output_data_dir.mkdir(parents=True, exist_ok=True)

# Load the YAML file
yaml_file = input_dir / 'map.yaml'
with yaml_file.open('r') as file:
    map_data = yaml.safe_load(file)

# Extract map metadata
map_image_file = input_dir / map_data['image']
map_resolution = map_data['resolution']
map_origin = map_data['origin']

# Read the PGM file
def read_pgm(filename):
    with filename.open('r') as file:
        assert file.readline() == 'P2\n'
        # next(file)  # Skip the comment line
        width, height = [int(i) for i in file.readline().split()]
        max_val = int(file.readline())
        data = []
        for line in file:
            data.extend([int(i) for i in line.split()])
        return np.array(data).reshape((height, width))

pgm_data = read_pgm(map_image_file)

# Convert the PGM data to an image and save as PNG
pgm_image = Image.fromarray(np.uint8(pgm_data))
png_image_file = output_images_dir / map_image_file.with_suffix('.png').name
pgm_image.save(png_image_file)

# Extract waypoints (assuming waypoints are stored in the YAML file)
waypoints = map_data.get('waypoints', [])

# Save waypoints and map metadata to a JSON file
map_info = {
    'image': str(png_image_file.relative_to(output_images_dir.parent.parent)),
    'resolution': map_resolution,
    'origin': map_origin,
    'waypoints': waypoints
}

json_file_path = output_data_dir / 'map.json'
with json_file_path.open('w') as json_file:
    json.dump(map_info, json_file, indent=4)

print("Map processed and JSON file created.")
