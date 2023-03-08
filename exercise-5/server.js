const express = require('express');
const path = require('path');
const app = express();
const { spawn } = require('child_process');

app.use(express.static(path.join(__dirname, 'client/build')))

// The endpoint of the API
app.post('/compile', (req, res) => {
    const code = req.body;
    res.send("Code received!");
});



app.get('*', (req, res) => {
    res.sendFile(path.join(__dirname+'/client/build/index.html'));
});

const port = 8082;
app.listen(port, () => {
    console.log(`Server is listening on port ${port}`);
});
