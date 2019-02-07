# Bain

Bain is a DIY wireless thermometer. It is made of two parts:

- [Adafruit Feather HUZZAH with ESP8266](https://www.adafruit.com/product/2821): it's an Arduino compatible board that has WiFi capability and a connector to plug any Adafruit 3.7V Lithium polymer batteries.
- [Adafruit BME280 I2C or SPI](https://www.adafruit.com/product/2652): It's an environmental sensor with temperature, barometric pressure and humidity from Bosch.

This repository contains all the instructions to build the thermometer yourself.

Features:

- Monitor temperature, pressure and humidity.
- Generated data are sent to a custom MQTT broker as a JSON string.
- Time is syncronized every 6h with NTP and the timezone can be set.
- A timestamp string is added to sensor data before they are sent.
- Four LEDs can indicate current controller state (WiFi, sensor, MQTT, NTP).
- The source code is profusely commented and factorized. It should be easily to adapt to your needs.

## Instructions

### Assembly on a breadboard

We will first connect both boards together using a breadboard in order to check that the flash procedure works well.

### Flash the controller

I suggest you to use the [Arduino IDE](https://www.arduino.cc/en/main/software) to flash the controller to the board. Note that [a VSCode extension for Arduino IDE](https://marketplace.visualstudio.com/items?itemName=vsciot-vscode.vscode-arduino) is also available.

You need to add the following URL to the **Arduino Board Manager**: `http://arduino.esp8266.com/stable/package_esp8266com_index.json`.

Then in the **Arduino Board Manager**, install the `ESP8266` package. After this, you should be able to select the correct board from the **Board Manager**: `Adafruit Feather HUZZAH ESP8266`.

You also need to install the following Arduino libraries (use the **Arduino Library Manager**):

- [Adafruit ESP8266](https://github.com/adafruit/Adafruit_ESP8266): Arduino board with WiFi capability.
- [Adafruit BME280 Library](https://github.com/adafruit/Adafruit_BME280_Library): Sensor library.
- [Adafruit Unified Sensor](https://github.com/adafruit/Adafruit_Sensor): Needed for Adafruit BME280 Library.
- [PubSubClient](https://github.com/knolleary/pubsubclient): MQTT Client.
- [ArduinoJson](https://github.com/bblanchon/ArduinoJson): a JSON library. **Warning**: as of today (06/02/2019), you should install version 5 only and not version 6.
- [NTPClient](https://github.com/arduino-libraries/NTPClient): an NTP client library to get date and time.

Set WiFi parameters:

- Copy `src/secret.h.template` to `src/secret.h`.
- Edit `src/secret.h` to set WiFi and MQTT parameters.

Now you're ready to flash your ESP8266 controller.

- From you Arduino editor, open [`src/main.ino`](src/main.ino).
- Compile it.
- After connecting the ESP8266 board to your computer, upload the controller.

The sensor values are sent as a JSON string:

```json
{
  "temperature": 25.23,
  "pressure": 1018.4,
  "humidty": 90.12,
  "timestamp": "2019-02-15 15:45:23"
}
```

### Final Assembly

Once you've checked that everything works you can solder both boards and insert them in a fancy 3D printed custom case.

TODO

## Home Assistant

If you are using [Home Assistant](https://www.home-assistant.io) to retrieve the sensor values through an MQTT broker, here is the `sensor` configuration you can use:

```yaml
sensor:
  - platform: mqtt
    name: 'Temperature 1'
    state_topic: 'bain_sensor/sensor1'
    unit_of_measurement: '°C'
    value_template: '{{ value_json.temperature }}'

  - platform: mqtt
    name: 'Humidity 1'
    state_topic: 'bain_sensor/sensor1'
    unit_of_measurement: '%'
    value_template: '{{ value_json.humidity }}'

  - platform: mqtt
    name: 'Pressure 1'
    state_topic: 'bain_sensor/sensor1'
    unit_of_measurement: 'hPa'
    value_template: '{{ value_json.pressure }}'
```

## License

[MIT](./LICENSE).

## Author

- [Hadrien Mary](mailto:hadrien.mary_AT_gmail.com)
