#include <Adafruit_BME280.h>
#include <Arduino.h>
#include <PubSubClient.h>
#include <WiFiManager.h>
#include <string>

//----------- Non Blocking Refresh Parameters -----------
constexpr unsigned long updateFrequency = 15 * 60 * 1000;  // In ms.

//----------- CONSTANTS -----------
// usually 0x76 or 0x77 --> See
// https://github.com/adafruit/Adafruit_BME280_Library/issues/15
constexpr uint8_t BME280_I2C_ADDR = 0x76;

//----------- BME Sensure Setup -----------
Adafruit_BME280 bme;

//----------- AP -----------
constexpr const char *ssidAP = "AutoConnectAP";
constexpr const char *passwordAP = "password";

//----------- MAC Address -----------
std::string macAddress;
std::string topic;

//----------- MQTT -----------
constexpr const char *mqttBroker =
    "192.168.178.100";  // Enter your mqtt broker address here.
WiFiClient espClient;
PubSubClient mqttClient(espClient);
char mqttBuffer[64];

//----------- MQTT Topics -----------
constexpr const char *topicTemperature = "/temperature";
constexpr const char *topicHumidity = "/humidity";
constexpr const char *topicVoltage = "/voltage";

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
  macAddress = mac;
  topic = "esp/" + macAddress;
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
                   const char *logMessage) {
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
#ifdef ESP01
  Wire.begin(0, 2);
  pinMode(16, WAKEUP_PULLUP);
#endif
#ifdef D1MINI
  Wire.begin(D3, D4);
  pinMode(D0, WAKEUP_PULLUP);
#endif
  Wire.setClock(100000);
  bool status = bme.begin(BME280_I2C_ADDR);
  if (!status) {
    errorDetected(4, 4, false,
                  "Could not find a valid BME280 sensor, check wiring or if "
                  "the defined I2C address is correct!");
  }
  connectToWifi();
  Serial.print("Mqtt broker address given: ");
  Serial.println(mqttBroker);
  mqttClient.setServer(mqttBroker, 1883);
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
 * @brief Tries to publish the given sensorValue to the given targetTopic
 */
void publishMqttTopic(float sensorValue, const char *targetTopic) {
  snprintf(mqttBuffer, sizeof mqttBuffer, "%f", sensorValue);
  Serial.print(topic.c_str());
  Serial.print(targetTopic);
  Serial.print(": ");
  Serial.println(sensorValue);
  auto resultOfPublish =
      mqttClient.publish((topic + targetTopic).c_str(), mqttBuffer);
  if (not resultOfPublish) {
    const char *errorLog =
        (std::string("Failed publishing topic: '") + targetTopic).c_str();
    errorDetected(3, 3, false, errorLog);
    Serial.print("State of client: ");
    Serial.println(mqttClient.state());
  }
  mqttClient.loop();
}
/**
 * Function to handle sensor updates.
 */
void handleSensorUpdates() {
  reconnectToMqttBroker();

  mqttClient.loop();
  publishMqttTopic(bme.readTemperature(), topicTemperature);
  publishMqttTopic(bme.readHumidity(), topicHumidity);
  publishMqttTopic(ESP.getVcc() / 1024.0f, topicVoltage);
}

//----------- LOOP -----------
void loop() {
  handleSensorUpdates();
  mqttClient.disconnect();
  ESP.deepSleep(updateFrequency * 1000);
}
