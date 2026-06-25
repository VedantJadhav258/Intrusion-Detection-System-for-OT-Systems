# OT-EdgeIDS: Microcontroller-Based Intrusion Detection for Industrial Control Systems

OT-EdgeIDS is a lightweight, edge-deployed Intrusion Detection System (IDS) written in C++ and designed specifically for Operational Technology (OT) networks. It runs on microcontrollers (e.g., ESP32) to provide real-time monitoring and automated mitigation for threats targeting Industrial Control Systems (ICS) and PLCs.

## Key Features

- **Edge-Based Processing**: Runs directly on edge nodes (ESP32), eliminating round-trip latency to centralized systems and enabling faster detection and response.
- **Unauthorized Access Detection**: Uses WiFi promiscuous mode to detect rogue access points, deauthentication attacks, and unauthorized MAC addresses attempting to bridge into OT VLANs.
- **Automated PLC Safe-Mode**: Integrates with Modbus TCP to send emergency "safe-state" commands to PLCs upon detecting critical threats.
- **Real-Time Web Dashboard**: Built-in async web server that serves a static, responsive dashboard for live threat monitoring and manual overrides.
- **Threat Mitigation**: Heuristic and rule-based detection for DoS (SYN/ping floods), unauthorized scanning, and suspicious Modbus payloads.

## Academic & Architectural Context

The architecture is inspired by recent IIoT and industrial security literature (2021–2026). Main themes include:

- Moving packet inspection to the edge to reduce bandwidth and latency.
- Localized Modbus TCP DPI to whitelist allowed function codes and protect critical registers.
- Zero-trust micro-segmentation by using microcontrollers as protective gateways for legacy PLCs.



## Hardware Requirements

- **Microcontroller**: ESP32 (recommended for WiFi promiscuous mode and dual-core processing)
- **Network Interface**: Native WiFi (for wireless monitoring) or W5500 Ethernet module (for wired OT networks)
- **Target Devices**: PLCs supporting Modbus TCP (e.g., Siemens S7, Allen-Bradley) or a Modbus simulator

## Quick Start

1. Clone the repository:

   ```bash
   git clone https://github.com/VedantJadhav258/Intrusion-Detection-System-for-OT-Systems.git
   cd Intrusion-Detection-System-for-OT-Systems
   ```

2. Configure variables: edit `src/main.cpp` to set your OT network SSID/password and the target PLC IP address.

3. Build & flash the firmware using PlatformIO or the Arduino IDE.

4. Upload the dashboard files (e.g., `data/index.html`) to the ESP32 SPIFFS/LittleFS.

5. Open the dashboard in a browser at the ESP32's IP address.

## Incident Response Workflow

1. **Listen**: NetworkMonitor inspects incoming 802.11/Ethernet frames.
2. **Detect**: IdsCore flags anomalies (e.g., SYN floods or unauthorized Modbus writes).
3. **Alert**: Alerts are pushed to the dashboard via WebSocket/SSE.
4. **Act**: PlcController opens a Modbus TCP connection and writes a pre-configured safe-state value to halt processes safely.


---

If you'd like, I can also:
- Add badges (build/license) to the top of the README.
- Expand the docs with a contributing guide and a deployment walkthrough.
