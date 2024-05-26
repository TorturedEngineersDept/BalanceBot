import mqtt from 'mqtt';
import { updateBattery } from './battery';
import { handleCoordinates } from './map';

let INTERVAL_STEERING_UPDATE = 200;
let currentSpeed = 0;
let currentAngle = 0;

const MQTT_BROKER = "18.132.10.124";
const MQTT_PORT = 8000;

export const setupMQTT = () => {
    const client = mqtt.connect(`ws://${MQTT_BROKER}:${MQTT_PORT}`, {
        reconnectPeriod: 1000,
        connectTimeout: 30 * 1000,
        keepalive: 60
    });

    client.on('connect', function () {
        console.log('Connected to MQTT broker');
        client.subscribe('esp32/send');
        client.subscribe('esp32/battery');
        client.subscribe('esp32/mapping');
    });

    client.on('message', function (topic, message) {
        if (topic === 'esp32/battery') {
            const data = JSON.parse(message.toString());
            const percentage = data.battery;
            updateBattery(percentage);
        }
        if (topic === 'esp32/mapping') {
            const data = JSON.parse(message.toString());
            const { x, y } = data;
            handleCoordinates(x, y);
        }
    });

    client.on('error', function (error) {
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

    setInterval(() => {
        client.publish('user/joystick', JSON.stringify({ speed: currentSpeed, angle: currentAngle }));
    }, INTERVAL_STEERING_UPDATE);
};