# Weatherstation - Client

[![Build Status](https://travis-ci.com/aaj07/weatherstation-client.svg?branch=master)](https://travis-ci.com/aaj07/weatherstation-client)

- [Weatherstation - Client](#weatherstation---client)
  - [Idea](#idea)
  - [Current State](#current-state)
  - [Visualization of Data](#visualization-of-data)
  - [Setup](#setup)
    - [Hardware](#hardware)
    - [Software](#software)
      - [Software settings](#software-settings)
      - [Flashing](#flashing)
  - [Documentation](#documentation)
    - [UDP Message Structure](#udp-message-structure)
      - [Temperature message](#temperature-message)
      - [Humidity message](#humidity-message)
  - [Usage of the client](#usage-of-the-client)
    - [Accesspoint at the beginning](#accesspoint-at-the-beginning)
    - [Flashing software to several clients](#flashing-software-to-several-clients)
  - [Some Images](#some-images)
  - [Footnotes](#footnotes)

## Idea

The idea was to create a platform tracking various climate data, which can be extended and has no dependencies to the visualization. Therefore a network based solution was chosen.
<sup>[1](#EnergyConsumption)</sup>

As basis for the project a D1-mini was chosen, as it comes with onboard WiFi. But also other platforms can be used (e.g.: NodeMCU, ...).

The idea is to read the sensor data and send a UDP broadcast in the network. Anyone who wants to use the data, can simply listen to the messages. To indicate the type of data, the messages are defined by a MAC (to know from which client the message comes from), a message id (to know the type of data) and the value.
One could also try getting the mac adress via an ARP-request, but going through various layers could cause a wrong mac adress (e.g. the communication to the docker container). It would not crash, but it would also not be the right mac adress.

Have a look at [Weatherstation Server](https://github.com/aaj07/weatherstation-server).

## Current State

Currently, two parameters are sent:

1. Temperature
2. Humidity

Further can be added by using a different or further sensor.

## Visualization of Data

How the data is visualized is pretty much up to the user. Either one can use another D1-mini with an attached display, or one can use a server, which collects the data and visualizes it in a browser.

## Setup

The setup is split up into two areas: hardware setup and software setup.

### Hardware

The list of needed hardware is as follows:

- D1 Mini
- BME 280
- 4k7 Ohm Resistor
- Button

Following is the schematic for this setup.

<img src="/documentation/Schematic.svg" width="400"> 

Note: The BME280 was added with wires to get a more exact temperature. Initially it was also added to the base shield, but there the temperature was always a bit higher.

### Software

As basis for the software, platform IO was used as it comes with a good library management and makes flashing rather easy. But first have a look into the code.

#### Software settings

The software allows several settings within the ```program.cpp```. There are more than the mentioned ones, but those are the most important ones:

1. ```BME280_I2C_ADDR``` - Depending on the used BME280, there are different I^2^C adresses in use. The used library offers a way to set this adress, as described [here](https://github.com/adafruit/Adafruit_BME280_Library/issues/15). Usually the adress is either ```0x76``` or ```0x77```. Please check the datasheet of the used BME280 and set the value.
2. ```ssidAP``` & ```passwordAP``` - The SSID and password of the accesspoint, which the hardware starts, in case it cannot connect to any wifi.
3. ```updateFrequency``` - The frequency of updates of the parameters/values to be sent to the network. Defined in milliseconds.

#### Flashing

Install platformIO as described on their [webpage](https://docs.platformio.org/en/latest/core.html).
If you chose platformIO for the CLI, you can execute following command from the terminal: ```platformio run --target upload```
If you use an ide, please refer to the [platformIO documenation](https://docs.platformio.org/en/latest/ide/pioide.html).

## Documentation

This chapter aims to describe some essential parts of the software.

### UDP Message Structure

The basic structure of a message is as follows:

| Mac Adress | Message ID | Value |
|------------|------------|-------|
| 6 Bytes    | 1 Byte     | ...   |

The mac adress is added without the ":". The size of the value is defined for each message type.

#### Temperature message

For the temperature message, the current temperature is stored in one byte. One byte would allow to store temperature values from '0' - '255'°. Using a signed byte (to allow negative values), one gets a range from '-128' - '127'°. To have a more accurate temperature, the value is bitwise shift to the left. Through that one can send values from '-64' - '63.5'°. The message then looks as follows:

| Mac Adress | Message ID | +/-   | 32    | 16    | 8     | 4     | 2     | 1     | 0.5   |
|------------|------------|-------|-------|-------|-------|-------|-------|-------|-------|
| 6 Bytes    | 1 Byte     | 1 bit | 1 bit | 1 bit | 1 bit | 1 bit | 1 bit | 1 bit | 1 bit |

To give an samplle of possible values, see following example table:

| Mac Adress | Message ID | +/- | 32 | 16 | 8 | 4 | 2 | 1 | 0.5 | Result |
|------------|------------|-----|----|----|---|---|---|---|-----|--------|
| Any        | 1          | 0   | 0  | 0  | 0 | 0 | 0 | 0 | 0   | 0°     |
| Any        | 1          | 0   | 1  | 1  | 1 | 1 | 1 | 1 | 0   | 63°    |
| Any        | 1          | 0   | 1  | 1  | 1 | 1 | 1 | 1 | 1   | 63.5°  |
| Any        | 1          | 1   | 0  | 0  | 0 | 0 | 0 | 0 | 0   | -64°   |
| Any        | 1          | 1   | 0  | 0  | 0 | 0 | 0 | 0 | 1   | -63.5° |
| Any        | 1          | 1   | 1  | 1  | 1 | 1 | 1 | 1 | 1   | -0.5°  |

#### Humidity message

Compared to the temperature message, the humidity message is rather simple, since the humidity can only have values from 0% to 100%. Therefore the message looks as follows:

| Mac Adress | Message ID | Value  |
|------------|------------|--------|
| 6 Bytes    | 1 Byte     | 1 Byte |

## Usage of the client

When the software is flashed, the client will directly start. If it has not been configured previously, it will open up an accesspoint. Otherwise it will simply reconnect to the last configured WiFi and start broadcasting the data.

### Accesspoint at the beginning

When the client is started, the [WiFiManager](https://github.com/tzapu/WiFiManager) is being used to check, whether it has any known WiFi available to connect to. If it either does not know any WiFi or cannot connect to one, it opens up an access point and provides an user interface to setup the desired WiFi. From that point on, it will always try to connect to that WiFi. If you want to connect to another WiFi, press the button added to the board and the access point will again be opened.

### Flashing software to several clients

Work in progress: It currently is possible to flash several clients at once. The [ESP8266httpUpdate](https://github.com/esp8266/Arduino/tree/master/libraries/ESP8266httpUpdate) library is used to auto update the firmware, if the defined update server provides any new firmware. This decision is based on a version defined in the ```program.cpp```. For now a python flask server is used as update server. This is currently based on the docker image [esp-update-server](https://hub.docker.com/r/kstobbe/esp-update-server/). For more information on that, checkout the [readme.md](https://github.com/kstobbe/esp-update-server).

## Some Images

On a breadboard:

<img src="/documentation/Prototype_01.jpg" width="400"><img src="/documentation/Prototype_02.jpg" width="400">

Further, one can also buy base shields and then solder all parts together:

<img src="/documentation/Prototype_03.jpg" width="400"><img src="/documentation/Prototype_04.jpg" width="400">

## Footnotes

<a name="EnergyConsumption">1</a>: For the purpose of the project, the network based solution is fine, but if one wants to tweak the energy consumption to a minimum, other options should be considered (e.g.: Bluetooth Low Energy (BLE), ...).
