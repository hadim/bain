#ifndef BAIN_MQTT
#define BAIN_MQTT

#include <PubSubClient.h>

#include "secret.h"

const char *mqtt_server = MQTT_SERVER;
const int mqtt_port = MQTT_PORT;
const char *mqtt_username = MQTT_USERNAME;
const char *mqtt_password = MQTT_PASSWORD;

WiFiClient espClient;
PubSubClient client(espClient);

void callback(char *topic, byte *payload, unsigned int length)
{
    Serial.print("Message arrived [");
    Serial.print(topic);
    Serial.print("] ");
    for (int i = 0; i < length; i++)
    {
        Serial.print((char)payload[i]);
    }
    Serial.println();
}

void connect(char *mqtt_client_id)
{
    // Loop until we're reconnected
    while (!client.connected())
    {
        Serial.println("Attempting MQTT connection.");

        // Attempt to connect
        if (client.connect(mqtt_client_id))
        {
            Serial.println("MQTT Client connected.");
        }
        else
        {
            Serial.print("Failed, rc=");
            Serial.print(client.state());
            Serial.println("Try again in 5 seconds");

            // Wait 2 seconds before retrying
            delay(2000);
        }
    }
}

void initMQTT(char *mqtt_client_id)
{
    // Init the client.
    client.setServer(mqtt_server, mqtt_port);

    // Set a callback when a message is received.
    // Disable it since we don't need it here.
    // client.setCallback(callback);

    // Connect to the broker.
    connect(mqtt_client_id);
}

void sendMessage(String message, char *topic, char *mqtt_client_id)
{
    if (!client.connected())
    {
        connect(mqtt_client_id);
    }

    const char *msg = message.c_str();
    client.publish(topic, msg);
}

#endif