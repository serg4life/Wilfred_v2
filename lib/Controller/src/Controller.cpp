#include <Controller.h>
#include <Core.h>

const char *ssid = "Wilfred";
const char *password = "12345678";

String MOTORS_STATE = "DISABLED";
String htmlContent;

AsyncWebServer server(80);

//HACE FALTA ALGUNA FORMA DE PASAR VARIABLES Y DE INTERPRETAR LAS ACCIONES
WebController::WebController(Core param_core) : webSocket(8080) {
    coreObject = param_core;
};

bool WebController::loadStatic(){
    File file = SPIFFS.open("/index.html","r");
    if (!file) {
        Serial.println("Error al abrir el archivo HTML");
        return false;
    }
    htmlContent = file.readString();
    file.close();
    return true;
};

void WebController::initWebController(void){
    if (!WiFi.softAP(ssid, password)) {
        log_e("Soft AP creation failed.");
    while(1);
    }
    IPAddress myIP = WiFi.softAPIP();
    Serial.print("AP IP address: ");
    Serial.println(myIP);

    webSocket.begin();
    server.begin();
    loadStatic();
    // Servir la página web cuando se acceda a la raíz del servidor
    server.on("/", HTTP_GET, [](AsyncWebServerRequest *request){
        request->send(200, "text/html", htmlContent);    
    });
    // Sirve el archivo styles.css cuando el navegador lo solicita
    server.on("/css/styles.css", HTTP_GET, [](AsyncWebServerRequest *request) {
        request->send(SPIFFS, "/css/styles.css", "text/css");
    });

    // Sirve el archivo script.js cuando el navegador lo solicita
    server.on("/js/script.js", HTTP_GET, [](AsyncWebServerRequest *request) {
        request->send(SPIFFS, "/js/script.js", "application/javascript");
    });
    // Sirve el archivo favicon.ico
    server.on("/ico/favicon.ico", HTTP_GET, [](AsyncWebServerRequest *request) {
        request->send(SPIFFS, "/ico/favicon.ico", "image/x-icon");
    });
    Serial.println("Server started");
};

void WebController::setOnEvent(WebSocketsServer::WebSocketServerEvent cbEvent){
    webSocket.onEvent(cbEvent);
};

void WebController::serviceLoop(){
    webSocket.loop();
};

void WebController::commandHandler(String command){
    if (command.startsWith("core:")) {
        command.substring(5);
        if (command == "forward") {
            coreObject.move(FORWARD, 70);
        } else if (command == "backward") {
            coreObject.move(BACKWARD, 70);
        } else if (command == "right") {
            coreObject.rotate(CLOCKWISE, 60);
        } else if (command == "left") {
            coreObject.rotate(COUNTERCLOCKWISE, 60);
        }
    } else if (command.startsWith("motors:")) {
        command.substring(7);
        if (command == "enable") {
            coreObject.enableMotors();
            //webSocket.broadcastTXT("NOTIFY:motors:status:enabled");    //Para notificar a todos los clientes el estado de los motores
        } else if (command == "disable") {
            //webSocket.broadcastTXT("NOTIFY:motors:status:disabled");
            coreObject.disableMotors();
        } else if (command == "stop") {
            coreObject.stop();
        }
    } else if (command.startsWith("imu:")) {
        command.substring(4);
        if (command == "calibrate") {
            coreObject.calibrateIMU();
        }
    }
};

