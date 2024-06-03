import mqtt from 'mqtt';

// Function to fetch initial data from API Gateway
export const fetchData = async (runId) => {
    try {
        const response = await fetch(`https://rts358y5pk.execute-api.eu-west-2.amazonaws.com/prod/get-battery?RunID=${runId}`);
        if (!response.ok) {
            throw new Error(`HTTP error! status: ${response.status}`);
        }
        const data = await response.json();
        return data.map(item => ({
            x: item.Timestamp,  // Using the raw Timestamp value
            y: item.Battery
        }));
    } catch (error) {
        console.error('Failed to fetch data:', error);
        throw error;
    }
};

// Function to initialize MQTT client
export const initializeMQTT = (setBatteryData) => {
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
            console.log('Received data:', data);
            const percentage = data.battery;
            console.log('Battery percentage:', percentage);
            setBatteryData(prevData => [...prevData, { x: data.timestamp, y: percentage }]);
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
};
