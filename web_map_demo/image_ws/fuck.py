with open('/home/kev/image_ws/map.pgm', 'r') as file:
    content = file.read()

content = content.replace('254', '150')

with open('/home/kev/image_ws/map.pgm', 'w') as file:
    file.write(content)
