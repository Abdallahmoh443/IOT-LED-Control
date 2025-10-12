require('dotenv').config();

const path = require('path');
const express = require('express');
const app = express(); // Express application instance

// the server that listens on a port and receives requests.
const http = require('http');
// the actual network server instance created by Node’s http module
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

// ws is the connected socket
wss.on('connection', (ws) => {
    console.log("Connected to a WebSocket");

    ws.on('message', (data) => {
      const msg = JSON.parse(data);
      console.log("Received Data: ", msg);
      
      let i = 1;

      wss.clients.forEach((client) =>{
        if (client.readyState === WebSocket.OPEN){ // Connected client
          client.send(JSON.stringify(msg));
          console.log('Client: ', i);
          i++;
        }
      });
    });
});