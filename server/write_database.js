const AWS = require('aws-sdk');
const mqtt = require('mqtt');

// Configure AWS SDK
AWS.config.update({ region: 'eu-west-2' });
const dynamoDB = new AWS.DynamoDB.DocumentClient();

// Configure MQTT client
const client = mqtt.connect('mqtt://18.130.87.186'); // Use your broker IP address

// Global variable to store the updated NB value
let globalNB = 0;

client.on('connect', () => {
    console.log('Connected to MQTT broker');
    client.subscribe(['esp32/mapping', 'esp32/status'], (err) => {
        if (err) {
            console.error('Failed to subscribe to topics', err);
        } else {
            console.log('Subscribed to topics');
        }
    });
});

client.on('message', async (topic, message) => {
    if (topic === 'esp32/mapping') {
        const data = JSON.parse(message.toString());

        const params = {
            TableName: 'Mapping',
            Item: {
                RunNb: globalNB, // Set the appropriate RunNb from globalNB
                Timestamp: data.timestamp, // Ensure case matches DynamoDB schema
                x: data.x,
                y: data.y,
                orientation: data.orientation
            },
        };

        try {
            await dynamoDB.put(params).promise();
            console.log('Data inserted successfully');
        } catch (error) {
            console.error('Could not insert data', error);
        }
    } else if (topic === 'esp32/status') {
        const data = JSON.parse(message.toString());
        console.log('Received message on esp32/status!', data);

        // Retrieve the current NB value from DynamoDB
        const getNBParams = {
            TableName: 'Status',
            Key: {
                'Id': 1 // Assuming Id 1 holds the NB value
            }
        };

        try {
            const nbResult = await dynamoDB.get(getNBParams).promise();
            let currentNB = 0;

            if (nbResult.Item && nbResult.Item.NB !== undefined) {
                currentNB = nbResult.Item.NB;
            }

            // Update NB by adding 1
            globalNB = currentNB + 1;
            console.log('Updated NB:', globalNB);

            // Save the new NB value back to DynamoDB
            const updateNBParams = {
                TableName: 'Status',
                Key: {
                    'Id': 1 // Assuming Id 1 holds the NB value
                },
                UpdateExpression: 'set NB = :newNB',
                ExpressionAttributeValues: {
                    ':newNB': globalNB
                }
            };

            await dynamoDB.update(updateNBParams).promise();
            console.log('NB value updated in DynamoDB');

        } catch (error) {
            console.error('Error retrieving or updating NB value:', error);
        }
    }
});

client.on('error', (error) => {
    console.error('MQTT Client Error:', error);
});
