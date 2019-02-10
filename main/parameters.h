#ifndef PARAMETERS_SECRET
#define PARAMETERS_SECRET

// MQTT
char *mqtt_client_id = "sensor2";
char *mqtt_message_topic = "bain_sensor/sensor2";

// LEDs. Disable by setting vaue to `-1`.
int state_LED = -1;

// Timezone
const int timeOffsetHours = -5;

// Delay between two measures in seconds.
const int loop_delay_s = 60 * 2;

#endif