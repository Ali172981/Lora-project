# Academic Project Report
## Secure LoRa-Based IoT Environmental Monitoring System

**Platform:** ESP32 sensor nodes • SX1278 LoRa • Gateway • Blynk Cloud

**Student Name:** Muhammad Ali  
**Department / Programme:** BS Physics  
**Institution:** BUITEMS  
**Supervisor / Professor:** DR.Rafaqat hussain  
**Submission Date:** 2025

*Prepared from the supplied project progress document and firmware files.*

## Abstract

This project documents a wireless environmental monitoring system based on ESP32 microcontrollers and LoRa radio communication. Sensor nodes use a DHT22 sensor to measure temperature and relative humidity, then transmit text-formatted readings over a 433 MHz LoRa link. A gateway ESP32 receives node messages and forwards parsed values to the Blynk cloud dashboard over Wi-Fi. Node 1 readings are mapped to Blynk virtual pins V0 and V1; Node 2 readings use V3 and V4. Dashboard controls on V2 and V5 set LED states that the gateway sends back to the nodes through LoRa. The supplied progress document records an earlier open-area range test of 315 metres and an earlier Bluetooth-based prototype. This report distinguishes implemented behaviour from reported progress and recommendations.

**Security status:** AES encryption is not present in the supplied sender and gateway code; the current implementation is not described as AES-protected.

## 1. Aim and Objectives

**Aim:** Implement and document an ESP32-based wireless environmental monitoring system that transports temperature and humidity readings over LoRa and presents them through Blynk.

Objectives: read temperature/humidity from a DHT22; transmit readings over 433 MHz LoRa; receive packets at an ESP32 gateway; forward parsed values to Blynk over Wi-Fi; provide dashboard-controlled LED states through return LoRa messages; and record evidence and remaining tests.

## 2. System Architecture

The architecture has three layers: sensor nodes, a LoRa gateway, and a cloud dashboard. A node reads the DHT22 and transmits a message containing its identifier, temperature, and humidity. The gateway parses the packet and updates Blynk. Blynk callbacks update stored LED states; after a LoRa packet is received, the gateway sends a command for each node. Each node checks the command identifier before switching its LED.

LoRa carries packets between local devices; Wi-Fi and the internet provide the gateway's route to Blynk.

## 3. Components and Connections

| Component | Role |
|---|---|
| ESP32 development boards | Sensor node(s) and gateway |
| SX1278 LoRa modules | Local wireless radio link |
| DHT22 | Temperature/humidity; data on GPIO 27 |
| LED / GPIO 2 | Node LED and gateway packet indicator |
| Wi-Fi | Gateway connectivity to Blynk |
| Blynk | Cloud datastreams and LED controls |
| Power prototype | Notes list Li-ion cell, TP14 charging point, 3 V-to-7 V converter, and cell case; ratings were not supplied |

| Signal / parameter | Value |
|---|---|
| LoRa frequency | 433 MHz (`433E6`) |
| Spreading factor | 12 |
| Bandwidth | 125 kHz |
| Coding rate | 4/8 |
| NSS / CS | GPIO 5 |
| RST | GPIO 4 |
| DIO0 | GPIO 13 |
| MOSI / MISO / SCK | GPIO 23 / 19 / 18 |
| DHT22 data | GPIO 27 |
| LED | GPIO 2 |

Verify exact breakout pin labels, supply-voltage requirements, antenna connection, common ground, and local radio regulations before powering or transmitting.

## 4. Firmware and Message Format

The sensor-node firmware initializes the DHT library and LoRa, reads temperature and humidity, transmits valid readings, checks received commands, and waits five seconds between loops. The supplied node ID is `NODE_1`; a second node must be flashed with the ID changed to `NODE_2`.

Illustrative example only, not a measured reading:

```text
NODE_1,Temp:24.5,Hum:55.0
```

The gateway uses matching LoRa settings, receives packet text, blinks GPIO 2, parses the Temp/Hum fields, and updates Blynk. It sends `NODE_1,LED:ON/OFF` and `NODE_2,LED:ON/OFF` commands after handling an uplink packet. A dashboard state change is not necessarily transmitted immediately.

## 5. Blynk Mapping

| Virtual pin | Purpose |
|---|---|
| V0 | Node 1 temperature |
| V1 | Node 1 humidity |
| V2 | Node 1 LED control |
| V3 | Node 2 temperature |
| V4 | Node 2 humidity |
| V5 | Node 2 LED control |

Configure matching Blynk datastreams and widgets. Successful live-cloud operation was not independently demonstrated by the supplied files.

