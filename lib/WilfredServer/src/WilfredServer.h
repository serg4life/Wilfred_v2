#ifndef WEB_CONTROLLER_H
#define WEB_CONTROLLER_H

#include <WiFi.h>
#include <ESPmDNS.h>
#include <AsyncTCP.h>
#include <ArduinoJson.h>
#include <ESPAsyncWebServer.h>
#include <WebSocketsServer.h>
#include <SPIFFS.h>

#define WIFI_TIMEOUT_MS 5000

class WebController {
    public:
        WebController(void);
        void initWebController(void);
        void setOnEvent(WebSocketsServer::WebSocketServerEvent cbEvent);
        void serviceLoop(void);
        bool loadStatic(void);
        void provideFiles(void);
        WebSocketsServer webSocket;

};

#endif