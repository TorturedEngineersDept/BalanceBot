import mqtt from 'mqtt';

let client;

export const initializeMQTT = (setBatteryPercentage, globalRunId, topic) => {
    const MQTT_BROKER = "18.130.87.186";
    const MQTT_PORT = 8000;

    client = mqtt.connect(`ws://${MQTT_BROKER}:${MQTT_PORT}`, {
        reconnectPeriod: 1000,
        connectTimeout: 30 * 1000,
        keepalive: 60
    });

    client.on('connect', () => {
        console.log('Connected to MQTT broker');

        client.subscribe(topic, (err) => {
            if (!err) {
                console.log(`Subscribed to topic: ${topic}`);
            }
        });
    });

    client.on('message', (receivedTopic, message) => {
        if (receivedTopic === topic) {
            const data = JSON.parse(message.toString());
            if (data.run_id === globalRunId) {
                console.log('Received data:', data);
                setBatteryPercentage(data.battery);
            }
        }
        const msgString = message.toString();
        console.log(`Received message: ${msgString} on topic: ${receivedTopic}`);
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

export const SendDirections = (key, globalRunId) => {
    if (!client) {
        console.error('MQTT client is not initialized');
        return;
    }
    const message = JSON.stringify({
        run_id: globalRunId,
        direction: key
    });
    client.publish('user/joystick', message);
};
