import mqtt from 'mqtt';
import { updateBattery } from './updateBattery';
import { handleCoordinates } from './handleCoordinates';

export function initializeMQTT() {
    const INTERVAL_STEERING_UPDATE = 200;
    let currentSpeed = 0;
    let currentAngle = 0;

    const MQTT_BROKER = "18.132.10.124";
    const MQTT_PORT = 8000;

    const client = mqtt.connect(`ws://${MQTT_BROKER}:${MQTT_PORT}`, {
        reconnectPeriod: 1000,
        connectTimeout: 30 * 1000,
        keepalive: 60
    });

    client.on('connect', () => {
        console.log('Connected to MQTT broker');

        client.subscribe('esp32/send', err => {
            if (!err) {
                client.publish('esp32/receive', 'Hello from MQTT over WebSockets!');
            }
        });
        client.subscribe('esp32/battery');
        client.subscribe('esp32/mapping');
    });

    client.on('message', (topic, message) => {
        if (topic === 'esp32/battery') {
            const data = JSON.parse(message.toString());
            console.log('Received data:', data);
            const percentage = data.battery;
            console.log('Battery percentage:', percentage);
            updateBattery(percentage);
        }
        if (topic === 'esp32/mapping') {
            const data = JSON.parse(message.toString());
            console.log('Received data:', data);
            const x = data.x;
            const y = data.y;
            console.log('Coordinates:', x, y);
            handleCoordinates(x, y);
        }
        const msgString = message.toString();
        console.log(`Received message: ${msgString} on topic: ${topic}`);
    });

    client.on('error', error => {
        console.error('Connection error:', error);
    });

    client.on('close', () => {
        console.log('MQTT client disconnected');
    });

    client.on('reconnect', () => {
        console.log('MQTT client reconnecting...');
    });

    client.on('offline', () => {
        console.log('MQTT client offline');
    });

    function polar_coordinates(speed, angle) {
        currentSpeed = speed;
        currentAngle = angle;
    }

    setInterval(() => {
        client.publish('user/joystick', JSON.stringify({ speed: currentSpeed, angle: currentAngle }));
    }, INTERVAL_STEERING_UPDATE);

    window.polar_coordinates = polar_coordinates;
}
