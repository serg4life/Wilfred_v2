#include <Controller.h>
#include <Core.h>

const char *ssid = "Wilfred";
const char *password = "12345678";
String MOTORS_STATE = "DISABLED";
WiFiServer server(80);

//HACE FALTA ALGUNA FORMA DE PASAR VARIABLES Y DE INTERPRETAR LAS ACCIONES
WebController::WebController(Core param_core){
    coreObject = param_core;
};

void WebController::initWebController(void){
    if (!WiFi.softAP(ssid, password)) {
        log_e("Soft AP creation failed.");
    while(1);
  }
  IPAddress myIP = WiFi.softAPIP();
  Serial.print("AP IP address: ");
  Serial.println(myIP);
  server.begin();
  Serial.println("Server started");
};

void WebController::startService(void){
    while(true){
        listenForClients();
        //headerParse();          //Important, must be called after exactly listenForClients
    };
};

void WebController::headerParse(void){
    if(header.indexOf("GET /LED/ON") >= 0){
        Serial.println("LED ON");
        MOTORS_STATE = "ENABLED";
        digitalWrite(LED_BUILTIN, HIGH);
        coreObject.enableMotors();
    } else if(header.indexOf("GET /LED/OFF") >= 0){
        Serial.println("LED OFF");
        MOTORS_STATE = "DISABLED";
        digitalWrite(LED_BUILTIN, LOW);
        coreObject.disableMotors();
    } else if(header.indexOf("GET /FORWARD") >= 0){
        Serial.println("FORWARD");
        coreObject.move(FORWARD, 70);
    } else if(header.indexOf("GET /BACKWARD") >= 0){
        Serial.println("BACKWARDS");
        coreObject.move(BACKWARD, 70);
    } else if(header.indexOf("GET /STOP") >= 0){
        Serial.println("STOP");
        coreObject.stop();
    } else if(header.indexOf("GET /ROTR") >= 0){
        Serial.println("ROTR");
        coreObject.rotate(CLOCKWISE, 70);
    } else if(header.indexOf("GET /ROTL") >= 0){
        Serial.println("ROTI");
        coreObject.rotate(COUNTERCLOCKWISE, 70);
    };
    //header = "";
};

void WebController::listenForClients(){
    //header = "";
    WiFiClient client = server.available();
    if (client) {                             // if you get a client,
        Serial.println("New Client.");           // print a message out the serial port
        String currentLine = "";                // make a String to hold incoming data from the client
        while (client.connected()) {            // loop while the client's connected
            if (client.available()) {             // if there's bytes to read from the client,
                char c = client.read();             // read a byte, then
                Serial.write(c);
                header += c;                    // print it out the serial monitor
                if (c == '\n') {                    // if the byte is a newline character
                    // if the current line is blank, you got two newline characters in a row.
                    // that's the end of the client HTTP request, so send a response:
                    if (currentLine.length() == 0) {
                        headerParse();
                        // HTTP headers always start with a response code (e.g. HTTP/1.1 200 OK)
                        // and a content-type so the client knows what's coming, then a blank line:
                        client.println("HTTP/1.1 200 OK");
                        client.println("Content-type:text/html");
                        client.println();

                        // Display the HTML web page
                        client.println("<!DOCTYPE html><html>");
                        client.println("<head><meta name=\"viewport\" content=\"width=device-width, initial-scale=1\">");
                        client.println("<link rel=\"icon\" href=\"data:,\">");
                        // CSS to style the on/off buttons 
                        // Feel free to change the background-color and font-size attributes to fit your preferences
                        client.println("<style>html { font-family: Helvetica; display: inline-block; margin: 0px auto; text-align: center;}");
                        client.println(".button { background-color: #4CAF50; border: none; color: white; padding: 16px 40px;");
                        client.println("text-decoration: none; font-size: 30px; margin: 2px; cursor: pointer;}");
                        client.println(".container {width: 300px; display: flex; justify-content: center; margin: auto;}");
                        client.println(".button2 {background-color: #555555;}</style><title>Wilfred Controller</title></head>");
                        
                        // Web Page Heading
                        client.println("<body><h1>Wilfred Controller</h1>");

                        client.println("<p>MOTORS - State " + MOTORS_STATE + "</p>");
                        // If the output27State is off, it displays the ON button       
                        if (MOTORS_STATE=="DISABLED") {
                            client.println("<p><a href=\"/LED/ON\"><button class=\"button\">ENABLE MOTORS</button></a></p>");
                        } else {
                            client.println("<p><a href=\"/LED/OFF\"><button class=\"button button2\">DISABLE MOTORS</button></a></p>");
                        }

                        client.println("<div class=\"container\"><p><a href=\"/FORWARD\"><button class=\"button\">FORWARD</button></a></p></div>");
                        client.println("<div class=\"container\"><p><a href=\"/ROTL\"><button class=\"button\">LEFT</button></a></p>");
                        client.println("<p><a href=\"/STOP\"><button class=\"button\">STOP</button></a></p>");
                        client.println("<p><a href=\"/ROTR\"><button class=\"button\">RIGHT</button></a></p></div>");
                        client.println("<div class=\"container\"><p><a href=\"/BACKWARD\"><button class=\"button\">BACKWARD</button></a></p></div>");
                        client.println("</body></html>");
                        
                        // The HTTP response ends with another blank line
                        client.println();
                        // break out of the while loop:
                        break;
                    } else {    // if you got a newline, then clear currentLine:
                        currentLine = "";
                    }
                } else if (c != '\r') {  // if you got anything else but a carriage return character,
                    currentLine += c;      // add it to the end of the currentLine
                }
            }
        }
        // Clear the header variable
        header = "";
        // close the connection:
        client.stop();
        Serial.println("Client Disconnected.");
    }
};
