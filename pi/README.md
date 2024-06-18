# Raspberry Pi

This folder contains the code to be uploaded to the Raspberry Pi. Note that the
Raspberry Pi already runs Linux, so here could be some services.

## Setup the Rasberry Pi

1. Use the Raspberry Pi Imager to flash Ubuntu 24.04 onto an SD card (must be 
larger than 4 GB).
2. Connect to the Internet and then download ROS2.
3. Copy the `main.py` and run it as a startup service.
4. Check the UART works with the `uart.py` script.
