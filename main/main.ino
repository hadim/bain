#include "parameters.h"
#include "secret.h"
#include "wifi.h"
#include "sensor.h"
#include "mqtt.h"
#include "ntp.h"

// Pin to read voltage.
#define VBATPIN A0

// WiFi
const char *wifi_ssid = WIFI_SSID;
const char *wifi_password = WIFI_PASSWORD;

// Suggested rate is 1/60Hz (1 m or 60,000 ms)
const int loop_delay_ms = loop_delay_s * 1000;

void setup()
{
  Serial.begin(115200);
  while (!Serial);

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

  // Trigger measure on the sensor.
  sensorMeasure();

  // Get sensor values as JSON string.
  String timestamp = getTimeStampString();
  String message = getValuesAsJSONString(timestamp, getBatteryVoltage());

  // Send JSON to MQTT broker.
  sendMessage(message, true);
}

float getBatteryVoltage()
{

  float measuredvbat = analogRead(VBATPIN);

  measuredvbat *= 2;    // we divided by 2, so multiply back
  measuredvbat *= 3.3;  // Multiply by 3.3V, our reference voltage
  measuredvbat /= 1024; // convert to voltage

  return measuredvbat;
}
