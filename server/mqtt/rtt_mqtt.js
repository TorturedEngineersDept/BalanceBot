const mqtt = require('mqtt');

let INTERVAL_STEERING_UPDATE = 200;
let currentSpeed = 0;
let currentAngle = 0;

const MQTT_BROKER = '35.177.90.106';
const MQTT_PORT = 1883;

// Connect to the MQTT broker via WebSockets
const client = mqtt.connect(`mqtt://${MQTT_BROKER}:${MQTT_PORT}`, {
    reconnectPeriod: 1000, // Reconnect after 1 second
    connectTimeout: 30 * 1000, // 30 seconds timeout for connecting
    keepalive: 60 // 60 seconds keep-alive ping
});

const topic = 'test/topic';
const message = 'test message';
const iterations = 1000;

let sendTimes = [];
let receiveTimes = [];
let completed = 0;

client.on('connect', () => {
    console.log('Connected to MQTT broker');

    client.subscribe(topic, (err) => {
        if (err) {
            console.error('Subscription error:', err);
            process.exit(1);
        } else {
            console.log('Subscribed to topic');
            measureTime();
        }
    });
});

client.on('message', (topic, message) => {
    receiveTimes.push(Date.now());

    completed++;
    if (completed < iterations) {
        measureTime();
    } else {
        calculateAverageTime();
        client.end();
    }
});


// ----------------- CALCULATING RTT -----------------

function measureTime() {
    const sendTime = Date.now();
    sendTimes.push(sendTime);
    client.publish(topic, message);
}

function calculateAverageTime() {
    let totalLatency = 0;

    for (let i = 0; i < iterations; i++) {
        const latency = receiveTimes[i] - sendTimes[i];
        totalLatency += latency;
    }

    const averageLatency = totalLatency / iterations;
    console.log(`Average latency over ${iterations} iterations: ${averageLatency} ms`);
}


// ----------------- MAINTAINING CONNECTION -----------------

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