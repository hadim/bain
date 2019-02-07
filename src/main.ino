#include <NTPClient.h>
#include <WiFiUdp.h>

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

// Timezone
const int timeOffsetHours = -5;

// Suggested rate is 1/60Hz (1 m or 60,000 ms)
const int loop_delay_ms = 1000;

// LEDs. Disable by setting vaue to `-1`.
int wifi_LED = 11;
int sensor_LED = 12;
int mqtt_LED = 13;
int ntp_LED = -1;

// By default 'pool.ntp.org' is used with 60 seconds
// update interval and no offset
WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP);

void setup()
{
  Serial.begin(115200);
  delay(100);

  // Setup LEDs
  if (wifi_LED != -1) pinMode(wifi_LED, OUTPUT);
  if (sensor_LED != -1) pinMode(sensor_LED, OUTPUT);
  if (mqtt_LED != -1) pinMode(mqtt_LED, OUTPUT);
  if (ntp_LED != -1) pinMode(ntp_LED, OUTPUT);

  // Set LEDs to low.
  if (wifi_LED != -1) digitalWrite(wifi_LED, LOW);
  if (sensor_LED != -1) digitalWrite(sensor_LED, LOW);
  if (mqtt_LED != -1) digitalWrite(mqtt_LED, LOW);
  if (ntp_LED != -1) digitalWrite(ntp_LED, LOW);

  // Connect to WiFi and print some informations
  // about the connection.
  connectWifi(wifi_ssid, wifi_password);
  logWifiInformations();
  if (wifi_LED != -1) digitalWrite(wifi_LED, HIGH);

  // Init BME280 sensor.
  initBME280Sensor();
  if (sensor_LED != -1) digitalWrite(sensor_LED, HIGH);

  // Init MQTT Client.
  initMQTT(mqtt_client_id);
  if (mqtt_LED != -1) digitalWrite(mqtt_LED, HIGH);

  // Init NTP Client.
  timeClient.begin();

  // Set the timezone.
  timeClient.setTimeOffset(timeOffsetHours * 3600);

  // Set time update interval to 6 hours.
  timeClient.setUpdateInterval(6 * 3600);
  if (ntp_LED != -1) digitalWrite(ntp_LED, HIGH);
}

void loop()
{
  // Update time if needed.
  timeClient.update();

  // Check in case WiFi has been disconnected.
  connectWifi(wifi_ssid, wifi_password);

  // Trigger measure on the sensor.
  sensorMeasure();

  // Print sensor values (can be disabled when in production).
  logSensorValues();

  // Get sensor values as JSON string.
  String timestamp = getTimeStampString();
  String message = getValuesAsJSONString(timestamp);

  // Send JSON to MQTT broker.
  sendMessage(message, "bain/sensor1", mqtt_client_id);

  // Delay in ms in between two data acquisition.
  delay(loop_delay_ms);
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
