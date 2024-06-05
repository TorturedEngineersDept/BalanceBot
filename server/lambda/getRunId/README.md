# Attributing unique RunID and BotID

The code in this folder ensures that each bot is attributed a unique BotID. This is done by inspecting the ESP32's MAC addresses.
Unique RunIDs are also attributed by maintaining a counter within the `identifier` table

The lambda function is triggered by an HTML GET response with argument the MAC address.

``` HTML
https://rts358y5pk.execute-api.eu-west-2.amazonaws.com/prod/get-runid-esp32?mac=30:C9:22:11:E7:64
```

Response has format:

``` JSON
response =  {
    RunId: integer, 
    BotId: integer
}
```

The lambda function is triggered in the MqttSetup::resolveId() within WifiSetup::WifiSetup.

The lambda function should be granted enough IAM privileges to write and read in DynamoDB.

CORS should also be bypassed within AWS API GATEWAY.