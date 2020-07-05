#include "parameters.h"
#include "secret.h"
#include "wifi.h"
#include "sensor.h"
#include "mqtt.h"
#include "ntp.h"
#include "battery.h"

#include <ArduinoJson.h>

// MQTT
const String mqtt_message_topic_base = MQTT_MESSAGE_TOPIC_BASE;

// WiFi
const char *wifi_ssid = WIFI_SSID;
const char *wifi_password = WIFI_PASSWORD;

// Suggested rate is 1/60Hz (1 m or 60,000 ms)
const int loop_delay_ms = loop_delay_s * 1000;

void setup()
{
    Serial.begin(115200);
    while (!Serial)
        ;

    // Clean console.
    Serial.println("");

    // Init BME280 sensor.
    initBME280Sensor();

    // Connect to WiFi and print some informations
    // about the connection.
    connectWifi(wifi_ssid, wifi_password);
    logWifiInformations();

    // Init NTP
    initNTP();

    if (deep_sleep == true)
    {
        one_step();
        disconnectMQTT();
        disconnectWifi();
        ESP.deepSleep(loop_delay_ms * 1e3);
    }
}

void loop()
{
    if (deep_sleep == false)
    {
        one_step();
        delay(loop_delay_ms);
    }
}

void one_step()
{
    // Update time if needed.
    updateNTP();

    // Check in case WiFi has been disconnected.
    connectWifi(wifi_ssid, wifi_password);

    // Get a timestamp.
    String timestamp = getTimeStampString();

    // Measure battery level
    BatteryLevel batteryLevel;
    if (monitorBattery)
    {
        batteryLevel = measureBatteryLevel();
    }

    // Get sensor values.
    //SensorValues sensorValues;
    SensorValues sensorValues = getSensorValues();

    // Send configs to MQTT broker
    sendConfig(sensorValues, batteryLevel);

    // Send state to MQTT broker
    sendState(sensorValues, batteryLevel, timestamp);
}

void sendConfig(SensorValues sensorValues, BatteryLevel batteryLevel)
{
    // Prepare message
    const size_t capacity = JSON_OBJECT_SIZE(3) + JSON_OBJECT_SIZE(10) + 360;
    DynamicJsonDocument data(capacity);

    String measurement_name = "Temperature";
    String measurement_type = "temperaturepppp";
    String measurement_unit = "°C";
    String measurement_template = "{{ value_json.temperature | round(1) }}";
    String entity_name = device_id + "_" + measurement_type;

    data["name"] = measurement_name;
    data["device_class"] = measurement_type;
    data["unit_of_measurement"] = measurement_unit;
    data["platform"] = "mqtt";
    data["force_update"] = true;
    data["expire_after"] = 0;

    // Example: "/homeassistant/sensor/bain/living_room/state"
    data["state_topic"] = mqtt_message_topic_base + "/" + device_id + "/state";

    // Example: "{{ value_json.temperature }}"
    data["value_template"] = measurement_template;

    // Example: "/bain/living_room/temperature"
    data["unique_id"] = entity_name;

    JsonObject device = data.createNestedObject("device");
    device["name"] = sensor_name;
    device["manufacturer"] = sensor_manufacturer;
    device["identifiers"] = device_id;
    device["sw_version"] = sensor_version;

    // Send message

    // Example: "/bain/living_room/temperature"
    String message_topic = mqtt_message_topic_base + "/" + device_id;
    message_topic = message_topic + "/" + entity_name + "/config";

    String message = "";
    serializeJson(data, message);
    sendMessage(message, message_topic, true);
}

void sendState(SensorValues sensorValues, BatteryLevel batteryLevel, String timestamp)
{
    // Prepare message
    const size_t capacity = JSON_OBJECT_SIZE(7) + 90;
    DynamicJsonDocument data(capacity);

    if (sensorValues._valid)
    {
        data["temperature"] = sensorValues.temperature;
        data["pressure"] = sensorValues.pressure;
        data["humidity"] = sensorValues.humidity;
    }
    else
    {
        data["temperature"] = "";
        data["pressure"] = "";
        data["humidity"] = "";
    }

    if (batteryLevel._valid)
    {
        data["batteryLevel"] = batteryLevel.level;
        data["batteryCharging"] = batteryLevel.isCharging;
        data["batteryVoltage"] = batteryLevel.realVoltage;
    }
    else
    {
        data["batteryLevel"] = "";
        data["batteryCharging"] = "";
        data["batteryVoltage"] = "";
    }

    const char *timestampChar = timestamp.c_str();
    data["timestamp"] = timestampChar;

    // Send message

    // Example: "/bain/living_room/temperature"
    String message_topic = mqtt_message_topic_base + "/" + device_id + "/state";

    String message = "";
    serializeJson(data, message);
    sendMessage(message, message_topic, true);
}