# 🔌 IoT LED Control Dashboard

## Overview

A comprehensive IoT system for remote LED control using WebSocket communication. This project enables real-time control of three LEDs connected to an ESP32 microcontroller through a responsive web dashboard. Built with Node.js, Express, WebSockets, and Arduino firmware.

**University Project** | Under Supervision of Eng. Osama Samir

---


## Features

### Dashboard Features
- **Real-time LED Control**: Toggle LEDs on/off instantly
- **Brightness Control**: Adjust LED intensity with smooth slider (0-255)
- **Live Status Updates**: Visual feedback for ESP32 connection status
- **Responsive Design**: Works on desktop, tablet, and mobile devices
- **Elegant UI**: Modern gradient background with smooth animations
- **Visual Indicators**: LED bulb icons that glow with intensity changes

### Backend Features
- **WebSocket Communication**: Bi-directional real-time data exchange
- **Connection Status Monitoring**: Automatic detection of ESP32 connectivity
- **State Management**: Persistent LED state tracking
- **Auto-reconnection**: ESP32 reconnection detection with timeout handling
- **Broadcast Messages**: Updates sent to all connected clients

### ESP32 Features
- **WiFi Connectivity**: Connects to existing WiFi networks
- **WebSocket Client**: Maintains persistent connection to server
- **JSON Message Handling**: Robust message parsing and validation
- **PWM Control**: Precise LED brightness adjustment
- **Ping/Pong Heartbeat**: Connection health monitoring
- **Serial Logging**: Detailed debug output for troubleshooting

---

##  System Architecture

```
┌─────────────────────────────────────────────────────┐
│                   Dashboard (Browser)                │
│  - HTML/CSS Frontend                                │
│  - Real-time UI Updates                             │
│  - WebSocket Client                                 │
└────────────────┬──────────────────────────────────┘
                 │
        WebSocket Connection (ws://)
                 │
┌────────────────▼──────────────────────────────────┐
│            Node.js Server (Express)                │
│  - HTTP Server                                     │
│  - WebSocket Server (ws)                           │
│  - Connection Management                           │
│  - State Broadcasting                              │
└────────────────┬──────────────────────────────────┘
                 │
        WebSocket Connection (ws://)
                 │
┌────────────────▼──────────────────────────────────┐
│           ESP32 Microcontroller                    │
│  - WiFi Module                                     │
│  - JSON Message Parser                             │
│  - PWM LED Controllers                             │
│  - 3× LED Outputs (GPIO 5, 18, 19)               │
└─────────────────────────────────────────────────────┘
```

---

## Hardware Requirements

### Components
- **ESP32 Development Board** (NodeMCU style)
- **3× LED Diodes** (any color)
- **3× 220Ω Resistors** (current limiting)
- **Breadboard & Jumper Wires**
- **USB Cable** (for ESP32 programming)

### Wiring Diagram

```
ESP32 GPIO 5  ──[Resistor]──> LED 1 Anode
ESP32 GPIO 18 ──[Resistor]──> LED 2 Anode
ESP32 GPIO 19 ──[Resistor]──> LED 3 Anode

LED Cathodes ──> GND (Common Ground)
```

### Network Requirements
- WiFi network (2.4GHz recommended for ESP32)
- Direct connection or same local network between server and ESP32

---

## Software Requirements

### Server Environment
- **Node.js** v18 or higher
- **npm** (Node Package Manager)

### Required Dependencies
```json
{
  "express": "^5.1.0",
  "ws": "^8.18.3",
  "dotenv": "^17.2.3"
}
```

### ESP32 Libraries
- `WiFi.h` (ESP32 core library)
- `ArduinoWebsockets.h` (WebSocket client)
- `ArduinoJson.h` (JSON serialization)

### Browser Requirements
- Modern browser with WebSocket support
- ES6 JavaScript support

---


## Project Details

**Academic Context:** This project is part of a university curriculum focused on IoT systems and embedded networking.

**Under Supervision of:** Eng. Osama Samir

**Key Learning Outcomes:**
- WebSocket communication protocol
- Real-time web applications
- Embedded systems programming
- Network connectivity
- Full-stack development

---



*Built with Node.js, Express, WebSockets, and Arduino*