# LoRa-Based IoT Environmental Monitoring System

ESP32 sensor nodes measure temperature and relative humidity with a DHT22 sensor and transmit readings over SX1278 LoRa radios to an ESP32 gateway. The gateway forwards readings to a Blynk dashboard over Wi-Fi and sends LED commands back to the addressed node.

> **Security:** Never commit active Wi-Fi passwords or Blynk tokens. The gateway reads credentials from local `secrets.h`, excluded by `.gitignore`. Copy `firmware/gateway/secrets.example.h` to `firmware/gateway/secrets.h` and fill in values locally. If credentials were previously public, rotate them; a new commit does not remove secrets from earlier Git history or forks.

## Repository layout
- `firmware/sensor_node/sensor_node.ino` — DHT22 sensor node firmware.
- `firmware/gateway/gateway.ino` — LoRa receiver, Wi-Fi/Blynk gateway, LED downlink.
- `firmware/gateway/secrets.example.h` — placeholder-only credential template.
- `docs/academic-report.md` — academic project report in Markdown.
- `docs/README.md` — report notes and submission evidence checklist.

## Hardware and radio settings
- ESP32 development boards, SX1278 LoRa modules, DHT22 sensor.
- DHT22 data: GPIO 27; LED: GPIO 2.
- LoRa: 433 MHz, spreading factor 12, bandwidth 125 kHz, coding rate 4/8.
- NSS/CS GPIO 5; RST GPIO 4; DIO0 GPIO 13; MOSI/MISO/SCK GPIO 23/19/18.

Verify module voltage requirements, wiring, antenna, and local radio regulations before use.

## Arduino setup
Install ESP32 board support and compatible LoRa, DHT, Adafruit Unified Sensor (if required), and Blynk libraries.

### Sensor node
Open `firmware/sensor_node/sensor_node.ino`, select your ESP32 board/port, and upload. The default identifier is `NODE_1`; change it to `NODE_2` and upload to a second node for two-node operation.

### Gateway
1. Copy `firmware/gateway/secrets.example.h` to `firmware/gateway/secrets.h` in the same folder.
2. Fill in Wi-Fi SSID/password and Blynk device token locally. Do not commit `secrets.h`.
3. Configure the Blynk template/device, select the correct ESP32 board/port, and upload `firmware/gateway/gateway.ino`.

### Blynk virtual pins
| Pin | Purpose |
|---|---|
| V0 | Node 1 temperature |
| V1 | Node 1 humidity |
| V2 | Node 1 LED control |
| V3 | Node 2 temperature |
| V4 | Node 2 humidity |
| V5 | Node 2 LED control |

Create matching datastreams/widgets. LED commands are sent after an uplink packet is received, so changes may not be sent immediately.

## Example packet
```text
NODE_1,Temp:24.5,Hum:55.0
```
Illustrative only, not a measured reading. Sender and gateway parsing formats must match.

## Reported testing and limitations
The supplied progress notes report a **315 m open-area range test**, but no packet-loss count, test log, antenna details, or full test conditions were supplied. Treat this as a reported prototype result, not a guaranteed performance claim.

- The supplied firmware does **not** implement AES encryption or packet authentication.
- Parser validation is limited; there is no explicit acknowledgement/retry/sequence-number mechanism.
- Two-node operation and live Blynk behaviour should be tested and documented.
- Recommended measurements: sensor accuracy, packet delivery ratio, RSSI/SNR, control latency, Wi-Fi recovery, and battery runtime.

See [the academic report](docs/academic-report.md).
