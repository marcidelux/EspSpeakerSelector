
#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <WebSocketsServer.h>
#include <vector>

#include "page.hpp"
#include "RelayHandler.hpp"
#include "Memory.hpp"
#include "NetworkSettings.hpp"

ESP8266WebServer server(80);
WebSocketsServer webSocket = WebSocketsServer(81);
std::vector<int> clientIDs;

RelayHandler relayHandler;
NetworkSettings networkSettings;

void handleRoot() {
  server.send_P(200, "text/html", webpageSource);
}

void broadcastTXT(String message) {
  for (int clientId : clientIDs) {
    webSocket.sendTXT(clientId, message);
  }
}

void webSocketEvent(uint8_t num, WStype_t type, uint8_t * payload, size_t length) {
  if (type == WStype_TEXT) {
    int buttonIndex = payload[0] - '0';
    if (buttonIndex >= 0 && buttonIndex < 4) {
      Serial.printf("Html button %d clicked\n", buttonIndex);
      relayHandler.setSpeaker(buttonIndex);

      // Broadcast to all other clients
      String message = String(buttonIndex);
      for (int clientId : clientIDs) {
        if (clientId != int(num)) {
          webSocket.sendTXT(clientId, message);
        }
      }
    }
  }
  else if (type == WStype_CONNECTED) {
    clientIDs.push_back(num);
    String message = String(relayHandler.getSelectedSpeaker());
    Serial.printf("Client %d connected, selectedSpeaker:%s\n", num, message.c_str());
    webSocket.sendTXT(num, message);
  }
  else if (type == WStype_DISCONNECTED) {
    Serial.printf("Client %d disconnected\n", num);
    clientIDs.erase(std::remove(clientIDs.begin(), clientIDs.end(), num), clientIDs.end());
  }
}

void setup() {
  Serial.begin(115200);
  delay(1000);
  Serial.println("Starting..");

  Memory::Init();
  relayHandler.init();
  networkSettings.begin();

  if(networkSettings.GetAPMode()) {
    return;
  }

  String ssid = networkSettings.getSSID();
  String password = networkSettings.getPassword();
  String static_ip = networkSettings.getStaticIP();
  String gateway = networkSettings.getGateway();

  IPAddress IpStatic;
  IpStatic.fromString(static_ip.c_str());
  IPAddress IpGateway;
  IpGateway.fromString(gateway.c_str());

  IPAddress IpSubnet(255, 255, 255, 0);

  if (!WiFi.config(IpStatic, IpGateway, IpGateway)) {
    Serial.println("STA Failed to configure");
  }

  WiFi.begin(ssid, password);
  Serial.print("Connecting to WiFi..");
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.print(".");
  }
  Serial.println("Connected to WiFi");

  server.on("/", handleRoot);
  server.begin();
  Serial.println("HTTP server started");

  webSocket.begin();
  webSocket.onEvent(webSocketEvent);
  Serial.println("WebSocket server started");
}

void loop() {
  static uint32_t lastTime = 0;
  
  // Handle network settings in AP mode
  if (networkSettings.GetAPMode()) {
    networkSettings.handleClient();
  }
  // Handle normal operation
  else {
    server.handleClient();
    webSocket.loop();
    
    if (millis() - lastTime > 500) {
      lastTime = millis();
      relayHandler.measureAndSetSpeaker();
      if(relayHandler.speakerChanged) {
        relayHandler.speakerChanged = false;
        broadcastTXT(String(relayHandler.getSelectedSpeaker()));
      }
    }
  }
}