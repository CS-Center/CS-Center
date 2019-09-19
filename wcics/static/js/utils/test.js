var socket = io();
socket.on('connection-established', (message, e) => {
  alert(message);
  console.log(e);
});