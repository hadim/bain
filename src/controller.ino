#include "secret.h"
#include "wifi.h"
#include "sensor.h"

const char *ssid = WIFI_SSID;
const char *password = WIFI_PASSWORD;

// Suggested rate is 1/60Hz (1 m or 60,000 ms)
const int loop_delay_ms = 1000;

void setup()
{
  Serial.begin(115200);
  delay(100);

  // Connect to WiFi and print some informations
  // about the connection.
  connectWifi(ssid, password);
  logWifiInformations();

  // Init BME280 sensor.
  initBME280Sensor();
}

void loop()
{
  // Delay in ms in between two data acquisition.
  delay(loop_delay_ms);

  // Check in case WiFi has been disconnected.
  connectWifi(ssid, password);

  // Trigger measure on the sensor.
  sensorMeasure();

  // Print sensor values (can be disabled when in production).
  logSensorValues();
}
