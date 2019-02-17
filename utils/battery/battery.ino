// The purpose of this file is to check the battery level only.
#include "battery.h"

void setup()
{
}

void loop()
{
    BatteryLevel level = measureBatteryLevel();

    Serial.println("-----------");
    Serial.print("Raw Level: ");
    Serial.println(level.rawLevel);
    Serial.print("Level: ");
    Serial.println(level.level);
    Serial.print("Voltage: ");
    Serial.println(level.realVoltage);
    Serial.print("Is Charging: ");
    Serial.println(level.isCharging);

    delay(1000);
}