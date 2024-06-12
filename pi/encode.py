def encode(linear, angular):
    # Stop, Speed = 31
    encoded = 0b00011111

    if linear > 0:
        # Go forward = 0b000
        encoded |= (0b000 << 5)
    elif linear < 0:
        # Go backwards
        encoded |= (0b001 << 5)
    elif angular > 0:
        # Go right
        encoded |= (0b011 << 5)
    elif angular < 0:
        # Go left
        encoded |= (0b010 << 5)
    else:
        # Stop
        encoded = 0b10000000

    return encoded

print(bin(encode(0.18, -0.08), 8))
