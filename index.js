require('dotenv').config();

const path = require('path');
const express = require('express');
const app = express(); // Express application instance

// the server that listens on a port and receives requests.
const http = require('http');
// the actual network server instance created by Node's http module
const server = http.createServer(app); 

// WebSocket (ws) needs a server to attach to
// it uses the same underlying HTTP connection
const SocketServer = require('ws').Server;
const wss = new SocketServer({server});


// Server static files from the "static" directory at the web root
app.use(express.static(path.join(__dirname, 'static')));

const port = process.env.PORT || 3000;

app.get('/', (req, res) => {
  res.sendFile('dashboard.html', { root: path.join(__dirname, 'static') });
});


server.listen(port, () => {
  console.log(`listening at http://localhost:${port}`);
});

let leds = [
  {status: false, intensity: 0},
  {status: false, intensity: 0},
  {status: false, intensity: 0}
];

// Track last ping time from ESP32
let lastEsp32Ping = null;
const ESP32_TIMEOUT = 10000; // 10 Seconds

// ws is the connected socket
wss.on('connection', (ws) => {
    console.log("Connected to a WebSocket");

    // Send initial state to the new client
    ws.send(JSON.stringify({
        type: "init",
        leds: leds
    }));

    // Communicate to all clients the current connection status with ESP32
    ws.send(JSON.stringify({
        type: "connection_status",
        esp32Connected: lastEsp32Ping && (Date.now() - lastEsp32Ping <= ESP32_TIMEOUT)
    }));

    ws.on('message', (data) => {
      const msg = JSON.parse(data);
      console.log("Received Data: ", msg);
      
      // تحديث وقت آخر ping من ESP32
      if (msg.type === "esp32_ping") {
          lastEsp32Ping = Date.now();
          
          // Communicate to all clients that ESP32 is connected
          wss.clients.forEach((client) => {
              if (client.readyState === 1) {
                  client.send(JSON.stringify({
                      type: "connection_status",
                      esp32Connected: true
                  }));
              }
          });
          return;
      }
      
      // Update LED state
      if (msg.id && msg.id >= 1 && msg.id <= 3) {
          leds[msg.id-1].status = msg.status;
          leds[msg.id-1].intensity = msg.intensity;

          // Broadcast to all connected clients
          wss.clients.forEach((client) => {
              if (client.readyState === 1) { // OPEN = 1
                  client.send(JSON.stringify(msg));
              }
          });
      }
    });

    ws.on('close', () => {
        console.log("Client disconnected");
    });
});

setInterval(() => {
  if (lastEsp32Ping && (Date.now() - lastEsp32Ping > ESP32_TIMEOUT)) {
    lastEsp32Ping = null;
    
    wss.clients.forEach((client) => {
      if (client.readyState === 1) {
        client.send(JSON.stringify({
          type: "connection_status",
          esp32Connected: false
        }));
      }
    });
  }
}, 5000); 
