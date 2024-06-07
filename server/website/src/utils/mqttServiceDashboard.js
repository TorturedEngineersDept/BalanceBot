// src/utils/mqttServiceDashboard.js
import mqtt from 'mqtt';

let client;

export const initializeMQTT = (setBatteryPercentage, setBatteryData, globalRunId) => {
    const MQTT_BROKER = "18.130.87.186";
    const MQTT_PORT = 8000;

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


export const sendTuning = (runId, innerKp, innerKi, innerKd, outerKp, outerKi, outerKd, compCoeff, velocitySetpoint, tiltSetpoint) => {
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
        comp_coeff: parseFloat(compCoeff),
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