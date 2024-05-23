# Server

This folder contains the code running on the server:
- User Interface
- Application layer protocol used for communications
- Back-end and database

22/05/2024: We will be running two AWS EC2 instances for:
- MOSQUITTO BROKER, enabling MQTT connection between ESP32 and WebServer
- APACHE, hosting the webserver remotely

Writes to a DynamoDB database will be triggered by specific MQTT topics messages arriving. This will probably be implemented through LAMBDA functions.

## Connecting to EC2 instance through SSH

I have attached the security key `AWS-EoY.pem` for convenience, in case anyone feels like trying it out for themselves.

In your terminal, `cd` to this folder, then run:

``` Shell
ssh -i "AWS-EoY.pem" ubuntu@ec2-18-132-10-124.eu-west-2.compute.amazonaws.com
````

## MOSQUITTO BROKER

To launch a MOSQUITTO instance on our EC2 instance, first make sure to allow all incoming TCP Traffic.

```bash
sudo apt update
sudo apt install mosquitto mosquitto-clients
sudo systemctl start mosquitto
```

Enable mosquitto to run as soon server is rebooted:

```bash
sudo systemctl enable mosquitto
```

Check instance ran without error:

```bash
systemctl status mosquitto
```

Modify configuration file to suit our needs:

```bash
sudo nano /etc/mosquitto/mosquitto.conf 
```

Now, allow traffic through port 8000 for WebSockets and 1883 for pure TCP:

``` Shell
persistence true
persistence_location /var/lib/mosquitto/

# Enable verbose logging for debugging
log_type all
log_dest file /var/log/mosquitto/mosquitto.log

include_dir /etc/mosquitto/conf.d

# Default listener for MQTT
listener 1883
protocol mqtt

# Listener for Websocket connectins
listener 8000
protocol websockets

# Enable anonymous access
allow_anonymous true
```

Now, ensure ports 8000 and 1883 are free to be used:

```bash
sudo lsof -i :8000
sudo lsof -i :1883
```

Kill all processes that appear through the instruction:

```bash
sudo kill <process_id>
```

You might also need to change privileges to allow Mosquitto to write to the log file:

```bash
chmod +rwx /var/log/mosquitto/mosquitto.log 
```

Then, restart the mosquitto instance:

```bash
mosquitto -c /etc/mosquitto/mosquitto.conf -v
```

To access logs, run:

```bash
tail -f /var/log/mosquitto/mosquitto.log
```