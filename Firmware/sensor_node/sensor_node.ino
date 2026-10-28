#include <SPI.h>
#include <LoRa.h>
#include <DHT.h>

#define NODE_ID "NODE_1"  // Change to "NODE_2" for a second node
#define DHTPIN 27
#define DHTTYPE DHT22
#define LED_PIN 2
#define LORA_SS 5
#define LORA_RST 4
#define LORA_DIO0 13

DHT dht(DHTPIN, DHTTYPE);

void setup() {
  Serial.begin(115200);
  dht.begin();
  pinMode(LED_PIN, OUTPUT);
  digitalWrite(LED_PIN, LOW);

  LoRa.setPins(LORA_SS, LORA_RST, LORA_DIO0);
  if (!LoRa.begin(433E6)) {
    Serial.println("LoRa init failed");
    while (1) delay(1000);
  }

  LoRa.setSpreadingFactor(12);
  LoRa.setSignalBandwidth(125E3);
  LoRa.setCodingRate4(8);
  Serial.println(String(NODE_ID) + " ready");
}

void loop() {
  float temperature = dht.readTemperature();
  float humidity = dht.readHumidity();

  if (!isnan(temperature) && !isnan(humidity)) {
    String data = String(NODE_ID) + ",Temp:" + temperature + ",Hum:" + humidity;
    LoRa.beginPacket();
    LoRa.print(data);
    LoRa.endPacket();
    Serial.println("Sent: " + data);
  }

  if (LoRa.parsePacket()) {
    String cmd = "";
    while (LoRa.available()) cmd += (char)LoRa.read();
    Serial.println("Command: " + cmd);

    if (cmd.startsWith(NODE_ID)) {
      if (cmd.indexOf("LED:ON") > 0) digitalWrite(LED_PIN, HIGH);
      else if (cmd.indexOf("LED:OFF") > 0) digitalWrite(LED_PIN, LOW);
    }
  }

  delay(5000);
}
