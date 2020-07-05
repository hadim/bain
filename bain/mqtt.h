#ifndef BAIN_MQTT
#define BAIN_MQTT

#define MQTT_MAX_PACKET_SIZE 2048
#include "PubSubClient.h"

#include "secret.h"

const char *mqtt_server = MQTT_SERVER;
const int mqtt_port = MQTT_PORT;
const char *mqtt_username = MQTT_USERNAME;
const char *mqtt_password = MQTT_PASSWORD;
const char *mqtt_client_id = MQTT_CLIENT_ID;

WiFiClient espClient;
PubSubClient client(espClient);

boolean connectMQTT()
{
    if (!client.connected())
    {
        client.setServer(mqtt_server, mqtt_port);
    }

    // MQTT Client parameters
    const char *willTopic = 0;
    const uint8_t willQos = 0;
    const boolean willRetain = false;
    const char *willMessage = 0;
    const boolean cleanSession = true;

    const int maximum_try = 10;
    int i = 0;
    boolean isConnected = false;

    // Loop until we're reconnected
    while (!client.connected() && (i <= maximum_try))
    {
        // Attempt to connect
        if (strlen(mqtt_username) == 0 || strlen(mqtt_password) == 0)
        {
            isConnected = client.connect(mqtt_client_id, NULL, NULL, willTopic, willQos, willRetain, willMessage, cleanSession);
        }
        else
        {
            isConnected = client.connect(mqtt_client_id, mqtt_username, mqtt_password, willTopic, willQos, willRetain, willMessage, cleanSession);
        }

        if (!isConnected)
        {
            Serial.print("[MQTT] - Failed, rc=");
            Serial.print(client.state());
            Serial.println("[MQTT] - Try again in 2 seconds.");

            // Wait 2 seconds before retrying
            delay(2000);
        }
        i++;
    }

    if (isConnected)
    {
        Serial.println("[MQTT] - Client connected.");
        return true;
    }
    else
    {
        Serial.println("[MQTT] - Client connection failed.");
        return false;
    }
}

boolean sendMessage(String message, String mqtt_message_topic, boolean printValues)
{
    boolean isConnected = client.connected();
    if (!isConnected)
    {
        isConnected = connectMQTT();
    }

    const char *msg = message.c_str();

    if (!isConnected)
    {
        Serial.println("[MQTT] - Message not sent.");
        Serial.println(msg);
        return false;
    }

    boolean messageSent;
    const int maximum_try = 10;
    int i = 0;
    const boolean retain = true;

    messageSent = client.publish(mqtt_message_topic.c_str(), msg, retain);

    while (!messageSent && (i <= maximum_try))
    {
        Serial.println("[MQTT] - Sending message failed. Trying again");
        delay(2000);

        connectMQTT();
        messageSent = client.publish(mqtt_message_topic.c_str(), msg, retain);
        i++;
    }

    client.flush();
    delay(100);

    if (messageSent && (printValues == true))
    {
        Serial.print("[MQTT] - Message sent to topic: ");
        Serial.println(mqtt_message_topic);
        Serial.println(msg);
        return true;
    }

    if (!messageSent)
    {
        Serial.println("[MQTT] - Message not sent.");
        Serial.println(msg);
    }

    return false;
}

void disconnectMQTT()
{
    client.disconnect();
}

#endif
