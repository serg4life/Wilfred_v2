/*Wilfred Web Controller at 192.168.4.1*/

#include <Core.h>
#include <Controller.h>

Core Wilfred = Core();
WebController controller = WebController(Wilfred);

void webSocketEvent(uint8_t client_num, WStype_t type, uint8_t * payload, size_t length) {
  if (type == WStype_TEXT) {
    String message = String((char*) payload);
    Serial.println("Mensaje recibido: " + message);

    // Parsear el JSON para obtener el comando
    JsonDocument doc;
    DeserializationError error = deserializeJson(doc, message);
    if (error) {
      Serial.println("Error al parsear JSON");
      return;
    }

    String command = doc["command"];
    controller.commandHandler(command); // Controlar el tanque según el comando recibido
  }
}

void setup() {
  pinMode(LED_BUILTIN, OUTPUT);
  Wilfred.initIMU();
  Wilfred.enableMotors();
  controller.initWebController();
  controller.setOnEvent(webSocketEvent);
  Serial.begin(115200); 
}

void loop() {
  controller.serviceLoop();
}