#ifndef BAIN_MQTT
#define BAIN_MQTT

#include <PubSubClient.h>

#include "secret.h"

const char *mqtt_server = MQTT_SERVER;
const int mqtt_port = MQTT_PORT;
const char *mqtt_username = MQTT_USERNAME;
const char *mqtt_password = MQTT_PASSWORD;
const char *mqtt_client_id = MQTT_CLIENT_ID;
const char *mqtt_message_topic = MQTT_MESSAGE_TOPIC;

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

void connect()
{
    // Loop until we're reconnected
    while (!client.connected())
    {
        Serial.println("Attempting MQTT connection.");

        // Attempt to connect
        boolean isConnected;
        if (strlen(mqtt_username) == 0 || strlen(mqtt_password) == 0)
        {
            isConnected = client.connect(mqtt_client_id);
        }
        else
        {
            isConnected = client.connect(mqtt_client_id, mqtt_username, mqtt_password);
        }

        if (isConnected)
        {
            Serial.println("MQTT Client connected.");
        }
        else
        {
            Serial.print("Failed, rc=");
            Serial.print(client.state());
            Serial.println("Try again in 2 seconds.");

            // Wait 2 seconds before retrying
            delay(2000);
        }
    }
}

void initMQTT()
{
    // Init the client.
    client.setServer(mqtt_server, mqtt_port);

    // Set a callback when a message is received.
    // Disable it since we don't need it here.
    // client.setCallback(callback);
}

void sendMessage(String message, boolean printValues)
{
    if (!client.connected())
    {
        connect();
    }

    const char *msg = message.c_str();
    client.publish(mqtt_message_topic, msg);
    delay(100);
    client.flush();

    if (printValues == true)
    {
        Serial.println(msg);
    }
}

#endif