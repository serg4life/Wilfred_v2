#include <CommandHandler.h>
#include <WilfredServer.h>

extern WebController controller;

CommandHandler::CommandHandler(){
    commandQueue = xQueueCreate(10, sizeof(String));
    xTaskCreatePinnedToCore(
        [](void *pvParameters) { static_cast<CommandHandler*>(pvParameters)->processCommandTask(pvParameters);},
        "CommandProcessingTask",
        2048,
        this,
        1,
        NULL,
        0
    );
};


void CommandHandler::assignCore(Core coreObject_){
    coreObject = coreObject_;
};


void CommandHandler::enqueueCommand(String& command){
    if (xQueueSend(commandQueue, &command, portMAX_DELAY) != pdTRUE) {
        Serial.println("Error: no se pudo agregar el comando a la cola");
    }
};

void CommandHandler::processCommandTask(void *pvParamaters){
    String command;
    while(true){
        if (xQueueReceive(commandQueue, &command, portMAX_DELAY) == pdTRUE) {
            // Procesar el comando
            Serial.println("Procesando comando: " + command);
            // Aquí va el código para ejecutar el comando
            if (command.startsWith("core:")) {
                command = command.substring(5);
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
                command = command.substring(7);
                if (command == "enable") {
                    coreObject.enableMotors();
                    controller.webSocket.broadcastTXT("NOTIFY:motors:status:enabled");    //Para notificar a todos los clientes el estado de los motores
                } else if (command == "disable") {
                    controller.webSocket.broadcastTXT("NOTIFY:motors:status:disabled");
                    coreObject.disableMotors();
                } else if (command == "stop") {
                    coreObject.stop();
                }
            } else if (command.startsWith("imu:")) {
                command = command.substring(4);
                if (command == "calibrate") {   
                    coreObject.calibrateIMU();  /*NO FUNCIONA cuando se ejecuta desde aqui?*/
                } else if (command == "activate") {
                    coreObject.wakeIMU();
                } else if (command == "deactivate") {
                    coreObject.sleepIMU();
                }
            }
        }
    }
};