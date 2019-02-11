#ifndef BAIN_WIFI
#define BAIN_WIFI

#include <ESP8266WiFi.h>

void logIPAddress()
{
    // Print IP address
    Serial.print("IP address: ");
    Serial.println(WiFi.localIP());
}

void logMACAddress()
{
    // Print your MAC address.
    byte mac[6];
    WiFi.macAddress(mac);
    Serial.print("MAC address: ");
    Serial.print(mac[5], HEX);
    Serial.print(":");
    Serial.print(mac[4], HEX);
    Serial.print(":");
    Serial.print(mac[3], HEX);
    Serial.print(":");
    Serial.print(mac[2], HEX);
    Serial.print(":");
    Serial.print(mac[1], HEX);
    Serial.print(":");
    Serial.println(mac[0], HEX);
}

void logSignalStrenght()
{
    // Print the received signal strength.
    Serial.print("Signal strength (RSSI): ");
    Serial.println(WiFi.RSSI());
}

void logEncryption()
{
    // Print the encryption type:
    Serial.print("Encryption Type: ");
    byte encryption = WiFi.encryptionType(0);
    Serial.println(encryption, HEX);
}

void logWifiInformations()
{
    logIPAddress();
    //logMACAddress();
    logSignalStrenght();
    //logEncryption();
}

void connectWifi(const char *wifi_ssid, const char *wifi_password)
{
    if (WiFi.status() != WL_CONNECTED)
    {
        WiFi.begin(wifi_ssid, wifi_password);

        Serial.print("Connecting to: ");
        Serial.println(wifi_ssid);

        while (WiFi.status() != WL_CONNECTED)
        {
            delay(500);
            Serial.print(".");
        }
        Serial.println();
        Serial.println("Connected.");
    }
}

void disconnectWifi()
{
    WiFi.disconnect();
}

#endif