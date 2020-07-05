#ifndef BAIN_PARAMETERS
#define BAIN_PARAMETERS

// Timezone
const int timeOffsetHours = -5;

// Delay between two measures in seconds.
const int loop_delay_s = 60;

// Enable deep sleep mode.
const boolean deep_sleep = false;

// Monitor battery level.
const boolean monitorBattery = true;

// LEDs. Disable by setting value to `-1`.
// Warning: Unused at the moment.
const int state_LED = -1;

#endif