#ifndef PARAMETERS_SECRET
#define PARAMETERS_SECRET

// MQTT
char *mqtt_client_id = "sensor1";
char *mqtt_message_topic = "bain_sensor/sensor1";

// LEDs. Disable by setting vaue to `-1`.
int state_LED = 0;

// Timezone
const int timeOffsetHours = -5;

// Delay between two measures in seconds.
const int loop_delay_s = 60 * 2;

#endif