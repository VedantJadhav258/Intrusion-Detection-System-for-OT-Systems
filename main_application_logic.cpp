#include <WiFi.h>
#include <WebServer.h>
#include <esp_wifi.h>

// --- Configuration ---
const char* ssid = "OT_VLAN_SECURE";
const char* password = "SuperSecretPassword123!";
const IPAddress plc_ip(192, 168, 1, 50); // Target PLC IP
const int PLC_MODBUS_PORT = 502;

WebServer server(80);

// --- Global State ---
int alertCount = 0;
String latestThreat = "None";
bool plcSafeModeEngaged = false;
unsigned long lastPacketTime = 0;
int packetRate = 0;

// --- Mock IDS Logic ---
// In a real implementation, this would involve deep packet inspection 
// or evaluating WiFi Promiscuous mode frames for Deauth/Rogue APs.

void analyzeTraffic() {
    // Simulating DoS / Unauthorized Access detection
    // E.g., analyzing frequency of connection attempts
    int simulatedTrafficSpike = random(0, 100); 
    
    if (simulatedTrafficSpike > 95 && !plcSafeModeEngaged) {
        latestThreat = "CRITICAL: High-Frequency Connection Anomalies (Potential DoS/Unauthorized Access)";
        alertCount++;
        Serial.println(latestThreat);
        triggerPlcSafeMode();
    }
}

// --- PLC Control (Modbus TCP Simulation) ---
void triggerPlcSafeMode() {
    Serial.println(">>> INTRUSION DETECTED. ENGAGING PLC SAFE MODE <<<");
    
    // Pseudo-code for Modbus TCP Safe-mode command:
    // 1. Connect to plc_ip on port 502
    // 2. Send Modbus Function Code 05 (Write Single Coil) or 06 (Write Single Register)
    // 3. Set the specific safe-state register designated by plant engineers
    
    WiFiClient client;
    if (client.connect(plc_ip, PLC_MODBUS_PORT)) {
        Serial.println("Connected to PLC. Sending Emergency Stop Signal...");
        // Example Modbus TCP payload for Write Single Coil
        // byte payload[] = {0x00, 0x01, 0x00, 0x00, 0x00, 0x06, 0x01, 0x05, 0x00, 0x10, 0xFF, 0x00};
        // client.write(payload, sizeof(payload));
        client.stop();
        plcSafeModeEngaged = true;
        Serial.println("PLC Isolated and in Safe Mode.");
    } else {
        Serial.println("FAILED to connect to PLC for Safe Mode engagement!");
    }
}

// --- Web Server Routes ---
void handleRoot() {
    // In production, this serves the index.html from SPIFFS/LittleFS
    // For this example, we return a simple instruction to use the API
    server.send(200, "text/plain", "OT IDS Edge Node Active. Navigate to /api/status for JSON.");
}

void handleStatus() {
    String json = "{";
    json += "\"alerts\":" + String(alertCount) + ",";
    json += "\"latestThreat\":\"" + latestThreat + "\",";
    json += "\"safeMode\":" + String(plcSafeModeEngaged ? "true" : "false");
    json += "}";
    
    server.sendHeader("Access-Control-Allow-Origin", "*");
    server.send(200, "application/json", json);
}

void handleReset() {
    plcSafeModeEngaged = false;
    latestThreat = "System Reset by Admin";
    Serial.println("System reset via Dashboard.");
    server.sendHeader("Access-Control-Allow-Origin", "*");
    server.send(200, "application/json", "{\"status\":\"reset\"}");
}

void setup() {
    Serial.begin(115200);
    Serial.println("\nInitializing OT Edge IDS...");

    // 1. Connect to Network
    WiFi.begin(ssid, password);
    while (WiFi.status() != WL_CONNECTED) {
        delay(500);
        Serial.print(".");
    }
    Serial.println("\nConnected to OT Network. Edge IDS IP: " + WiFi.localIP().toString());

    // Optional: Enable Promiscuous Mode here using esp_wifi_set_promiscuous(true)
    // to sniff for unauthorized MACs and Rogue APs.

    // 2. Setup Web Dashboard
    server.on("/", handleRoot);
    server.on("/api/status", handleStatus);
    server.on("/api/reset", HTTP_POST, handleReset);
    server.begin();
    Serial.println("Web Portal active on port 80");
}

void loop() {
    server.handleClient();
    
    // Run anomaly detection periodically
    if (millis() - lastPacketTime > 2000) {
        analyzeTraffic();
        lastPacketTime = millis();
    }
}