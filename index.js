require('dotenv').config();

const path = require('path');

const express = require('express');
const app = express();

// Serve static files from the "static" directory at the web root
app.use(express.static(path.join(__dirname, 'static')));

const port = process.env.PORT || 3000;

app.get('/dashboard', (req, res) => {
  res.sendFile('dashboard.html', { root: path.join(__dirname, 'static') });
});


app.listen(port, () => {
  console.log(`Example app listening at http://localhost:${port}`);
});