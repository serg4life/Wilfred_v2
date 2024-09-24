#ifndef WEB_CONTROLLER_H
#define WEB_CONTROLLER_H

#include <WiFi.h>
#include <WiFiClient.h>
#include <WiFiAP.h>

class WebController {
    public:
        WebController(WiFiServer server_par);
        void initWebController(void);
        void listenForClients(void);

    private:
        WiFiServer server;
        String header;

};

#endif