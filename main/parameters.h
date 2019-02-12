#ifndef BAIN_PARAMETERS
#define BAIN_PARAMETERS

// LEDs. Disable by setting vaue to `-1`.
// Warning: Unused at the moment.
const int state_LED = -1;

// Timezone
const int timeOffsetHours = -5;

// Delay between two measures in seconds.
const int loop_delay_s = 5 * 60;

// Enable deep sleep
const boolean deep_sleep = true;

#endif