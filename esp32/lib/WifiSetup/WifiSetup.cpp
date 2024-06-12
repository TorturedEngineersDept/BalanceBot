#include "WifiSetup.h"
#include "ESP32Ping.h"

WifiSetup::WifiSetup(
    const char *ssid,
    const char *password,
    const char *mqtt_server,
    int mqtt_port)
    : ssid(ssid),
      username(nullptr),
      password(password),
      timeClient(ntpUDP, "pool.ntp.org", 0, 60000),
      mqtt(mqtt_server, mqtt_port, timeClient)
{
}

WifiSetup::WifiSetup(
    const char *ssid,
    const char *username,
    const char *password,
    const char *mqtt_server,
    int mqtt_port)
    : ssid(ssid),
      username(username),
      password(password),
      timeClient(ntpUDP, "pool.ntp.org", 0, 60000),
      mqtt(mqtt_server, mqtt_port, timeClient)
{
}

void WifiSetup::connect(unsigned long timeout)
{
    delay(10);
    // Start connecting to WiFi network
    Serial.println();
    Serial.print("Connecting to ");
    Serial.println(ssid);

    if (username != nullptr)
    {
        WiFi.disconnect();
        WiFi.mode(WIFI_STA);
        WiFi.begin(ssid, WPA2_AUTH_PEAP, username, username, password);
    }
    else
    {
        WiFi.begin(ssid, password);
    }

    unsigned long start = 0;
    uint32_t d = 500;

    while (WiFi.status() != WL_CONNECTED && start < timeout)
    {
        delay(d);
        Serial.print(".");
        start += d;
    }

    Serial.println("");
    Serial.println("WiFi connected");
    Serial.println("IP address: ");
    Serial.println(WiFi.localIP());

    // Get the WiFi signal strength
    getStrength();

    // Ping the MQTT server to ensure it's reachable
    pingServer(mqtt.getServer());

    // Setup MQTT
    mqtt.setCallback(callback);
    mqtt.connect(timeout - start);

    if (!runIdResolved)
    {
        resolveId();
        runIdResolved = true;
    }

    // Setup NTP
    timeClient.begin();
}

void WifiSetup::getStrength() const
{
    Serial.print("Signal strength (RSSI): ");
    Serial.println(WiFi.RSSI());
}

int WifiSetup::pingServer(const char *server) const
{
    IPAddress ip;
    if (!WiFi.hostByName(server, ip))
    {
        Serial.println("DNS lookup failed. Unable to resolve the MQTT broker.");
        return 1;
    }

    Serial.print("Pinging MQTT broker at ");
    Serial.println(ip);

    if (Ping.ping(ip))
    {
        Serial.println("Ping successful.");
        return 0;
    }

    Serial.println("Ping failed.");

    // Test ping to Google DNS
    Serial.println("Trying ping to Google server");
    if (Ping.ping(IPAddress(8, 8, 8, 8)))
    {
        Serial.println("Ping to 8.8.8.8 successful.");
        return 1;
    }

    Serial.println("Ping to 8.8.8.8 failed.");
    return 2;
}

bool WifiSetup::mqttConnected()
{
    return mqtt.isConnected();
}

void WifiSetup::loop()
{
    mqtt.loop();
    delay(100); // <- fixes some issues with WiFi stability
}

void WifiSetup::print(const char *message)
{
    if (mqttConnected())
    {
        DebugMessage debugMessage(message, mqtt.getEpochTime());
        mqtt.publishMessage(debugMessage);
        delay(100);
    }
    Serial.print(message);
}

void WifiSetup::println(const char *message)
{
    if (mqttConnected())
    {
        std::string msg = std::string(message) + "\n";
        DebugMessage debugMessage(msg.c_str(), mqtt.getEpochTime());
        mqtt.publishMessage(debugMessage);
        delay(100);
    }
    Serial.println(message);
}

