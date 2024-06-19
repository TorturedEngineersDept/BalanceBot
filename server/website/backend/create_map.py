import numpy as np

def create_room_pgm(filename, width, height):
    # Create an empty grid with 255 (white space)
    grid = np.full((height, width), 255, dtype=int)

    # Create walls (0 value)
    grid[0, :] = 0           # Top wall
    grid[-1, :] = 0          # Bottom wall
    grid[:, 0] = 0           # Left wall
    grid[:, -1] = 0          # Right wall
    
    # Create a door (255 value in the wall)
    door_position = width // 2
    grid[0, door_position-1:door_position+2] = 255  # Door in the top wall

    # Write to PGM file
    with open(filename, 'w') as f:
        f.write('P2\n')
        f.write(f'{width} {height}\n')
        f.write('255\n')
        for row in grid:
            f.write(' '.join(map(str, row)) + '\n')

# Generate the PGM file
create_room_pgm('output/map.pgm', 20, 20)
