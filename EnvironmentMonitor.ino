// Industrial IoT Environment Monitor - Week 6 Final Submission
#include <WiFi.h>
#include <PubSubClient.h> // For MQTT

const char* ssid = "YOUR_WIFI_NAME";
const char* password = "YOUR_WIFI_PASSWORD";
const char* mqtt_server = "broker.hivemq.com";

WiFiClient espClient;
PubSubClient client(espClient);

void setup() {
  Serial.begin(115200);
  setup_wifi();
  client.setServer(mqtt_server, 1883);
}

void setup_wifi() {
  delay(10);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) { delay(500); }
}

void loop() {
  if (!client.connected()) { reconnect(); }
  client.loop();

  // Simulated Sensor Reading for Industrial Benchmarks
  float temp = random(20, 35); 
  String payload = "{\"temperature\":" + String(temp) + "}";
  client.publish("uct/factory/env", (char*) payload.c_str());
  
  // Power Constraint: Enter Deep Sleep
  Serial.println("Entering Deep Sleep to save battery...");
  esp_deep_sleep_start(); 
}