void WifiSetup::callback(char *topic, byte *payload, unsigned int length)
{
    Serial.print("Message arrived [");
    Serial.print(topic);
    Serial.print("] ");

    // Create a buffer to store the payload
    char payloadStr[length + 1];
    memcpy(payloadStr, payload, length);
    payloadStr[length] = '\0'; // Null-terminate the string

    Serial.println(payloadStr);

    // Parse the JSON
    StaticJsonDocument<200> doc;
    DeserializationError error = deserializeJson(doc, payloadStr);

    if (error)
    {
        Serial.print("Failed to parse JSON: ");
        Serial.println(error.c_str());
        return;
    }

    if (doc["run_id"] == RunID)
    {
        // Check the topic and process accordingly
        Serial.println("Processing message...");
        if (strcmp(topic, "user/joystick") == 0)
        {
            // Extract values from the JSON document
            float speed = 100; // Being overwritten in PidController:StabilisedLoop for some reason
            const char *speedStr = doc["direction"];

            KeyDirection key_dir;
            switch (speedStr[0])
            {
            case 'A':
                key_dir = KeyDirection::LEFT;
                break;
            case 'D':
                key_dir = KeyDirection::RIGHT;
                break;
            case 'W':
                key_dir = KeyDirection::FORWARD;
                break;
            case 'S':
                key_dir = KeyDirection::BACKWARD;
                break;
            default:
                key_dir = KeyDirection::STOP;
            }

            Serial.println("Speed: " + String(speed) + ", key_dir: " + String(key_dir));

            PidController::setDirection(PidDirection(speed, key_dir));
        }
        else if (strcmp(topic, "user/pid") == 0)
        {
            // Get previous coefficients
            PidParams params = PidController::getParams();

            // Extract values from the JSON document
            float kp_i = doc["kp_i"] | params.kp_i;
            float ki_i = doc["ki_i"] | params.ki_i;
            float kd_i = doc["kd_i"] | params.kd_i;
            float setpoint_i = doc["setpoint_i"] | params.tilt_setpoint;
            float kp_o = doc["kp_o"] | params.kp_o;
            float ki_o = doc["ki_o"] | params.ki_o;
            float kd_o = doc["kd_o"] | params.kd_o;
            float tilt_setpoint = doc["tilt_setpoint"] | params.tilt_setpoint;
            float velocity_setpoint = doc["velocity_setpoint"] | params.velocity_setpoint;
            float rotation_setpoint = doc["rotation_setpoint"] | params.rotation_setpoint;
            Serial.println("kp_i: " + String(kp_i) +
                           ", ki_i: " + String(ki_i) +
                           ", kd_i: " + String(kd_i) +
                           ", tilt_setpoint: " + String(tilt_setpoint) +
                           ", kp_o: " + String(kp_o) +
                           ", ki_o: " + String(ki_o) +
                           ", kd_o: " + String(kd_o) +
                           ", velocity_setpoint: " + String(velocity_setpoint) +
                           ", rotation_setpoint: " + String(rotation_setpoint));

            // Use the callback given in the static class
            PidController::setParams(PidParams(kp_i, ki_i, kd_i, tilt_setpoint, kp_o, ki_o, kd_o, velocity_setpoint, comp_coeff));
        }
        else
        {
            Serial.println("Unsupported topic");
        }
    }
}

void WifiSetup::resolveId()
{
    HTTPClient http;
    String baseApiEndpoint = "https://rts358y5pk.execute-api.eu-west-2.amazonaws.com/prod/get-runid-esp32?mac=";

    // Construct the full API endpoint URL with the MAC address
    String macAddress = WiFi.macAddress();
    String apiEndpoint = String(baseApiEndpoint) + macAddress;

    http.begin(apiEndpoint);

    // Trigger Lambda function
    int httpResponseCode = http.GET();

    while (httpResponseCode != 200)
    {
        httpResponseCode = http.GET();
    }

    String response = http.getString();
    http.end();

    // Parse the JSON response
    StaticJsonDocument<200> doc;
    DeserializationError error = deserializeJson(doc, response);
    BotID = doc["BotId"];
    RunID = doc["RunId"];

    Serial.print("BotId: ");
    Serial.println(BotID);
    Serial.print("RunId: ");
    Serial.println(RunID);
}

NTPClient &WifiSetup::getNTPClient()
{
    return timeClient;
}