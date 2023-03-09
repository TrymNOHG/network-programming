const express = require('express');
const path = require('path');
const app = express();
const bodyParser = require('body-parser');
const { execSync, spawn } = require('child_process');
const fs = require('fs').promises;

app.use(express.static(path.join(__dirname, 'client/build')))
app.use(bodyParser.json())

// The endpoint of the API
app.post('/compile', async (req, res) => {
    const userCode = req.body.code;

    try{
        await fs.writeFile('main.cpp', userCode)
            .then(r => console.log('Main.cpp successfully created!'));
    } catch (error) {
        console.log('Error', error)
    }
    //docker build -t comp . && docker run --rm comp
    execSync('docker build . -t comp');

    const process = spawn('docker', [
        'run',
        '--rm',
        'comp'
    ]);

    process.stdout.on('data', async data => {
        console.log(`stdout: ${data}`);
        res.send(data);
    });

    process.stderr.on('data', data => {
        console.error(`stderr: ${data}`);
    });

    process.on('close', code => {
        try{
            fs.unlink('./main.cpp')
                .then(r => console.log('Main.cpp has been removed'))
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
