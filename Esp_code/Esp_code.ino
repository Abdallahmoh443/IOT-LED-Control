#include <WiFi.h>                   // Library for WiFi connection
#include <ArduinoWebsockets.h>      // Library for WebSocket client
#include <ArduinoJson.h>            // Library for handling JSON data

using namespace websockets;         // Use the WebSocket namespace

// WiFi credentials
const char* WIFI_SSID = "Horizon";
const char* WIFI_PASSWORD = "872004mokadry@";

// WebSocket server URL
const char* SERVER_URL = "ws://192.168.1.7:3000"; 

WebsocketsClient client;            // WebSocket client instance

// LED pins
#define LED1 5
#define LED2 18
#define LED3 19

// Variables to store LED states
bool led1State = false, led2State = false, led3State = false;
int intensity1 = 0, intensity2 = 0, intensity3 = 0;

// Variables to track Ping
unsigned long lastPingTime = 0;
const unsigned long PING_INTERVAL = 5000; // Send ping every 5 seconds

// Callback function when a message is received from the server
void onMessageCallback(WebsocketsMessage message) {
  String msg = message.data();       // Get the message as a string
  Serial.print("Message from server: ");
  Serial.println(msg);

  // Parse the JSON message
  DynamicJsonDocument doc(1024);
  DeserializationError error = deserializeJson(doc, msg);
  
  if (error) {
    Serial.print("JSON parsing failed: ");
    Serial.println(error.c_str());
    return;                          // Exit if JSON is invalid
  }

  // Extract data from JSON
  int id = doc["id"];                // LED id (1, 2, or 3)
  bool status = doc["status"];       // LED on/off
  int intensity = doc["intensity"];  // LED brightness

  Serial.print(" Parsed - ID: ");
  Serial.print(id);
  Serial.print(", Status: ");
  Serial.print(status);
  Serial.print(", Intensity: ");
  Serial.println(intensity);

  // Update corresponding LED state
  if(id >= 1 && id <= 3) {
    switch(id) {
      case 1:
        led1State = status;
        intensity1 = intensity;
        analogWrite(LED1, led1State ? intensity1 : 0); // Set LED brightness
        Serial.println(" LED 1 updated");
        break;
      case 2:
        led2State = status;
        intensity2 = intensity;
        analogWrite(LED2, led2State ? intensity2 : 0);
        Serial.println(" LED 2 updated");
        break;
      case 3:
        led3State = status;
        intensity3 = intensity;
        analogWrite(LED3, led3State ? intensity3 : 0);
        Serial.println(" LED 3 updated");
        break;
    }

    sendStatusUpdate();              // Send updated status back to server
  }
}

// Function to send current LED states to the server
void sendStatusUpdate() {
  DynamicJsonDocument doc(512);
  doc["id1"] = led1State;
  doc["intensity1"] = intensity1;
  doc["id2"] = led2State;
  doc["intensity2"] = intensity2;
  doc["id3"] = led3State;
  doc["intensity3"] = intensity3;
  doc["type"] = "status_update";   // Include message type

  String jsonStr;
  serializeJson(doc, jsonStr);     // Convert JSON object to string
  
  client.send(jsonStr);            // Send JSON to server
  Serial.print(" Sent status: ");
  Serial.println(jsonStr);
}

// Callback for WebSocket events
void onEventsCallback(WebsocketsEvent event, String data) {
  if (event == WebsocketsEvent::ConnectionOpened) {
    Serial.println(" Connected to WebSocket server!");
    sendStatusUpdate();            // Send initial status
  }
  if (event == WebsocketsEvent::ConnectionClosed) {
    Serial.println("  WebSocket Disconnected!");
  }
  if (event == WebsocketsEvent::GotPing) {
    Serial.println("  Got Ping");  // Server ping received
  }
  if (event == WebsocketsEvent::GotPong) {
    Serial.println("  Got Pong");  // Response to ping received
  }
}

void setup() {
  Serial.begin(115200);
  delay(500);

  // Initialize LED pins as outputs
  pinMode(LED1, OUTPUT);
  pinMode(LED2, OUTPUT);
  pinMode(LED3, OUTPUT);

  // Turn off all LEDs initially
  analogWrite(LED1, 0);
  analogWrite(LED2, 0);
  analogWrite(LED3, 0);

  // Connect to WiFi
  Serial.println();
  Serial.print("Connecting to WiFi: ");
  Serial.println(WIFI_SSID);
  
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  while (WiFi.status() != WL_CONNECTED) { 
    delay(500); 
    Serial.print("."); 
  }
  
  Serial.println("\nWiFi connected!");
  Serial.print("IP Address: "); 
  Serial.println(WiFi.localIP());

  // Set WebSocket callbacks
  client.onMessage(onMessageCallback);
  client.onEvent(onEventsCallback);
  
  Serial.print("Connecting to WebSocket: ");
  Serial.println(SERVER_URL);
  
  // Connect to WebSocket server
  if (client.connect(SERVER_URL)) {
    Serial.println("WebSocket Connected!");
  } else {
    Serial.println("WebSocket Connection Failed!");
  }
}

void loop() {
  client.poll();  // Handle incoming messages

  // Periodically send LED status every 10 seconds
  static unsigned long lastSend = 0;
  if (millis() - lastSend > 10000) {
    lastSend = millis();
    if (client.available()) {
      sendStatusUpdate();
    } else {
      Serial.println("WebSocket not available, trying to reconnect...");
      if (client.connect(SERVER_URL)) {
        Serial.println("Reconnected to WebSocket!");
      }
    }
  }
  
  // Regularly send ping to server every 5 seconds
  if (millis() - lastPingTime > PING_INTERVAL) {
    lastPingTime = millis();
    if (client.available()) {
      DynamicJsonDocument pingDoc(128);
      pingDoc["type"] = "esp32_ping";   // Ping type
      String pingStr;
      serializeJson(pingDoc, pingStr);
      client.send(pingStr);
      Serial.println("Sent ESP32 ping");
    }
  }
  
  delay(100);  // Small delay to avoid busy loop
}
