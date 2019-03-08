#ifndef BAIN_SENSOR
#define BAIN_SENSOR

#include <Wire.h>
#include <SPI.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BME280.h>

#include "battery.h"

#define BME_SCK 13
#define BME_MISO 12
#define BME_MOSI 11
#define BME_CS 10

// Modify below if you use SPI.
Adafruit_BME280 bme; // I2C
//Adafruit_BME280 bme(BME_CS); // hardware SPI
//Adafruit_BME280 bme(BME_CS, BME_MOSI, BME_MISO, BME_SCK); // software SPI

struct SensorValues
{
    float temperature;
    float pressure;
    float humidity;
    boolean _valid = false;
};

void initBME280Sensor()
{
    Serial.println("[BME280] - Checking for a sensor.");

    if (!bme.begin())
    {
        int max_try = 50;
        int i = 0;
        while (!bme.begin() && i < max_try)
        {
            delay(500);
            Serial.print(".");
            i++;
        }
        Serial.println("");
    }

    if (bme.begin())
    {
        Serial.println("[BME280] - Sensor found.");

        // Weather monitoring settings
        bme.setSampling(Adafruit_BME280::MODE_FORCED,
                        Adafruit_BME280::SAMPLING_X1, // temperature
                        Adafruit_BME280::SAMPLING_X1, // pressure
                        Adafruit_BME280::SAMPLING_X1, // humidity
                        Adafruit_BME280::FILTER_OFF);
    }
    else
    {
        Serial.println("[BME280] - Sensor not found.");
    }
}

void logSensorValues(String timestamp)
{
    Serial.print("[BME280] - Date : ");
    Serial.println(timestamp);

    Serial.print("[BME280] - Temperature = ");
    Serial.print(bme.readTemperature());
    Serial.println(" *C");

    Serial.print("[BME280] - Pressure = ");
    Serial.print(bme.readPressure() / 100.0F);
    Serial.println(" hPa");

    Serial.print("[BME280] - Humidity = ");
    Serial.print(bme.readHumidity());
    Serial.println(" %");

    Serial.println();
}

SensorValues getSensorValues()
{
    SensorValues sensorValues;

    if (bme.begin())
    {
        // Only needed in forced mode!
        // In normal mode, you can remove the next line.
        // Has no effect in normal mode
        bme.takeForcedMeasurement();

        // Read values twice to avoid NaN;
        bme.readTemperature();
        bme.readPressure();
        bme.readHumidity();

        sensorValues.temperature = bme.readTemperature();
        sensorValues.pressure = bme.readPressure() / 100.0F;
        sensorValues.humidity = bme.readHumidity();
        sensorValues._valid = true;
    }

    return sensorValues;
}

#endif