/*Wilfred Web Controller at 192.168.4.1*/

#include <Core.h>
#include <WilfredServer.h>
#include <CommandHandler.h>

Core Wilfred = Core();
WebController controller = WebController();
CommandHandler handler = CommandHandler();

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
    handler.processCommand(command); // Controlar el tanque según el comando recibido
  }
}

void serverTask(void *pvParameters){
  // Iniciar SPIFFS, montar FS
  if (!SPIFFS.begin(true)) {
    Serial.println("Error al montar SPIFFS");
    vTaskDelete(NULL);
    return;
  }

  controller.initWebController();
  controller.setOnEvent(webSocketEvent);

  while(true) {
    controller.serviceLoop();
  }
};

void setup() {
  Serial.begin(115200);
  delay(1000);
  if(!Serial){Serial.end();}
  handler.assignCore(Wilfred);
  //Serial.print("setup() running on core ");
  //Serial.println(xPortGetCoreID());
  Wilfred.initIMU();
  xTaskCreatePinnedToCore(serverTask, "ServerTask", 8192, NULL, 1, NULL, 1);
}

void loop() {
  //Serial.print("loop() running on core ");
  //Serial.println(xPortGetCoreID());
}