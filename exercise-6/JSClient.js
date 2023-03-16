const net = require('net');

// Simple HTTP server responds with a simple WebSocket client test
const httpServer = net.createServer((connection) => {
    connection.on('data', () => {
        let content = `<!DOCTYPE html>
<html lang="en-us">
  <head>
    <meta charset="UTF-8" /><title>Web Socket</title>
  </head>
  <body>
    WebSocket test page
    <form onsubmit="">
        <input type="text" id="chat-box"/>
        <input type="submit" id="forum"/>
    </form>
    <textarea id="forum"></textarea>
    <script>
      const chatBox = document.getElementById('chat-box');
      const forum = document.getElementById('forum');
      let ws = new WebSocket('ws://localhost:3001');
      ws.onmessage = event => forum.value += 'Server: ' + event.data + '\\n'; 
      ws.onopen = () => ws.send('hello');
      const send_msg = () => {
          ws.send(chatBox.value)
          forum.value += 'Client: ' + chatBox.value + '\\n';
      }
    </script>
  </body>
</html>
`;
        connection.write('HTTP/1.1 200 OK\r\nContent-Length: ' + content.length + '\r\n\r\n' + content);
    });
});
httpServer.listen(3000, () => {
    console.log('HTTP server listening on port 3000');
});