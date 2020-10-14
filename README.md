# Weatherstation - Client

- [Weatherstation - Client](#weatherstation---client)
  - [Idea](#idea)
  - [Current State](#current-state)
  - [Visualization of Data](#visualization-of-data)
  - [Setup](#setup)
    - [Hardware](#hardware)
      - [D1 Mini setup](#d1-mini-setup)
      - [ESP 01 setup](#esp-01-setup)
    - [Software](#software)
      - [Software settings](#software-settings)
      - [Flashing](#flashing)
  - [Documentation](#documentation)
  - [Usage of the client](#usage-of-the-client)
    - [Accesspoint at the beginning](#accesspoint-at-the-beginning)
    - [Flashing software to several clients](#flashing-software-to-several-clients)
  - [Some Images](#some-images)
  - [Footnotes](#footnotes)

## Idea

The idea was to create a platform tracking various climate data, which can be extended and has no dependencies to the visualization. Therefore a network based solution was chosen.
<sup>[1](#EnergyConsumption)</sup>

As basis for the project a D1-mini was chosen, as it comes with onboard WiFi. But also other platforms can be used (e.g.: NodeMCU, ...).

The idea is to read the sensor data and send it via MQTT. Anyone who wants to use the data, can simply subscribe to the topic `/esp/#`. To indicate the type of data, the messages are defined by a MAC (to know from which client the message comes from), the type of data and the value. In this case, the topic holds this information in following structure: `/esp/MAC_ADDRESS/TYPE_OF_DATA/`

Have a look at [Weatherstation Server](https://github.com/jerey/weatherstation-server).

## Current State

Currently, two parameters are published:

1. Temperature
2. Humidity

Further can be added by using a different or further sensor.

## Visualization of Data

How the data is visualized is pretty much up to the user. Either one can use another D1-mini with an attached display, or one can use a server, which collects the data and visualizes it in a browser.

## Setup

The setup is split up into two areas: hardware setup and software setup.

### Hardware

The list of needed hardware is as follows:

- D1 Mini or an ESP 01
- BME 280
- 2 x 4k7 Ohm Resistor
- Button

#### D1 Mini setup

Following is the schematic for this setup with an **D1 Mini**.

<img src="/documentation/Schematic.svg" width="400">

Note: The BME280 was added with wires to get a more precise temperature. Initially it was also added to the base shield, but there the temperature was always a bit higher.

#### ESP 01 setup

If you want to reduce power consumption even more, you can use the **ESP01**.

Following pins from the [D1 mini](#d1-mini-setup) have to be changed then:

- `D3` :arrow_right: `Pin 0`
- `D4` :arrow_right: `Pin 2`

Further, to enable deep sleep, a little soldering has to be done. There are several tutorials in the internet for that, so I will not go into that here.

### Software

As basis for the software, platform IO was used as it comes with a good library management and makes flashing rather easy. But first have a look into the code.

#### Software settings

The software allows several settings within the `program.cpp`. There are more than the mentioned ones, but those are the most important ones:

1. `BME280_I2C_ADDR` - Depending on the used BME280, there are different I^2^C addresses in use. The used library offers a way to set this address, as described [here](https://github.com/adafruit/Adafruit_BME280_Library/issues/15). Usually the address is either `0x76` or `0x77`. Please check the datasheet of the used BME280 and set the value.
2. `ssidAP` & `passwordAP` - The SSID and password of the accesspoint, which the hardware starts, in case it cannot connect to any wifi. One can then look for the wifi `ssidAP` and connect to it with the password `passwordAP`.
3. `updateFrequency` - The frequency of updates of the parameters/values to be sent to the network. Defined in milliseconds.

#### Flashing

Install platformIO as described on their [webpage](https://docs.platformio.org/en/latest/core.html).
If you chose platformIO for the CLI, you can execute following command from the terminal: `platformio run --target upload`
If you use an ide, please refer to the [platformIO documenation](https://docs.platformio.org/en/latest/ide/pioide.html).

## Documentation

This chapter aims to describe some essential parts of the software.

## Usage of the client

When the software is flashed, the client will directly start. If it has not been configured previously, it will open up an accesspoint. Otherwise it will simply reconnect to the last configured WiFi and start publishing the data.

### Accesspoint at the beginning

When the client is started, the [WiFiManager](https://github.com/tzapu/WiFiManager) is being used to check, whether it has any known WiFi available to connect to. If it either does not know any WiFi or cannot connect to one, it opens up an access point and provides an user interface to setup the desired WiFi. From that point on, it will always try to connect to that WiFi. If you want to connect to another WiFi, press the button added to the board and the access point will again be opened.

### Flashing software to several clients

Work in progress: It currently is possible to flash several clients at once. The [ESP8266httpUpdate](https://github.com/esp8266/Arduino/tree/master/libraries/ESP8266httpUpdate) library is used to auto update the firmware, if the defined update server provides any new firmware. This decision is based on a version defined in the `program.cpp`. For now a python flask server is used as update server. This is currently based on the docker image [esp-update-server](https://hub.docker.com/r/kstobbe/esp-update-server/). For more information on that, checkout the [readme.md](https://github.com/kstobbe/esp-update-server).

## Some Images

On a breadboard:

<img src="/documentation/Prototype_01.jpg" width="400"><img src="/documentation/Prototype_02.jpg" width="400">

Further, one can also buy base shields and then solder all parts together:

<img src="/documentation/Prototype_03.jpg" width="400"><img src="/documentation/Prototype_04.jpg" width="400">

## Footnotes

<a name="EnergyConsumption">1</a>: For the purpose of the project, the network based solution is fine, but if one wants to tweak the energy consumption to a minimum, other options should be considered (e.g.: Bluetooth Low Energy (BLE), ...).
