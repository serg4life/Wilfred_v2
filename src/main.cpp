/*Wilfred Web Controller at 192.168.4.1*/

#include <Core.h>
#include <Controller.h>
#include <PID.h>

Core Wilfred = Core();
WebController controller = WebController(Wilfred);

// Declaración del timer de hardware
hw_timer_t * timer = NULL;
portMUX_TYPE timerMux = portMUX_INITIALIZER_UNLOCKED;  // Mutex para proteger los recursos compartidos

volatile bool computePID = false;  // Bandera para saber cuándo ejecutar el PID

// Función de interrupción del timer
void IRAM_ATTR onTimer() {
  portENTER_CRITICAL_ISR(&timerMux);
  computePID = true;  // Bandera para ejecutar PID en el loop
  portEXIT_CRITICAL_ISR(&timerMux);
}

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
  while(!Serial){}; //PARA DEBUG
  Wilfred.initIMU();

  // Configurar el timer de hardware
  timer = timerBegin(0, 80, true);  // Timer 0, preescalador 80 (1 tick = 1 µs), contar hacia arriba
  timerAttachInterrupt(timer, &onTimer, true);  // Adjuntar la función de interrupción
  timerAlarmWrite(timer, 100000, true);  // Disparar la interrupción cada 100 ms (100,000 µs)
  timerAlarmEnable(timer);  // Habilitar la alarma del timer

  xTaskCreatePinnedToCore(
    serverTask,
    "ServerTask",
    8192,
    NULL,
    1,
    NULL,
    1
  );
}

void loop() {
  if (computePID) {
    portENTER_CRITICAL(&timerMux);
    computePID = false;
    portEXIT_CRITICAL(&timerMux);
    //HACER COSAS
  }
}