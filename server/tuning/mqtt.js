// MQTT connection configuration
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

    // Subscribe to the topics
    client.subscribe('user/pid', function (err) {
        if (!err) {
            console.log('Subscribed to user/pid');
        }
    });
    client.subscribe('tuning/graphing', function (err) {
        if (!err) {
            console.log('Subscribed to tuning/graphing');
        }
    });
});

client.on('message', function (topic, message) {
    console.log('Received message:', message.toString());

    const data = JSON.parse(message.toString());

    if (topic === "user/pid") {
        addRowToTables(data);
    }

    if (topic === 'tuning/graphing') {
        const timestamp = data.timestamp;
        const tilt = data.tilt;
        const velocity = data.velocity;

        // Inner loop chart
        innerLoopChart.data.labels.push(timestamp);
        innerLoopChart.data.datasets[0].data.push(tilt);
        if (innerLoopChart.data.labels.length > 30) {
            innerLoopChart.data.labels.shift();
            innerLoopChart.data.datasets[0].data.shift();
        }
        innerLoopChart.update('none');

        // Outer loop chart
        outerLoopChart.data.labels.push(timestamp);
        outerLoopChart.data.datasets[0].data.push(velocity);
        if (outerLoopChart.data.labels.length > 30) {
            outerLoopChart.data.labels.shift();
            outerLoopChart.data.datasets[0].data.shift();
        }
        outerLoopChart.update('none');
    }
});

// Handle connection events
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

// Initialize old values
let oldKP_i = 0;
let oldKD_i = 0;
let oldKI_i = 0;
let oldSetpoint_i = 0;
let oldKP_o = 0;
let oldKD_o = 0;
let oldKI_o = 0;
let oldSetpoint_o = 0;

let newKP_i = 0;
let newKD_i = 0;
let newKI_i = 0;
let newSetpoint_i = 0;
let newKP_o = 0;
let newKD_o = 0;
let newKI_o = 0;
let newSetpoint_o = 0;

function handleSubmitInner(event) {
    event.preventDefault(); // Prevent the form from submitting the traditional way

    // Get values from the input fields
    const kp_i = document.getElementById('kp_i').value;
    const kd_i = document.getElementById('kd_i').value;
    const ki_i = document.getElementById('ki_i').value;
    const setpoint_i = document.getElementById('setpoint_i').value;

    // Check if the input fields are empty, if so, keep the old values
    const newKP_i = kp_i !== "" ? kp_i : oldKP_i;
    const newKD_i = kd_i !== "" ? kd_i : oldKD_i;
    const newKI_i = ki_i !== "" ? ki_i : oldKI_i;
    const newSetpoint_i = setpoint_i !== "" ? setpoint_i : oldSetpoint_i;

    // Update the old values
    oldKP_i = newKP_i;
    oldKD_i = newKD_i;
    oldKI_i = newKI_i;
    oldSetpoint_i = newSetpoint_i;

    // Create a JSON object
    const data = {
        kp_i: newKP_i,
        kd_i: newKD_i,
        ki_i: newKI_i,
        setpoint_i: newSetpoint_i,
        kp_o: oldKP_o,
        kd_o: oldKD_o,
        ki_o: oldKI_o,
        setpoint_o: oldSetpoint_o
    };

    // Convert JSON object to a string and log it to the console
    console.log(JSON.stringify(data));

    // Publish the JSON data to the MQTT topic
    client.publish('user/pid', JSON.stringify(data));
}

function handleSubmitOuter(event) {
    event.preventDefault(); // Prevent the form from submitting the traditional way

    // Get values from the input fields
    const kp_o = document.getElementById('kp_o').value;
    const kd_o = document.getElementById('kd_o').value;
    const ki_o = document.getElementById('ki_o').value;
    const setpoint_o = document.getElementById('setpoint_o').value;

    // Check if the input fields are empty, if so, keep the old values
    const newKP_o = kp_o !== "" ? kp_o : oldKP_o;
    const newKD_o = kd_o !== "" ? kd_o : oldKD_o;
    const newKI_o = ki_o !== "" ? ki_o : oldKI_o;
    const newSetpoint_o = setpoint_o !== "" ? setpoint_o : oldSetpoint_o;

    // Update the old values
    oldKP_o = newKP_o;
    oldKD_o = newKD_o;
    oldKI_o = newKI_o;
    oldSetpoint_o = newSetpoint_o;

    // Create a JSON object
    const data = {
        kp_i: oldKP_i,
        kd_i: oldKD_i,
        ki_i: oldKI_i,
        setpoint_i: oldSetpoint_i,
        kp_o: newKP_o,
        kd_o: newKD_o,
        ki_o: newKI_o,
        setpoint_o: newSetpoint_o
    };

    // Convert JSON object to a string and log it to the console
    console.log(JSON.stringify(data));

    // Publish the JSON data to the MQTT topic
    client.publish('user/pid', JSON.stringify(data));
}

function addRowToTables(data) {
    const tableInner = document.getElementById('pidTableInner').getElementsByTagName('tbody')[0];
    const newRowInner = tableInner.insertRow(0);

    const kp_iCell = newRowInner.insertCell(0);
    const kd_iCell = newRowInner.insertCell(1);
    const ki_iCell = newRowInner.insertCell(2);
    const setpoint_iCell = newRowInner.insertCell(3);

    kp_iCell.textContent = data.kp_i;
    kd_iCell.textContent = data.kd_i;
    ki_iCell.textContent = data.ki_i;
    setpoint_iCell.textContent = data.setpoint_i;

    const tableOuter = document.getElementById('pidTableOuter').getElementsByTagName('tbody')[0];
    const newRowOuter = tableOuter.insertRow(0);

    const kp_oCell = newRowOuter.insertCell(0);
    const kd_oCell = newRowOuter.insertCell(1);
    const ki_oCell = newRowOuter.insertCell(2);
    const setpoint_oCell = newRowOuter.insertCell(3);

    kp_oCell.textContent = data.kp_o;
    kd_oCell.textContent = data.kd_o;
    ki_oCell.textContent = data.ki_o;
    setpoint_oCell.textContent = data.setpoint_o;
}


const innerLoopCtx = document.getElementById('innerLoopGraph').getContext('2d');
const outerLoopCtx = document.getElementById('outerLoopGraph').getContext('2d');

const innerLoopChart = new Chart(innerLoopCtx, {
    type: 'line',
    data: {
        labels: [],
        datasets: [{
            label: 'Tilt',
            data: [],
            borderColor: 'rgba(75, 192, 192, 1)',
            borderWidth: 1,
            fill: false
        }]
    },
    options: {
        scales: {
            x: {
                type: 'linear',
                position: 'bottom',
                title: {
                    display: true,
                    text: 'Timestamp'
                }
            },
            y: {
                title: {
                    display: true,
                    text: 'Tilt'
                },
                suggestedMin: -1,
                suggestedMax: 1
            }
        }
    }
});

const outerLoopChart = new Chart(outerLoopCtx, {
    type: 'line',
    data: {
        labels: [],
        datasets: [{
            label: 'Velocity',
            data: [],
            borderColor: 'rgba(153, 102, 255, 1)',
            borderWidth: 1,
            fill: false
        }]
    },
    options: {
        scales: {
            x: {
                type: 'linear',
                position: 'bottom',
                title: {
                    display: true,
                    text: 'Timestamp'
                }
            },
            y: {
                title: {
                    display: true,
                    text: 'Velocity'
                },
                // TODO: Change this for axis
                // suggestedMin: -,
                // suggestedMax: 10
            }
        }
    }
});