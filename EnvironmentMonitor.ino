/*
 * Project: Smart Industrial Environment Monitor
 * Internship: upSkill Campus & UniConverge Technologies (UCT)
 * File: EnvironmentMonitor.ino
 */

#include <WiFi.h>
#include <PubSubClient.h> // MQTT Library

// Network Credentials
const char* ssid = "YOUR_WIFI_SSID";
const char* password = "YOUR_WIFI_PASSWORD";

// MQTT Broker (Industry standard protocol)
const char* mqtt_server = "broker.hivemq.com"; 

WiFiClient espClient;
PubSubClient client(espClient);

void setup() {
  Serial.begin(115200);
  
  // Connect to Wi-Fi
  setup_wifi();
  
  // Set MQTT Broker [cite: 45]
  client.setServer(mqtt_server, 1883);
}

void setup_wifi() {
  Serial.print("Connecting to WiFi...");
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("Connected!");
}

void reconnect() {
  while (!client.connected()) {
    if (client.connect("Jayesh_IoT_Node")) {
      Serial.println("MQTT Connected");
    } else {
      delay(5000);
    }
  }
}

void loop() {
  if (!client.connected()) {
    reconnect();
  }
  client.loop();

  // 1. Reading Sensors [cite: 93, 100]
  float temperature = random(22, 30); // Simulated industrial temp
  float gasLevel = random(100, 400);  // Simulated gas ppm

  // 2. Data Serialization into JSON [cite: 43]
  String payload = "{\"temp\":" + String(temperature) + ",\"gas\":" + String(gasLevel) + "}";
  
  // 3. Publish to Cloud Dashboard [cite: 47, 90]
  client.publish("uct/factory/sensor", (char*) payload.c_str());
  Serial.println("Data published: " + payload);

  // 4. Performance Constraint: Low Power [cite: 106, 110]
  Serial.println("Entering Deep Sleep to save battery...");
  esp_sleep_enable_timer_wakeup(600 * 1000000); // Sleep for 10 minutes
  esp_deep_sleep_start();
}
