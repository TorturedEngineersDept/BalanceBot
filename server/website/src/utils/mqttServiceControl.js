import mqtt from 'mqtt';

let client;

export const initializeMQTT = (setBatteryPercentage, handleDebugMessage, globalRunId) => {
    const MQTT_BROKER = "18.130.87.186";
    const MQTT_PORT = 8000;

    if (client) {
        client.end();
    }

    client = mqtt.connect(`ws://${MQTT_BROKER}:${MQTT_PORT}`, {
        reconnectPeriod: 1000,
        connectTimeout: 30 * 1000,
        keepalive: 60
    });

    client.on('connect', () => {
        console.log('Connected to MQTT broker');

        client.subscribe("esp32/battery", (err) => {
            if (!err) {
                console.log(`Subscribed to topic esp32/battery`);
            }
        });
        client.subscribe("esp32/debug", (err) => {
            if (!err) {
                console.log(`Subscribed to topic esp32/debug`);
            }
        });
    });

    client.on('message', (receivedTopic, message) => {
        const data = JSON.parse(message.toString());
        if (data.run_id === globalRunId) {
            if (receivedTopic === "esp32/battery") {
                console.log('Received data:', data);
                setBatteryPercentage(data.battery);
            }
            else if (receivedTopic === "esp32/debug") {
                console.log('Received data:', data);
                const timestamp = new Date().toLocaleTimeString();
                handleDebugMessage({ timestamp, text: data.message });
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


export const sendCLICommand = (command, globalRunId) => {
    if (!client) {
        console.error('MQTT client is not initialized');
        return;
    }
    const message = JSON.stringify({
        run_id: globalRunId,
        command: command
    });
    client.publish('esp32/cli', message);
};