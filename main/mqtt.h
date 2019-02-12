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

void connect()
{
    client.setServer(mqtt_server, mqtt_port);

    // MQTT Client parameters
    const char *willTopic = 0;
    const uint8_t willQos = 1;
    const boolean willRetain = true;
    const char *willMessage = 0;
    const boolean cleanSession = true;

    // Loop until we're reconnected
    while (!client.connected())
    {
        // Attempt to connect
        boolean isConnected;
        if (strlen(mqtt_username) == 0 || strlen(mqtt_password) == 0)
        {
            isConnected = client.connect(mqtt_client_id, NULL, NULL, willTopic, willQos, willRetain, willMessage, cleanSession);
        }
        else
        {
            isConnected = client.connect(mqtt_client_id, mqtt_username, mqtt_password, willTopic, willQos, willRetain, willMessage, cleanSession);
        }

        if (isConnected)
        {
            Serial.println("[MQTT] - Client connected.");
        }
        else
        {
            Serial.print("[MQTT] - Failed, rc=");
            Serial.print(client.state());
            Serial.println("[MQTT] - Try again in 2 seconds.");

            // Wait 2 seconds before retrying
            delay(2000);
        }
    }

    espClient.setSync(true);
}

void sendMessage(String message, boolean printValues)
{
    if (!client.connected())
    {
        connect();
    }

    const char *msg = message.c_str();

    int messageSentCode;
    const int maximum_try = 10;
    int i = 0;
    const boolean retain = true;

    messageSentCode = client.publish(mqtt_message_topic, msg, retain);
    client.flush();
    delay(500);

    while ((messageSentCode != 1) && (i <= maximum_try))
    {
        Serial.println("[MQTT] - Sending message failed. Trying again");
        Serial.print("[MQTT] - Returned code: ");
        Serial.println(messageSentCode);
        delay(2000);

        messageSentCode = client.publish(mqtt_message_topic, msg, retain);
        client.flush();
        i++;
    }

    Serial.println(messageSentCode);

    if (printValues == true)
    {
        Serial.print("[MQTT] - Message sent to topic: ");
        Serial.println(mqtt_message_topic);
        Serial.println(msg);
    }
}

void disconnectMQTT()
{
    client.disconnect();
}

#endif