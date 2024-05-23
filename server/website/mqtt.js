let INTERVAL_STEERING_UPDATE = 200;
let currentSpeed = 0;
let currentAngle = 0;

const MQTT_BROKER = "18.132.10.124";
const MQTT_PORT = 8000;

// Connect to the MQTT broker via WebSockets
const client = mqtt.connect(`ws://${MQTT_BROKER}:${MQTT_PORT}`, {
    reconnectPeriod: 1000, // Reconnect after 1 second
    connectTimeout: 30 * 1000, // 30 seconds timeout for connecting
    keepalive: 60 // 60 seconds keep-alive ping
});

client.on('connect', function () {
    console.log('Connected to MQTT broker');

    // Subscribe to the topic
    client.subscribe('esp32/send', function (err) {
        if (!err) {
            client.publish('esp32/receive', 'Hello from MQTT over WebSockets!');
        }
    });
    client.subscribe('esp32/battery');
    client.subscribe('esp32/mapping');
});

client.on('message', function (topic, message) {
    // Message is in Buffer
    if (topic === 'esp32/battery') {
        const data = JSON.parse(message.toString());
        console.log('Received data:', data); // Log received data for debugging
        const percentage = data.battery; // Ensure correct key
        console.log('Battery percentage:', percentage); // Log percentage
        update_battery(percentage);
    }
    if (topic === 'esp32/mapping') {
        const data = JSON.parse(message.toString());
        console.log('Received data:', data); // Log received data for debugging

        // Extract the data from the message
        const timestamp = data.timestamp;
        const x = data.x;
        const y = data.y;
        const orientation = data.orientation;

        console.log('Coordinates:', x, y); // Log coordinates
        // Call a function to handle the coordinates
        handleCoordinates(x, y);
    }
    const msgString = message.toString();
    console.log(`Received message: ${msgString} on topic: ${topic}`);
});

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



// ----------------- SENDING POLAR COORDINATES TO ESP32 -----------------

function polar_coordinates(speed, angle) {
    currentSpeed = speed;
    currentAngle = angle;
}

// Publish the speed and angle to the topic at the specified interval
setInterval(() => {
    client.publish('user/joystick', JSON.stringify({ speed: currentSpeed, angle: currentAngle }));
}, INTERVAL_STEERING_UPDATE);

// Ensure polar_coordinates is globally accessible
window.polar_coordinates = polar_coordinates;