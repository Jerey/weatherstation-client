#include <Adafruit_BME280.h>
#include <Arduino.h>
#include <ESP8266httpUpdate.h>
#include <PubSubClient.h>
#include <WiFiManager.h>

//----------- UpdateServer Constants -----------
#define VERSION "v1.0.3"
#define HOST "Weatherstation_Temp_Hum"
constexpr const char* updateServerURL =
    "http://192.168.178.100:5000/update";  // Enter the correct update URL here.

//----------- Non Blocking Refresh Parameters -----------
constexpr unsigned long updateFrequency = 15 * 60 * 1000;  // In ms.

//----------- CONSTANTS -----------
#define BME280_I2C_ADDR \
  0x76  // usually 0x76 or 0x77 --> See
        // https://github.com/adafruit/Adafruit_BME280_Library/issues/15

//----------- BME Sensure Setup -----------
Adafruit_BME280 bme;

//----------- AP -----------
constexpr const char* ssidAP = "AutoConnectAP";
constexpr const char* passwordAP = "password";

//----------- MAC Adress -----------
std::string macAdress;
std::string topic;

//----------- MQTT -----------
constexpr const char* mqttBroker = "192.168.178.100";
WiFiClient espClient;
PubSubClient mqttClient(espClient);
char mqttBuffer[64];

/**
 * @brief Function to check the given updateServerURL for new updates.
 * @see https://github.com/kstobbe/esp-update-server
 */
void checkForSoftwareUpdates() {
  String checkUrl = String(updateServerURL);
  checkUrl.concat("?ver=" + String(VERSION));
  checkUrl.concat("&dev=" + String(HOST));

  Serial.println("INFO: Checking for updates at URL: " + String(checkUrl));
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wdeprecated-declarations"
  auto returnStatus = ESPhttpUpdate.update(checkUrl);
#pragma GCC diagnostic pop

  switch (returnStatus) {
    default:
    case HTTP_UPDATE_FAILED:
      Serial.println("ERROR: HTTP_UPDATE_FAILD Error (" +
                     String(ESPhttpUpdate.getLastError()) +
                     "): " + ESPhttpUpdate.getLastErrorString().c_str());
      break;
    case HTTP_UPDATE_NO_UPDATES:
      Serial.println("INFO: HTTP_UPDATE_NO_UPDATES");
      break;
    case HTTP_UPDATE_OK:
      Serial.println("INFO status: HTTP_UPDATE_OK");
      break;
  }
}

/**
 * Helper function to print a string and a value. It does not add a newline
 * after printing.
 * @param str The string describing what the value does represent.
 * @param val The value.
 */
void extendetPrint(String str, float val) {
  Serial.print(str);
  Serial.print(val);
}

/**
 * Helper function to print a string, a value and a unit. It does add a newline
 * after printing.
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
  mac.erase(std::remove(mac.begin(), mac.end(), ':'), mac.end());
  macAdress = mac;
  topic = "esp/" + macAdress;
  Serial.println(" -> Success!");
}

/**
 * Function to be used, when an error is detected.
 * @param slowblinks Defines how many slow blinks shall occur per execution.
 * @param fastblinks Defines how many fast blinks shall occur per execution.
 * @param loop [false] Execute the blinking and logging routine once. [true]
 * Execute the blinking and logging routine endless.
 * @param logMessage The log message, which shall be print to the serial
 * console.
 */
void errorDetected(uint8_t slowblinks, uint8_t fastblinks, bool loop,
                   const char* logMessage) {
  pinMode(LED_BUILTIN, OUTPUT);
  do {
    Serial.println(logMessage);
    for (unsigned int i = 0; i < slowblinks; ++i) {
      digitalWrite(LED_BUILTIN, LOW);
      delay(250);
      digitalWrite(LED_BUILTIN, HIGH);
      delay(250);
    }
    for (unsigned int i = 0; i < fastblinks; ++i) {
      digitalWrite(LED_BUILTIN, LOW);
      delay(100);
      digitalWrite(LED_BUILTIN, HIGH);
      delay(100);
    }
  } while (loop);
}

//----------- Setup -----------
ADC_MODE(ADC_VCC)
void setup() {
  Serial.begin(115200);
  Serial.println(
      "**************************\n\r******** BEGIN "
      "***********\n\r**************************");

  Wire.begin(D3, D4);
  Wire.setClock(100000);
  bool status = bme.begin(BME280_I2C_ADDR);
  if (!status) {
    errorDetected(16, 16, true,
                  "Could not find a valid BME280 sensor, check wiring or if "
                  "the defined I2C adress is correct!");
  }
  connectToWifi();
  Serial.print("Mqtt broker adress given: ");
  Serial.println(mqttBroker);
  mqttClient.setServer(mqttBroker, 1883);

  pinMode(D0, WAKEUP_PULLUP);
}

/**
 * Tries to reconnect to the mqtt broker.
 */
void reconnectToMqttBroker() {
  while (not mqttClient.connected()) {
    Serial.print("Reconnecting...");
    if (not mqttClient.connect("ESP8266Client")) {
      Serial.print("failed, rc=");
      Serial.print(mqttClient.state());
      Serial.println(" retrying in 5 seconds");
      delay(5000);
    }
  }
}

/**
 * Function to handle sensor updates.
 */
void handleSensorUpdates() {
  reconnectToMqttBroker();

  mqttClient.loop();
  const auto currentTemperature = bme.readTemperature();
  snprintf(mqttBuffer, sizeof mqttBuffer, "%f", currentTemperature);
  extendetPrint("Temperature: ", currentTemperature, " *C");
  auto resultOfPublish =
      mqttClient.publish((topic + "/temperature").c_str(), mqttBuffer);
  if (not resultOfPublish) {
    errorDetected(10, 10, false, "Publishing the temperature failed!");
    Serial.print("State of client: ");
    Serial.println(mqttClient.state());
  }
  mqttClient.loop();

  const auto currentHumidity = bme.readHumidity();
  snprintf(mqttBuffer, sizeof mqttBuffer, "%f", currentHumidity);
  extendetPrint("Humidity: ", currentHumidity, " %");
  resultOfPublish =
      mqttClient.publish((topic + "/humidity").c_str(), mqttBuffer);
  if (not resultOfPublish) {
    errorDetected(10, 10, false, "Publishing the humidity failed!");
    Serial.print("State of client: ");
    Serial.println(mqttClient.state());
  }
  mqttClient.loop();

  const auto vccValue = ESP.getVcc() / 1024.0f;
  snprintf(mqttBuffer, sizeof mqttBuffer, "%f", vccValue);
  extendetPrint("Voltage: ", vccValue, " V");
  resultOfPublish =
      mqttClient.publish((topic + "/voltage").c_str(), mqttBuffer);
  if (not resultOfPublish) {
    errorDetected(10, 10, false, "Publishing the voltage failed!");
    Serial.print("State of client: ");
    Serial.println(mqttClient.state());
  }
  mqttClient.loop();
}

//----------- LOOP -----------
void loop() {
  checkForSoftwareUpdates();
  handleSensorUpdates();
  mqttClient.disconnect();
  ESP.deepSleep(updateFrequency * 1000);
}
