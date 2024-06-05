// src/utils/mqttServiceDashboard.js
import mqtt from 'mqtt';

export const initializeMQTT = (setBatteryPercentage, setBatteryData, globalRunId) => {
    const MQTT_BROKER = "18.132.10.124";
    const MQTT_PORT = 8000;

    const client = mqtt.connect(`ws://${MQTT_BROKER}:${MQTT_PORT}`, {
        reconnectPeriod: 1000,
        connectTimeout: 30 * 1000,
        keepalive: 60
    });

    client.on('connect', () => {
        console.log('Connected to MQTT broker');

        client.subscribe('esp32/battery', (err) => {
            if (!err) {
                console.log('Subscribed to topic: esp32/battery');
            }
        });
    });

    client.on('message', (topic, message) => {
        if (topic === 'esp32/battery') {
            const data = JSON.parse(message.toString());
            if (data.run_id === globalRunId) {
                console.log('Received data:', data);
                setBatteryPercentage(data.battery);
                setBatteryData(prevData => {
                    const newData = Array.isArray(prevData) ? [...prevData, { x: data.timestamp, y: data.battery }] : [{ x: data.timestamp, y: data.battery }];
                    console.log('New data array:', newData);
                    return newData;
                });
            }
        }
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
};
