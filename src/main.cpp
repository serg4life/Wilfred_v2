/*Wilfred Web Controller at 192.168.4.1*/

#include <Core.h>
#include <WiFi.h>
#include <WiFiClient.h>
#include <WiFiAP.h>

#define LED_BUILTIN 13   // Set the GPIO pin where you connected your test LED or comment this line out if your dev board has a built-in LED

// Set these to your desired credentials.

Core Wilfred = Core();

void setup() {
  pinMode(LED_BUILTIN, OUTPUT);
  Wilfred.initIMU();
  Wilfred.initController();
  Serial.begin(115200);
}

void loop() {
  Wilfred.initControllerService();
}