#include <Controller.h>

const char *ssid = "Wilfred";
const char *password = "12345678";

WebController::WebController(WiFiServer server_par){
    server = server_par;
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

void WebController::listenForClients(){
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
                        // HTTP headers always start with a response code (e.g. HTTP/1.1 200 OK)
                        // and a content-type so the client knows what's coming, then a blank line:
                        client.println("HTTP/1.1 200 OK");
                        client.println("Content-type:text/html");
                        client.println();

                        if(header.indexOf("GET /LED/ON") >= 0){
                            Serial.println("LED ON");
                            LED_STATE = "ON";
                            digitalWrite(LED_BUILTIN, HIGH);
                        } else if(header.indexOf("GET /LED/OFF") >= 0){
                            Serial.println("LED OFF");
                            LED_STATE = "OFF";
                            digitalWrite(LED_BUILTIN, LOW);
                        } else if(header.indexOf("GET /FORWARD") >= 0) {
                            Wilfred.move(FORWARD, 70);
                        } else if(header.indexOf("GET /BACKWARD") >= 0) {
                            Wilfred.move(BACKWARD, 70);
                        } else if(header.indexOf("GET /STOP") >= 0) {
                            Wilfred.stop();
                        }
                        // Display the HTML web page
                        client.println("<!DOCTYPE html><html>");
                        client.println("<head><meta name=\"viewport\" content=\"width=device-width, initial-scale=1\">");
                        client.println("<link rel=\"icon\" href=\"data:,\">");
                        // CSS to style the on/off buttons 
                        // Feel free to change the background-color and font-size attributes to fit your preferences
                        client.println("<style>html { font-family: Helvetica; display: inline-block; margin: 0px auto; text-align: center;}");
                        client.println(".button { background-color: #4CAF50; border: none; color: white; padding: 16px 40px;");
                        client.println("text-decoration: none; font-size: 30px; margin: 2px; cursor: pointer;}");
                        client.println(".button2 {background-color: #555555;}</style><title>Wilfred Controller</title></head>");
                        
                        // Web Page Heading
                        client.println("<body><h1>Wilfred Controller</h1>");

                        // Display current state, and ON/OFF buttons for GPIO 27  
                        client.println("<p>LED - State " + LED_STATE + "</p>");
                        // If the output27State is off, it displays the ON button       
                        if (LED_STATE=="OFF") {
                            client.println("<p><a href=\"/LED/ON\"><button class=\"button\">ON</button></a></p>");
                        } else {
                            client.println("<p><a href=\"/LED/OFF\"><button class=\"button button2\">OFF</button></a></p>");
                        }
                        client.println("<p><a href=\"/FORWARD\"><button class=\"button\">FORWARD</button></a></p>");
                        client.println("<p><a href=\"/BACKWARD\"><button class=\"button\">BACKWARD</button></a></p>");
                        client.println("<p><a href=\"/STOP\"><button class=\"button\">STOP</button></a></p>");
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
