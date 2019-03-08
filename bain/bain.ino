#include "parameters.h"
#include "secret.h"
#include "wifi.h"
#include "sensor.h"
#include "mqtt.h"
#include "ntp.h"
#include "battery.h"

#include <ArduinoJson.h>

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

    // Connect to WiFi and print some informations
    // about the connection.
    connectWifi(wifi_ssid, wifi_password);
    logWifiInformations();

    // Init BME280 sensor.
    initBME280Sensor();

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

    // Convert values to JSON string.
    String message = packToJSON(timestamp, sensorValues, batteryLevel);

    // Send JSON to MQTT broker.
    sendMessage(message, true);
}

String packToJSON(String timestamp, SensorValues sensorValues, BatteryLevel batteryLevel)
{
    // Create JSON object.
    const size_t capacity = JSON_OBJECT_SIZE(7) + 90;
    DynamicJsonBuffer jsonBuffer(capacity);
    JsonObject &root = jsonBuffer.createObject();

    if (sensorValues._valid)
    {
        root["temperature"] = sensorValues.temperature;
        root["pressure"] = sensorValues.pressure;
        root["humidity"] = sensorValues.humidity;
    }
    else
    {
        root["temperature"] = "";
        root["pressure"] = "";
        root["humidity"] = "";
    }

    if (batteryLevel._valid)
    {
        root["batteryLevel"] = batteryLevel.level;
        root["batteryCharging"] = batteryLevel.isCharging;
        root["batteryVoltage"] = batteryLevel.realVoltage;
    }
    else
    {
        root["batteryLevel"] = "";
        root["batteryCharging"] = "";
        root["batteryVoltage"] = "";
    }

    const char *timestampChar = timestamp.c_str();
    root["timestamp"] = timestampChar;

    String message = "";
    root.printTo(message);
    return message;
}