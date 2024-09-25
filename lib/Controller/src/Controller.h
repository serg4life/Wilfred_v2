#ifndef WEB_CONTROLLER_H
#define WEB_CONTROLLER_H

#include <WiFi.h>
#include <WiFiClient.h>
#include <WiFiAP.h>
#include <Core.h>

typedef enum {FORWARD_CMD, BACKWARD_CMD, STOP_CMD, ENABLE_MOTORS_CMD} Commands;

class WebController {
    public:
        WebController(Core param_core);
        void initWebController(void);
        void listenForClients(void);
        void startService(void);
        void headerParse(void);

    private:
        String header;
        Core coreObject;
};

#endif