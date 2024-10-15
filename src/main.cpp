/*Wilfred Web Controller at 192.168.4.1*/

#include <Core.h>
#include <WilfredServer.h>
#include <CommandHandler.h>
#include <PID.h>

PIDController pid = PIDController(10.0, 0.1, 0.025, 0.9);
Core Wilfred = Core();
WebController controller = WebController();
CommandHandler handler = CommandHandler();
float power = 50;

// Declaración del timer de hardware
hw_timer_t * timer = NULL;
portMUX_TYPE timerMux = portMUX_INITIALIZER_UNLOCKED;  // Mutex para proteger los recursos compartidos

volatile bool computePID = false;  // Bandera para saber cuándo ejecutar el PID
unsigned long lastComputeTime = 0; // Para almacenar el tiempo del último cálculo

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
    handler.enqueueCommand(command);  //Lo metemos en una cola y es procesado por otra tarea en otro core.
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
  //if(!Serial){Serial.end();}
  handler.assignCore(Wilfred);
  //Serial.print("setup() running on core ");
  //Serial.println(xPortGetCoreID());
  Wilfred.initIMU();
  Wilfred.wakeIMU();
  Wilfred.calibrateIMU();
  // Configurar el timer de hardware
  timer = timerBegin(0, 80, true);  // Timer 0, preescalador 80 (1 tick = 1 µs), contar hacia arriba
  timerAttachInterrupt(timer, &onTimer, true);  // Adjuntar la función de interrupción
  timerAlarmWrite(timer, 1000, true);  // Disparar la interrupción cada 100 ms (100,000 µs)
  timerAlarmEnable(timer);  // Habilitar la alarma del timer
  pid.setReference(Wilfred.getHeading());
  pid.setCheckAngle(true);
  Wilfred.enableMotors();
  //Wilfred.move(FORWARD, power);
  //xTaskCreatePinnedToCore(serverTask, "ServerTask", 8192, NULL, 1, NULL, 1);
}

void loop() {
  double headingData = Wilfred.getHeading();
  pid.updateInput(headingData);

  if (computePID) {
    portENTER_CRITICAL(&timerMux);
    computePID = false;
    portEXIT_CRITICAL(&timerMux);
    // Calcular tiempo transcurrido desde la última ejecución del PID
    unsigned long now = micros();
    double deltaTime = (now - lastComputeTime) / 1000000.0;  // Convertir a segundos
    lastComputeTime = now;
    pid.compute(deltaTime);
    double u = pid.getOutput();
    double powerR = power - u/2;
    double powerL = power + u/2;

    // if(pid.getLastError() < 0){
    //   Wilfred.motor_R.setRotation(COUNTERCLOCKWISE);
    //   Wilfred.motor_L.setRotation(CLOCKWISE);
    // }

    // //CAMBIAR SENTIDO RAPIDO IGNORANDO EL CONTROL
    // if(powerR == 0){
    //   Wilfred.motor_R.invertRotation();
    //   powerR = 100;
    // }
    // if(powerL == 0){
    //   Wilfred.motor_L.invertRotation();
    //   powerL = 100;
    // }

    Wilfred.motor_L.setPower(powerL);
    Wilfred.motor_R.setPower(powerR);
    
    Serial.print("angle: ");
    Serial.print(headingData);
    Serial.print(" | u: ");
    Serial.print(u);
    Serial.print(" | powerR: ");
    Serial.print(powerR);
    Serial.print(" | powerL: ");
    Serial.println(powerL);
  }
}