# Raspberry Pi

This folder contains the code to be uploaded to the Raspberry Pi. Note that the
Raspberry Pi already runs Linux, so here could be some services.

## Setup the Rasberry Pi

1. Use the Raspberry Pi Imager to flash Ubuntu 24.04 onto an SD card (must be 
larger than 4 GB).
2. Connect to the Internet and then download ROS2.
3. Copy the `main.py` and run it as a startup service.
4. Check the UART works with the `uart.py` script.

1. `ros2 run nav2_map_server map_server_cli -f maps/map_map`
1. nav2 with `yaml` of map
2. Then rviz should work

On the Raspverry Pi

```sh
ros2 run ldlidar_stl_ros2 ld19.launch.py
ros2 run map_remapper map_remapper
ros2 launch autonomous_exploration control
```
