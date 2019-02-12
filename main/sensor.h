#ifndef BAIN_SENSOR
#define BAIN_SENSOR

#include <Wire.h>
#include <SPI.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BME280.h>
#include <ArduinoJson.h>

#define BME_SCK 13
#define BME_MISO 12
#define BME_MOSI 11
#define BME_CS 10

// Modify below if you use SPI.
Adafruit_BME280 bme; // I2C
//Adafruit_BME280 bme(BME_CS); // hardware SPI
//Adafruit_BME280 bme(BME_CS, BME_MOSI, BME_MISO, BME_SCK); // software SPI

void initBME280Sensor()
{
    Serial.println("[BME280] - Checking for a sensor.");

    if (!bme.begin())
    {
        while (!bme.begin())
        {
            delay(500);
            Serial.print(".");
        }
        Serial.println("");
    }

    Serial.println("[BME280] - Sensor found.");

    // Weather monitoring settings
    bme.setSampling(Adafruit_BME280::MODE_FORCED,
                    Adafruit_BME280::SAMPLING_X1, // temperature
                    Adafruit_BME280::SAMPLING_X1, // pressure
                    Adafruit_BME280::SAMPLING_X1, // humidity
                    Adafruit_BME280::FILTER_OFF);
}

void sensorMeasure()
{
    // Only needed in forced mode! In normal mode, you can remove the next line.
    bme.takeForcedMeasurement(); // Has no effect in normal mode
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

String getValuesAsJSONString(String timestamp, float voltage)
{
    const size_t capacity = JSON_OBJECT_SIZE(4) + 90;
    DynamicJsonBuffer jsonBuffer(capacity);

    JsonObject &root = jsonBuffer.createObject();

    root["temperature"] = bme.readTemperature();
    root["pressure"] = bme.readPressure() / 100.0F;
    root["humidity"] = bme.readHumidity();
    root["voltage"] = voltage;

    const char *timestampChar = timestamp.c_str();
    root["date"] = timestampChar;

    String message = "";
    root.printTo(message);
    return message;
}

#endif