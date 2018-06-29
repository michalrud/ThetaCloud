#include <ESP8266WiFi.h>
#include <PubSubClient.h>  // PubSubClient library is required - install it using Arduino IDE
#include <ThetaCloud.h>
#include <ThetaCloudLight.h>
#include <ThetaCloudRelay.h>
#include <ThetaCloudHumidity.h>
#include <ThetaCloudCO2.h>

const unsigned int THETA_CLOUT_TICK_DELAY_S = 5;
const char SSID[] = "YOUR_SSID";
const char PASS[] = "YOUR PASSWORD";
std::string deviceId = "UNKNOWN";
IPAddress server(192, 168, 0, 2);   // IP Address of an MQTT Broker.
WiFiClient wclient;
PubSubClient client(wclient);

void connectToWifi()
{
  WiFi.begin(SSID, PASS);

  Serial.print("Connecting");
  while (WiFi.status() != WL_CONNECTED)
  {
    delay(500);
    Serial.print(".");
  }
  Serial.println();

  Serial.print("Connected, IP address: ");
  Serial.println(WiFi.localIP());
}

void handleSensorData(const SensorData& sensorData)
{
  client.publish((deviceId + sensorData.name).c_str(), sensorData.getValue());
}

void handleMqttWrite(char* topic, byte* payload, unsigned int length)
{
  std::string payload_str((const char*) payload, length);
  thetaCloud.write(SensorData{std::string(topic), payload_str});
}

void setDeviceId()
{
  char buf[10];
  deviceId = ltoa(ESP.getChipId(), buf, 16);
  deviceId += "_";
}

void setup()
{
  setDeviceId();
  Serial.begin(115200);
  Serial.println();
  connectToWifi();
  
  client.setServer(server, 1883);
  client.setCallback(handleMqttWrite);
  client.connect((deviceId + "esp").c_str());
  client.subscribe("Relay1");
  client.subscribe("Relay2");
  
  thetaCloud.whenDataAvailable(handleSensorData);
  thetaCloud.init();
  thetaCloudLight.init();
  thetaCloudRelay.init();
  thetaCloudHumidity.init();
  thetaCloudCO2.init();
}

void loop()
{
  thetaCloud.tick();
  for (unsigned int i = 0; i < THETA_CLOUT_TICK_DELAY_S*1000; i += 100) {
    client.loop();
    delay(100);
  }
}
