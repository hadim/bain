#include <NTPClient.h>
#include <WiFiUdp.h>

#include "parameters.h"
#include "secret.h"
#include "wifi.h"
#include "sensor.h"
#include "mqtt.h"

// WiFi
const char *wifi_ssid = WIFI_SSID;
const char *wifi_password = WIFI_PASSWORD;

// Suggested rate is 1/60Hz (1 m or 60,000 ms)
const int loop_delay_ms = loop_delay_s * 1000;

// By default 'pool.ntp.org' is used with 60 seconds
// update interval and no offset
WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP);

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

  // Init MQTT Client.
  initMQTT();

  // Init NTP Client.
  timeClient.begin();

  // Set the timezone.
  timeClient.setTimeOffset(timeOffsetHours * 3600);

  // Set time update interval to 6 hours.
  timeClient.setUpdateInterval(6 * 3600);

  Serial.println("Start the controller loop.");

  if (deep_sleep == true)
  {
    one_step();
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
  timeClient.update();

  // Check in case WiFi has been disconnected.
  connectWifi(wifi_ssid, wifi_password);

  // Trigger measure on the sensor.
  sensorMeasure();

  // Get sensor values as JSON string.
  String timestamp = getTimeStampString();
  String message = getValuesAsJSONString(timestamp);

  // Print sensor values (can be disabled when in production).
  // logSensorValues(timestamp);

  // Send JSON to MQTT broker.
  sendMessage(message, true);
}

// https://github.com/arduino-libraries/NTPClient/issues/36
String getTimeStampString()
{
  time_t rawtime = timeClient.getEpochTime();
  struct tm *ti;
  ti = localtime(&rawtime);

  uint16_t year = ti->tm_year + 1900;
  String yearStr = String(year);

  uint8_t month = ti->tm_mon + 1;
  String monthStr = month < 10 ? "0" + String(month) : String(month);

  uint8_t day = ti->tm_mday;
  String dayStr = day < 10 ? "0" + String(day) : String(day);

  uint8_t hours = ti->tm_hour;
  String hoursStr = hours < 10 ? "0" + String(hours) : String(hours);

  uint8_t minutes = ti->tm_min;
  String minuteStr = minutes < 10 ? "0" + String(minutes) : String(minutes);

  uint8_t seconds = ti->tm_sec;
  String secondStr = seconds < 10 ? "0" + String(seconds) : String(seconds);

  return yearStr + "-" + monthStr + "-" + dayStr + " " +
         hoursStr + ":" + minuteStr + ":" + secondStr;
}