## 6. Testing and Recorded Results

| Test / prototype | Result in progress notes | Evidence status |
|---|---|---|
| Basic LoRa sender/receiver range test | 315 metres in open area | Reported; no test log, antenna details, packet-loss count, or full conditions supplied |
| DHT22 LoRa prototype | Readings transmitted over LoRa and Bluetooth in an earlier prototype | Notes and code supplied; no calibrated comparison data |
| Bluetooth extraction | Android Bluetooth serial-terminal app reportedly extracted data | No screenshots/logs supplied |
| Blynk gateway | Firmware maps readings and supports LED callbacks | Visible in code; live cloud operation not independently demonstrated |

No additional numerical results are invented.

### Recommended validation plan

| Test | Procedure | Record |
|---|---|---|
| Sensor accuracy | Compare with reference thermometer/hygrometer | Reference and sensor values, absolute error, time |
| Range/delivery | Repeat at known distances | Distance, packets sent/received, delivery ratio, RSSI/SNR |
| Cloud forwarding | Check each node's datastream | Timestamp, node ID, expected pin, displayed value |
| LED control | Toggle V2/V5 repeatedly | Latency, success count, wrong-node activations |
| Wi-Fi recovery | Disconnect and restore Wi-Fi | Recovery time, resets, lost readings |
| Endurance | Run for a defined duration | Uptime, missing packets, resets, current draw |
| Power subsystem | Measure voltage/current | Battery capacity, average current, runtime, charging behaviour |

## 7. Security, Reliability, and Limitations

The supplied node and gateway firmware contain no AES library, encryption/decryption routines, key management, nonce/IV handling, or authentication tag. The sensor message is transmitted as readable text. AES is therefore not implemented in the supplied code reviewed for this report.

The original supplied gateway source contained Wi-Fi credentials and a Blynk token in plaintext. Those values are omitted from the sanitized firmware. If credentials are live, rotate the Blynk token and change the Wi-Fi password where appropriate. Keep local secrets in an ignored file. A later commit does not remove secrets from prior Git history or forks.

Other limitations: packet parsing assumes a particular message structure; blocking delays and synchronous transmission can affect command timing; there is no explicit acknowledgement, retransmission, sequence number, or duplicate detection; two-node operation and live Blynk behaviour need documented tests. The progress notes report a 315 m range test but do not include full experimental conditions or packet-loss data.

## 8. Recommended Improvements

| Area | Recommended improvement |
|---|---|
| Encryption | Implement authenticated encryption if required; document key provisioning and test tampering/replay |
| Credentials | Rotate exposed values and keep secrets out of source control/history where practical |
| Packet format | Add versioning, validation, sequence number, and range checks |
| Delivery | Add bounded retries, timeouts, and packet counters |
| Two nodes | Flash NODE_2 and test collisions/addressing |
| Downlink | Design command scheduling and measure latency |
| Range evidence | Document antenna, height, obstructions, packet count, RSSI/SNR, and loss rate |
| Power | Measure consumption, runtime, charging, and protection behaviour |

## 9. Conclusion

The supplied files document a prototype for environmental monitoring using ESP32 nodes, DHT22 sensors, SX1278 LoRa modules, and a Blynk-connected gateway. The firmware configures a 433 MHz radio link, transmits temperature and humidity readings, maps two node identifiers to separate Blynk pins, and supports remote LED commands. Progress notes report a 315-metre open-area range test and an earlier Bluetooth-based prototype.

Further evidence is required before final submission: AES is absent from the supplied firmware; two-node operation and live Blynk behaviour need documented tests; and the range result needs fuller experimental conditions. Add reproducible test logs, circuit/PCB images, and Blynk screenshots.

## References and Source Files

1. *Project updates.docx* — supplied progress document with wiring tables, prototype code, and reported test notes.
2. *node 1 - sender.txt* — supplied ESP32 node firmware using LoRa and DHT libraries.
3. *gateway - receiver.txt* — supplied ESP32 gateway firmware using LoRa, Wi-Fi, and Blynk.
4. Repository: https://github.com/Ali172981/Lora-project

### Submission checklist
- [ ] Fill in cover-page placeholders.
- [ ] Add actual circuit/PCB photos and Blynk screenshots.
- [ ] Rotate any exposed credentials.
- [ ] Implement and test AES only if encryption is a project requirement.
- [ ] Record sensor accuracy, range, packet loss, Blynk updates, LED control, Wi-Fi recovery, and power tests.
- [ ] Ensure evidence matches the final hardware and firmware revision.
