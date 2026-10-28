#define BLYNK_TEMPLATE_ID "TMPL68zHHtM13"
#define BLYNK_TEMPLATE_NAME "LoRa Sensor Gateway"

#include <WiFi.h>
#include <LoRa.h>
#include <BlynkSimpleEsp32.h>
#include "secrets.h"  // Local file excluded from version control.

#define LORA_SS 5
#define LORA_RST 4
#define LORA_DIO0 13
#define LED_PIN 2

int node1LED = 0;
int node2LED = 0;

void connectToWiFi() {
  Serial.print("Connecting to WiFi");
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  int attempts = 0;
  while (WiFi.status() != WL_CONNECTED && attempts < 10) {
    delay(500);
    Serial.print(".");
    attempts++;
  }
  if (WiFi.status() == WL_CONNECTED) {
    Serial.println("\nWiFi connected. IP: " + WiFi.localIP().toString());
  } else {
    Serial.println("\nFailed to connect to WiFi. Restarting...");
    delay(3000);
    ESP.restart();
  }
}

void setup() {
  Serial.begin(115200);
  pinMode(LED_PIN, OUTPUT);
  digitalWrite(LED_PIN, LOW);
  connectToWiFi();
  Blynk.begin(BLYNK_AUTH_TOKEN, WIFI_SSID, WIFI_PASSWORD);

  LoRa.setPins(LORA_SS, LORA_RST, LORA_DIO0);
  if (!LoRa.begin(433E6)) {
    Serial.println("LoRa init failed. Restarting...");
    delay(2000);
    ESP.restart();
  }
  LoRa.setSpreadingFactor(12);
  LoRa.setSignalBandwidth(125E3);
  LoRa.setCodingRate4(8);
  Serial.println("Gateway ready");
}

void loop() {
  Blynk.run();
  int packetSize = LoRa.parsePacket();
  if (packetSize) {
    String received = "";
    while (LoRa.available()) received += (char)LoRa.read();
    Serial.println("Received: " + received);

    digitalWrite(LED_PIN, HIGH);
    delay(100);
    digitalWrite(LED_PIN, LOW);

    if (received.startsWith("NODE_")) {
      int t = received.indexOf("Temp:");
      int h = received.indexOf("Hum:");
      int commaAfterTemp = received.indexOf(",", t);
      if (t > 0 && h > 0 && commaAfterTemp > t) {
        float temp = received.substring(t + 5, commaAfterTemp).toFloat();
        float hum = received.substring(h + 4).toFloat();
        if (received.startsWith("NODE_1")) {
          Blynk.virtualWrite(V0, temp);
          Blynk.virtualWrite(V1, hum);
        } else if (received.startsWith("NODE_2")) {
          Blynk.virtualWrite(V3, temp);
          Blynk.virtualWrite(V4, hum);
        }
      }
    }
    sendLEDCommand("NODE_1", node1LED);
    sendLEDCommand("NODE_2", node2LED);
  }
}

void sendLEDCommand(String nodeID, int ledState) {
  String cmd = nodeID + ",LED:" + (ledState ? "ON" : "OFF");
  LoRa.beginPacket();
  LoRa.print(cmd);
  LoRa.endPacket();
}

BLYNK_WRITE(V2) { node1LED = param.asInt(); }
BLYNK_WRITE(V5) { node2LED = param.asInt(); }
