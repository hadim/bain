#include "secret.h"
#include "wifi.h"
#include "sensor.h"
#include "mqtt.h"

// WiFi
const char *wifi_ssid = WIFI_SSID;
const char *wifi_password = WIFI_PASSWORD;

// MQTT
char *mqtt_client_id = "sensor1";
char *mqtt_message_topic = "bain_sensor/sensor1";

// Suggested rate is 1/60Hz (1 m or 60,000 ms)
const int loop_delay_ms = 1000;

void setup()
{
  Serial.begin(115200);
  delay(100);

  // Connect to WiFi and print some informations
  // about the connection.
  connectWifi(wifi_ssid, wifi_password);
  logWifiInformations();

  // Init BME280 sensor.
  initBME280Sensor();

  // Init MQTT Client
  initMQTT(mqtt_client_id);
}

void loop()
{
  // Delay in ms in between two data acquisition.
  delay(loop_delay_ms);

  // Check in case WiFi has been disconnected.
  connectWifi(wifi_ssid, wifi_password);

  // Trigger measure on the sensor.
  sensorMeasure();

  // Print sensor values (can be disabled when in production).
  logSensorValues();

  // Get sensor values as JSON string.
  String message = getValuesAsJSONString();

  // Send JSON to MQTT broker.
  sendMessage(message, "bain/sensor1", mqtt_client_id);
}
