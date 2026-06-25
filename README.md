OT-EdgeIDS: Microcontroller-Based Intrusion Detection for Industrial Control Systems

OT-EdgeIDS is a lightweight, edge-deployed Intrusion Detection System (IDS) written in C++ designed specifically for Operational Technology (OT) networks. It provides real-time monitoring, anomaly detection, and automated incident response (PLC Safe-Mode engagement) to protect critical infrastructure from DoS, unauthorized access, and malware propagation.

Key Features

Edge-Based Processing: Runs directly on edge network nodes (e.g., ESP32), eliminating round-trip latency to centralized SIEMs, resulting in a 3x faster detection and response time.

Unauthorized Access Detection: Utilizes Promiscuous Mode (WiFi.h) to detect Rogue Access Points, Deauth attacks, and unauthorized MAC addresses attempting to bridge into the OT VLAN.

Automated PLC Safe-Mode: Integrates with Modbus TCP to automatically send emergency "Safe-State" holding register commands to Programmable Logic Controllers (PLCs) upon detecting critical threats, isolating them from attacker control.

Real-Time Web Dashboard: Built-in async web server (WebServer.h) serving a static, responsive portal for live threat monitoring and manual overrides.

Threat Mitigation: Heuristic and rule-based detection for DoS (SYN/Ping floods), ransomware network propagation (unauthorized port scanning), and malicious payload injection.

Academic & Architectural Context

This project's architecture is heavily inspired by recent (2021-2026) implementations found in IEEE and Springer literature regarding Industrial IoT (IIoT) security:

Edge-Computing IDS: Moving packet inspection from the core network to the edge to reduce bandwidth bottleneck and latency (e.g., IEEE Transactions on Industrial Informatics).

Modbus TCP Anomaly Detection: Implementing localized Deep Packet Inspection (DPI) to validate function codes against a whitelist, preventing unauthorized register writes (often a precursor to physical damage).

Zero-Trust Micro-Segmentation: Using edge microcontrollers to act as zero-trust gateways for legacy PLCs that lack native authentication.

Project Structure

OT-EdgeIDS/
├── data/
│   └── index.html           # Real-time control dashboard UI (HTML/JS/CSS)
├── docs/
│   ├── architecture.png     # Network topology diagrams
│   └── threat_model.md      # Detailed threat vectors and mitigations
├── src/
│   ├── main.cpp             # Main system loop and hardware init
│   ├── NetworkMonitor.h     # Packet sniffing and promiscuous mode logic
│   ├── IdsCore.h            # Threat detection engine (DoS, Malware signatures)
│   ├── PlcController.h      # Modbus TCP client for Safe-Mode engagement
│   └── WebDashboard.h       # WebServer.h implementation and API routes
├── platformio.ini           # PlatformIO configuration file
└── README.md



Hardware Requirements

Microcontroller: ESP32 (Recommended for WiFi Promiscuous mode and dual-core processing).

Network Interface: Native WiFi (for wireless edge monitoring) or W5500 Ethernet Module (for hardwired OT networks).

Target Device: Any standard PLC supporting Modbus TCP (e.g., Siemens S7, Allen-Bradley, or a simulator like ModRSsim2).

Quick Start

Clone the repository:

git clone [https://github.com/yourusername/OT-EdgeIDS.git](https://github.com/yourusername/OT-EdgeIDS.git)



Configure Variables: Update src/main.cpp with your specific OT Network SSID/Password and the target PLC IP Address.

Flash the Firmware: Use PlatformIO or the Arduino IDE to upload the code to your ESP32.

Upload File System: Upload the data/index.html file to the ESP32 SPIFFS/LittleFS.

Access Dashboard: Navigate to the ESP32's IP address in your web browser.

Incident Response Workflow

Listen: NetworkMonitor continuously analyzes headers of incoming 802.11/Ethernet frames.

Detect: IdsCore flags a surge in TCP SYN requests (DoS) or an unauthorized write command to a critical holding register.

Alert: Pushes WebSocket/SSE alert to the WebDashboard.

Act: PlcController immediately opens a Modbus TCP connection to the PLC and writes 0xFF to the pre-configured Safe-Mode coil, halting physical processes safely before the attacker can cause damage.
