#ifndef BAIN_NTP
#define BAIN_NTP

#include <NTPClient.h>
#include <WiFiUdp.h>

#include "parameters.h"

// NTP client
WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP);

void initNTP()
{
    // Init NTP Client.
    timeClient.begin();

    // Set the timezone.
    timeClient.setTimeOffset(timeOffsetHours * 3600);

    // Set time update interval to 6 hours.
    timeClient.setUpdateInterval(6 * 3600);
}

void updateNTP()
{
    timeClient.update();
}

String getTimeStampString()
{
    time_t rawtime = timeClient.getEpochTime();
    struct tm *ti;
    ti = localtime(&rawtime);

    uint16_t year = ti->tm_year + 1900;
    String yearStr = String(year);

    uint8_t month = ti->tm_mon + 1;
    String monthStr = month < 10 ? "0" + String(month) : String(month);

    uint8_t day = ti->tm_mday;
    String dayStr = day < 10 ? "0" + String(day) : String(day);

    uint8_t hours = ti->tm_hour;
    String hoursStr = hours < 10 ? "0" + String(hours) : String(hours);

    uint8_t minutes = ti->tm_min;
    String minuteStr = minutes < 10 ? "0" + String(minutes) : String(minutes);

    uint8_t seconds = ti->tm_sec;
    String secondStr = seconds < 10 ? "0" + String(seconds) : String(seconds);

    return yearStr + "-" + monthStr + "-" + dayStr + " " +
           hoursStr + ":" + minuteStr + ":" + secondStr;
}

#endif