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
    File file = SPIFFS.open("/index.html");
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
    Serial.println("Server started");
};

void WebController::setOnEvent(WebSocketsServer::WebSocketServerEvent cbEvent){
    webSocket.onEvent(cbEvent);
};

void WebController::serviceLoop(){
    webSocket.loop();
};

void WebController::commandHandler(String command){
    if (command == "forward") {
        coreObject.move(FORWARD, 70);
    } else if (command == "stop") {
        coreObject.stop();
    }
};

