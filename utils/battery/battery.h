// Code inspired from https://github.com/lobeck/adafruit-feather-huzzah-8266-battery-monitor/blob/0334424f507455439cae621d935adbde878ce022/main.ino

#ifndef BAIN_BATTERY
#define BAIN_BATTERY

struct BatteryLevel
{
    int rawLevel;
    int level;
    int realVoltage;
    bool isCharging;
    //Only for internal use.
    bool _valid = false;
};

BatteryLevel measureBatteryLevel()
{
    BatteryLevel batteryLevel;

    // Battery monitor parameters
    int min_voltage = 500;
    int max_voltage = 649;
    float r1 = 47000;
    float r2 = 10000;

    // The 10kΩ/47kΩ voltage divider reduces the voltage, so the ADC Pin can handle it.
    // According to Wolfram Alpha, this results in the following values:
    // 10kΩ / (47kΩ + 10kΩ) * 5v = 0.8772v
    // 10kΩ / (47kΩ + 10kΩ) * 3.7v = 0.649v
    // 10kΩ / (47kΩ + 10kΩ) * 3.1v = 0.544v
    // * i asumed 3.1v as minimum voltage : see LiPO discharge diagrams.
    // The actual minimum i've seen was 467, which would be 2.7V immediately before automatic cutoff.
    // A measurement on the LiPo Pins directly resulted in >3.0V, so thats good to know, but no danger to the battery.

    // Get battery raw level.
    batteryLevel.rawLevel = analogRead(A0);

    // Convert battery level to percent.
    batteryLevel.level = map(batteryLevel.rawLevel, min_voltage, max_voltage, 0, 100);

    // I'd like to report back the real voltage, so apply some math to get it back
    // 1. convert the ADC level to a float
    // 2. divide by (R2 / R1 + R2)
    batteryLevel.realVoltage = (float)batteryLevel.rawLevel / 1000 / (r2 / (r1 + r2));

    // USB is connected if the reading is ~870,
    // as the voltage will be 5V, so we assume it's charging.
    batteryLevel.isCharging = batteryLevel.rawLevel > 800 ? 1 : 0;

    batteryLevel._valid = true;

    return batteryLevel;
}

#endif