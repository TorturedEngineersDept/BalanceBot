import mqtt from 'mqtt';

let client;
let test = 0;
let startTime = 0;

export const initializeMQTT = (setBatteryData, setPowerData, globalRunId, setRTT) => {
    const MQTT_BROKER = "18.130.87.186";
    const MQTT_PORT = 8000;

    test = globalRunId;
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

        client.subscribe('esp32/battery', (err) => {
            if (!err) {
                console.log('Subscribed to topic: esp32/battery');
            }
        });
        client.subscribe('esp32/power', (err) => {
            if (!err) {
                console.log('Subscribed to topic: esp32/power');
            }
        });
    });

    client.on('message', (topic, message) => {
        if (topic === 'esp32/battery') {
            try {
                const data = JSON.parse(message.toString());
                if (data.run_id === test) {
                    console.log('Received data:', data);
                    setBatteryData(prevData => {
                        const newData = Array.isArray(prevData)
                            ? [...prevData, { x: new Date(data.timestamp * 1000), y: data.battery }]
                            : [{ x: new Date(data.timestamp * 1000), y: data.battery }];
                        console.log('New data array:', newData);
                        return newData;
                    });
                } else {
                    console.log(`Run ID mismatch: received ${data.run_id}, expected ${globalRunId}`);
                }
            } catch (e) {
                console.error('Error parsing message:', e);
            }
        }
        if (topic === 'esp32/power') {
            try {
                const data = JSON.parse(message.toString());
                if (data.run_id === test) {
                    console.log('Received data:', data);
                    setPowerData(prevData => {
                        const newData = Array.isArray(prevData)
                            ? [...prevData, { x: new Date(data.timestamp * 1000), y: data.power }]
                            : [{ x: new Date(data.timestamp * 1000), y: data.power }];
                        console.log('New data array:', newData);
                        return newData;
                    });
                } else {
                    console.log(`Run ID mismatch: received ${data.run_id}, expected ${globalRunId}`);
                }
            } catch (e) {
                console.error('Error parsing message:', e);
            }
        }
        if (topic === 'user/RTT') {
            const endTime = Date.now();
            const rtt = endTime - startTime;
            setRTT(rtt);
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

export const sendTuning = (runId, innerKp, innerKi, innerKd, outerKp, outerKi, outerKd, rotationSetpoint, velocitySetpoint, tiltSetpoint) => {
    if (!client || !client.connected) {
        console.error('MQTT client is not initialized or connected');
        return;
    }
    const message = JSON.stringify({
        run_id: runId,
        kp_i: parseFloat(innerKp),
        ki_i: parseFloat(innerKi),
        kd_i: parseFloat(innerKd),
        kp_o: parseFloat(outerKp),
        ki_o: parseFloat(outerKi),
        kd_o: parseFloat(outerKd),
        rotation_setpoint: parseFloat(rotationSetpoint),
        velocity_setpoint: parseFloat(velocitySetpoint),
        tilt_setpoint: parseFloat(tiltSetpoint)
    });

    console.log('Publishing message:', message);
    client.publish('user/pid', message, (err) => {
        if (err) {
            console.error('Publish error:', err);
        } else {
            console.log('Message published successfully');
        }
    });
};

export const sendPingMessage = () => {
    if (!client || !client.connected) {
        console.error('MQTT client is not initialized or connected');
        return;
    }
    client.subscribe('user/RTT', (err) => {
        if (!err) {
            console.log('Subscribed to topic: user/RTT');
        }
    });
    startTime = Date.now();
    client.publish('user/RTT', JSON.stringify({ message: 'ping' }), (err) => {
        if (err) {
            console.error('Publish error:', err);
        } else {
            console.log('Ping message sent successfully');
        }
    });
    client.unsubscribe('user/RTT', (err) => {
        if (!err) {
            console.log('Unsubscribed from topic: user/RTT');
        }
    });
};
