

#include <WiFi.h>    //  WiFi biblioteket 

const char* ssid     = "Min-ESP32";   // Her skal stå Min-ESP32 som det, navn vi vil give neværket 
const char* password = "87654321";  // Her skal stå 87654321, som adgangskoden til netværket


WiFiServer server(80); // Sæt webserverens portnummer til 80


String header; // Variabel til at gemme HTTP-anmodningen

// Hjælpevariable til lagring af den aktuelle outputtilstand. De to lysdioder skal være slukket fra starten
String led_OneState = "off";
String led_TwoState = "off";

// Tildel outputvariabler til GPIO-pin
const int output18 = 18;
const int output22 = 22;

void setup() {
  Serial.begin(115200);
  //  Initialiser outputvariablerne som output
  pinMode(output18, OUTPUT);
  pinMode(output22, OUTPUT);
  //Indstil output til LOW
  digitalWrite(output18, LOW);
  digitalWrite(output22, LOW);
  

  // Opret forbindelse til Wi-Fi-netværk med SSID og adgangskode
  Serial.print("Setting AP");
  WiFi.softAP (ssid, password);
  IPAddress IP = WiFi. softAPIP();
  Serial.print ("AP IP adress: ");
  Serial.println(IP); 
  server.begin();                         


}

void loop(){
  WiFiClient client = server.available();   

  if (client) {                             
    Serial.println("New Client.");         
    String currentLine = "";                
    while (client.connected()) {            
      if (client.available()) {          
        char c = client.read();             
        Serial.write(c);                    
        header += c;
        if (c == '\n') {                    // hvis byten er et nylinjetegn
          //hvis den aktuelle linje er tom, har du to nye linjetegn i træk
          // det er slutningen af klientens HTTP-anmodning, så send et svar
          if (currentLine.length() == 0) {
            // HTTP-headere starter altid med en svarkode (f.eks. HTTP/1.1 200 OK)
            // og en indholdstype, så klienten ved, hvad der kommer, derefter en tom linje:
            client.println("HTTP/1.1 200 OK");
            client.println("Content-type:text/html");
            client.println("Connection: close");
            client.println();
            
          // tænder og slukker for GPIO'erne
           if (header.indexOf("GET /18/on") >= 0) {
                Serial.println("GPIO 18 on");
                led_OneState = "on";
                digitalWrite(output18, HIGH);
              } else if (header.indexOf("GET /18/off") >= 0) {
                Serial.println("GPIO 18 off");
                led_OneState = "off";
                digitalWrite(output18, LOW);
              } else if (header.indexOf("GET /22/on") >= 0) {
                Serial.println("GPIO 22 on");
                led_TwoState = "on";
                digitalWrite(output22, HIGH);
              } else if (header.indexOf("GET /22/off") >= 0) {
                Serial.println("GPIO 22 off");
                led_TwoState = "off";
                digitalWrite(output22, LOW);        
            }
            
            // Viser HTML-websiden
            client.println("<!DOCTYPE html><html>");
            client.println("<head><meta name=\"viewport\" content=\"width=device-width, initial-scale=1\">");
            client.println("<link rel=\"icon\" href=\"data:,\">");
            // CSS to style the on/off buttons 
            // Feel free to change the background-color and font-size attributes to fit your preferences
            client.println("<style>html { font-family: Helvetica; display: inline-block; margin: 2px auto; text-align: center;}");
            client.println(".button { background-color: #f8fc05; border: none; color: white; padding: 16px 40px;");
            client.println("text-decoration: none; font-size: 30px; margin: 2px; cursor: pointer;}");
            client.println(".button2 {background-color: #0954eb;}</style></head>");
            
            // Webside overskrift
            
            client.println("<body><h1>ESP32 Webserver AP</h1>");
           
            // Vis den aktuelle tilstand og Tænd/Sluk-knapper for GPIO 18
          
            // Hvis Led_OneState er slukket, viser den knappen Tænd       
            if (led_OneState=="off") {
              client.println("<p><a href=\"/18/on\"><button class=\"button\">T&aelig;nd</button></a>");
            } else {
              client.println("<a href=\"/18/off\"><button class=\"button button2\">Sluk</button></a>");
            } 
               
          
         
            // Hvis Led_TwoState er slukket, viser den knappen Tænd        
            if (led_TwoState=="off") {
              client.println("<a href=\"/22/on\"><button class=\"button\">T&aelig;nd</button></a>");
            } else {
              client.println("<a href=\"/22/off\"><button class=\"button button2\">Sluk</button></a></p>");
            }
            client.println("</body></html>");
            
             client.println("<body><h3>GPIO 18     &nbsp &nbsp &nbsp &nbsp &nbsp &nbsp &nbsp &nbsp     GPIO 22  </h3>");
            // HTTP-svaret slutter med endnu en tom linje
            client.println();
            // Bryd ud af while-løkken (loop)
            break;
          } else { // hvis du har en ny linje, så ryd den aktuelle linje
            currentLine = "";
          }
        } else if (c != '\r') {  // // hvis du har noget andet end et vognretur-karakter,
          currentLine += c;      // tilføje det til slutningen af den aktuelle linje
                                 //En vognretur betyder at flytte markøren til begyndelsen af linjen
        }
      }
    }
    // Ryd overskriftsvariablen
    header = "";
    // Luk forbindelsen
    client.stop();
    Serial.println("Client disconnected.");
    Serial.println("");
  }
}
