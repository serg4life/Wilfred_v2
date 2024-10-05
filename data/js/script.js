// Conexión WebSocket al servidor (reemplazar con la IP del servidor)
const socket = new WebSocket('ws://192.168.4.1:8080'); // Asegúrate de que el servidor esté en ws:// o wss://

// Función para enviar comandos al servidor
function sendCommand(command) {
    if (socket.readyState === WebSocket.OPEN) {
        socket.send(JSON.stringify({ command }));
    }
}

// Asignación de los botones a variables
const forwardButton = document.getElementById('forward');
const backwardButton = document.getElementById('backward');
const leftButton = document.getElementById('left');
const rightButton = document.getElementById('right');
const stopButton = document.getElementById('stop');
const switchElement = document.getElementById('switch');

switchElement.addEventListener('change', function(){
    if (switchElement.checked) {
        sendCommand('motors:enable');
    } else {
        sendCommand('motors:disable');
    }
});

// Eventos de mantener presionado y soltar los botones
function setupButton(button, command) {
    button.addEventListener('mousedown', () => sendCommand(command));
    button.addEventListener('mouseup', () => sendCommand('motors:stop'));
    button.addEventListener('touchstart', () => sendCommand(command)); // Para dispositivos táctiles
    button.addEventListener('touchend', () => sendCommand('motors:stop'));   // Para dispositivos táctiles
}

// Configurar los botones
setupButton(forwardButton, 'forward');
setupButton(backwardButton, 'backward');
setupButton(leftButton, 'left');
setupButton(rightButton, 'right');
setupButton(stopButton, 'motors:stop');

// Manejar los mensajes entrantes del servidor (por ejemplo, estados o respuestas del tanque)
socket.onmessage = function(event) {
    if(event.data.indexOf("NOTIFY:") === 0) {                  //PARA GESTIONAR NOTIFICACIONES DE ESTADOS
        const data = JSON.parse(event.data.substring(7));
        console.log("BROADCAST recibido del servidor:", data);
        if(data === "motors:status:enabled") {
            switchElement.checked = true;                       //esta parte no funciona bien
        } else if (data === "motors:status:disabled") {
            switchElement.checked = false;
        }
    } else {
        const data = JSON.parse(event.data);
        console.log("Mensaje recibido del servidor:", data);
    }
};

// Manejar errores de conexión
socket.onerror = function(error) {
    console.error("Error de WebSocket:", error);
};

// Manejar cierre de conexión
socket.onclose = function() {
    console.log("Conexión cerrada");
};