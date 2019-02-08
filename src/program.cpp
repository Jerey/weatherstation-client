#include <Arduino.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BME280.h>
#include <SPI.h>
#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <WiFiUdp.h>
#include <Wire.h>
#include <WiFiManager.h>
#include <DNSServer.h>
#include <ESP8266WebServer.h>
#include <string>
#include <algorithm>

#include "msg.temperature/MsgTemperature.h"
#include "msg.humidity/MsgHumidity.h"

//------------------------------ CONSTANTS ------------------------------
#define TRIGGER_CONFIG_PORTAL_PIN D8
#define BME280_I2C_ADDR 0x76 // usually 0x76 or 0x77 --> See https://github.com/adafruit/Adafruit_BME280_Library/issues/15

//------------------------------ BME Sensure Setup ------------------------------
Adafruit_BME280 bme;

//------------------------------ UDP Setup ------------------------------
WiFiUDP clientUDP;
IPAddress udpAdress;
const unsigned int UDPPort = 2807;

//------------------------------ AP ------------------------------
const char* ssidAP = "AutoConnectAP";
const char* passwordAP = "password";

//------------------------------ Non Blocking Refresh Parameters ------------------------------
unsigned long lastUpdateMillis;
unsigned long updateFrequency = 10000;

//------------------------------ MAC Adress
std::string macAdress;


/**
 * Helper function, which simply prints "." and sets a delay.
 * @param msec The delay in mseconds.
 */
void serialPrintDotAndDelay(int msec) {
	Serial.print(".");
	delay(msec);
}

/**
 * Helper function to print a string and a value. It does not add a newline after printing.
 * @param str The string describing what the value does represent.
 * @param val The value.
 */
void extendetPrint(String str, float val) {
	Serial.print(str);
	Serial.print(val);
}

/**
 * Helper function to print a string, a value and a unit. It does add a newline after printing.
 * @see extendedPrint
 * @param str The string describing what the value does represent.
 * @param val The value.
 * @param unit The unit of the value
 */
void extendetPrint(String str, float val, String unit) {
	extendetPrint(str, val);
	Serial.println(unit);
}

/**
 * Function to connect to a wifi. It waits until a connection is established.
 */
void connectToWifi() {
	WiFiManager wifiManager;
	wifiManager.setBreakAfterConfig(true);
	wifiManager.autoConnect(ssidAP, passwordAP);
	Serial.println(WiFi.SSID());
	auto mac = std::string(WiFi.macAddress().c_str());
	mac.erase (std::remove(mac.begin(), mac.end(), ':'), mac.end());
	macAdress = mac;
	Serial.println(" -> Success!");
}

/**
 * Get the current ip of the device and create the broadcast ip.
 */
void getAndSetTheUDPBroadcastIP() {
	udpAdress = WiFi.localIP();
	Serial.print("My Current Adress: ");
	Serial.println(udpAdress);
	udpAdress[3] = 255;
	Serial.print("My Broadcasting Adress: ");
	Serial.println(udpAdress);
}

/**
 * Function to be used, when an error is detected.
 * @param slowblinks Defines how many slow blinks shall occur per execution.
 * @param fastblinks Defines how many fast blinks shall occur per execution.
 * @param loop [false] Execute the blinking and logging routine once. [true] Execute the blinking and logging routine endless.
 * @param logMessage The log message, which shall be print to the serial console.
 */
void errorDetected(uint8_t slowblinks, uint8_t fastblinks, bool loop, const char* logMessage) {
    pinMode(LED_BUILTIN, OUTPUT);
    do {
        Serial.println(logMessage);
        for (int i = 0; i < slowblinks; ++i) {
            digitalWrite(LED_BUILTIN, LOW);
            delay(250);
            digitalWrite(LED_BUILTIN, HIGH);
            delay(250);
        }
        for (int i = 0; i < fastblinks; ++i) {
            digitalWrite(LED_BUILTIN, LOW);
            delay(100);
            digitalWrite(LED_BUILTIN, HIGH);
            delay(100);
        }
    } while(loop);
}


//------------------------------ Setup ------------------------------
void setup() {
	Serial.begin(115200);
	Serial.println(
			"**************************\n******** BEGIN ***********\n**************************");

	pinMode(TRIGGER_CONFIG_PORTAL_PIN, INPUT);

	Wire.begin(D3, D4);
	Wire.setClock(100000);
	bool status = bme.begin(BME280_I2C_ADDR); 
	if (!status) {
        errorDetected(16, 16, true, "Could not find a valid BME280 sensor, check wiring or if the defined I2C adress is correct!");
	}
	connectToWifi();
	getAndSetTheUDPBroadcastIP();
}

/**
 * Helper to send the given parameter as a udp broadcast.
 * @param str The string to be broadcasted.
 */
void udpBroadcastStream(const char *str) {
    clientUDP.beginPacket(udpAdress, UDPPort);
    auto package = macAdress;
    package.append(str);
    clientUDP.write(package.c_str());
    clientUDP.endPacket();
}

/**
 * Sends a broadcast with the current temperature.
 */
void udpBroadcastTemperature() {
    float t = bme.readTemperature();
    extendetPrint("Temperature: ", t, " *C");
    Msg::Temperature::MsgTemperature msgTemp(t);
    auto resTemp = msgTemp.getMsgStream();
    udpBroadcastStream(resTemp);
}

/**
 * Sends a broadcast with the current humidity.
 */
void udpBroadcastHumidity() {
    float h = bme.readHumidity();
    extendetPrint("Humidity: ", h, " Precent");
    Msg::Humidity::MsgHumidity msgHum(h);
    auto resHum = msgHum.getMsgStream();
    udpBroadcastStream(resHum);
}

/**
 * Handle the "wifi config button"-pressed.
 */
void handleWifiConfigPageButtonPressed() {
    if (digitalRead(TRIGGER_CONFIG_PORTAL_PIN) == HIGH) {
        Serial.println("Received a trigger to go back to a config page!");
        WiFiManager wifiManager;
        wifiManager.startConfigPortal(ssidAP, passwordAP);
    }
}

/**
 * Functino to handle sensor updates: Instead of sleeping, it checks if the msec have passed to update correclty.
 */
void handleSensorUpdates() {
    unsigned long currentMillis = millis();
    if ((currentMillis - lastUpdateMillis) > updateFrequency) {
        lastUpdateMillis = currentMillis;
        udpBroadcastTemperature();
        udpBroadcastHumidity();
    }
}

//------------------------------ LOOP ------------------------------
void loop() {
    handleWifiConfigPageButtonPressed();
    handleSensorUpdates();
}
