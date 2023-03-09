const express = require('express');
const path = require('path');
const app = express();
const bodyParser = require('body-parser');
const { spawn } = require('child_process');
const fs = require('fs').promises;

app.use(express.static(path.join(__dirname, 'client/build')))
app.use(bodyParser.json())

// The endpoint of the API
app.post('/compile', (req, res) => {
    const userCode = req.body.code;

    try{
        fs.writeFile('main.cpp', userCode)
            .then(r => console.log('Main.cpp created!'));
    } catch (error) {
        console.log('Error', error)
    }
    //docker build -t comp . && docker run --rm comp
    const process = spawn('docker', [
        'run',
        '--rm',
        'comp'
    ]);

    process.stdout.on('data', data => {
        console.log(`stdout: ${data}`);
        res.send(data);
    });

    process.stderr.on('data', data => {
        console.error(`stderr: ${data}`);
    });

    process.on('close', code => {
        try{
            fs.unlink('./main.cpp')
                .then(r => console.log('Compiled File Removed.'));
        } catch (error) {
            console.log('Error', error)
        }
        console.log(`Child process exited with code ${code}`);
    });


});



app.get('*', (req, res) => {
    res.sendFile(path.join(__dirname+'/client/build/index.html'));
});

const port = 8082;
app.listen(port, () => {
    console.log(`Server is listening on port ${port}`);
});
