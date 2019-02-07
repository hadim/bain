# Bain

Bain is a DIY wireless thermometer. It is made of two parts:

- [Adafruit Feather HUZZAH with ESP8266](https://www.adafruit.com/product/2821): it's an Arduino compatible board that has WiFi and a connector to plug any Adafruit 3.7V Lithium polymer batteries.
- [Adafruit BME280 I2C or SPI](https://www.adafruit.com/product/2652): It's an environmental sensor with temperature, barometric pressure and humidity from Bosch.

This repository contains all the instructions to build the thermometer yourself. Generated data are sent to an MQTT broker but the code should be easily adaptable to the most convenient method that fit your needs.

## Instructions

### Assembly on a breadboard

TODO

### Flash the controller

I suggest you to use the [Arduino IDE](https://www.arduino.cc/en/main/software) to flash the controller to the board. Note that [a VSCode extension for Arduino IDE](https://marketplace.visualstudio.com/items?itemName=vsciot-vscode.vscode-arduino) is also available.

You need to add the following URL to the **Arduino Board Manager**: `http://arduino.esp8266.com/stable/package_esp8266com_index.json`.

Then in the **Arduino Board Manager**, install the `ESP8266` package. After this, you should be able to select the correct board from the **Board Manager**: `Adafruit Feather HUZZAH ESP8266`.

You also need to install the following Arduino libraries (use the **Arduino Library Manager**):

- Adafruit ESP8266
- Adafruit BME280 Library
- Adafruit Unified Sensor

Set WiFi parameters:

- Copy `src/secret.h.template` to `src/secret.h`.
- Edit `src/secret.h` to set WiFi SSID and WiFi password.

Now you're ready to flash your ESP8266 controller.

- From you Arduino editor, open [`src/controller.ino`](src/controller.ino).
- Compile it.
- After connecting the ESP8266 board to your computer, upload the controller.

### Final Assembly

Once you've checked that everything works you can solder both boards and insert them in a fancy 3D printed custom case.

TODO

## License

[MIT](./LICENSE).

## Author

- [Hadrien Mary](mailto:hadrien.mary_AT_gmail.com)

