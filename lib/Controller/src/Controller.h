#ifndef WEB_CONTROLLER_H
#define WEB_CONTROLLER_H

#include <WiFi.h>
#include <WiFiClient.h>
#include <WiFiAP.h>

typedef enum {FORWARD_CMD, BACKWARD_CMD, STOP_CMD, ENABLE_MOTORS_CMD} Commands;

class WebController {
    public:
        WebController(WiFiServer server_par);
        void initWebController(void);
        void listenForClients(void);
        void startService(void);
        void headerParse(void);

    private:
        WiFiServer server;
        String header;

};

#endif