import yaml
import json
from PIL import Image
import numpy as np

# Load the YAML file
with open('map.yaml', 'r') as file:
    map_data = yaml.safe_load(file)

# Extract map metadata
map_image_file = map_data['image']
map_resolution = map_data['resolution']
map_origin = map_data['origin']

# Read the PGM file
def read_pgm(filename):
    with open(filename, 'r') as file:
        assert file.readline() == 'P2\n'
        next(file)  # Skip the comment line
        width, height = [int(i) for i in file.readline().split()]
        max_val = int(file.readline())
        data = []
        for line in file:
            data.extend([int(i) for i in line.split()])
        return np.array(data).reshape((height, width))

pgm_data = read_pgm(map_image_file)

# Convert the PGM data to an image and save as PNG
pgm_image = Image.fromarray(np.uint8(pgm_data))
png_image_file = map_image_file.replace('.pgm', '.png')
pgm_image.save(png_image_file)

# Extract waypoints (assuming waypoints are stored in the YAML file)
waypoints = map_data.get('waypoints', [])

# Save waypoints and map metadata to a JSON file
map_info = {
    'image': png_image_file,
    'resolution': map_resolution,
    'origin': map_origin,
    'waypoints': waypoints
}

with open('map_info.json', 'w') as json_file:
    json.dump(map_info, json_file, indent=4)

print("Map processed and JSON file created.")
