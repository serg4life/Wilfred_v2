/*Wilfred Web Controller at 192.168.4.1*/

#include <Core.h>
#include <Controller.h>

Core Wilfred = Core();
WebController controller = WebController(Wilfred);

void setup() {
  pinMode(LED_BUILTIN, OUTPUT);
  Wilfred.initIMU();
  controller.initWebController();
  Serial.begin(115200);
}

void loop() {
  controller.startService();
}