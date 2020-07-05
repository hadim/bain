#ifndef BAIN_PARAMETERS
#define BAIN_PARAMETERS

// Sensor configuration
String sensor_name = "Bain Environmental Sensor";
String sensor_manufacturer = "hadim";
String sensor_version = "1.0.0";
String device_id = "bain_living_room";

// Timezone
const int timeOffsetHours = -5;

// Delay between two measures in seconds.
const int loop_delay_s = 10;

// Enable deep sleep mode.
const boolean deep_sleep = false;

// Monitor battery level.
const boolean monitorBattery = true;

// LEDs. Disable by setting value to `-1`.
// Warning: Unused at the moment.
const int state_LED = -1;

#endif