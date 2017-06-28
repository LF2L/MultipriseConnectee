/*
    This sketch demonstrates how to set up a simple HTTP-like server.
    The server will set a GPIO pin depending on the request
      http://server_ip/gpio/0 will set the GPIO2 low,
      http://server_ip/gpio/1 will set the GPIO2 high
    server_ip is the IP address of the ESP8266 module, will be
    printed to Serial when the module is connected.
*/

#include <ESP8266WiFi.h>
#include <Wire.h>
#include "ds3231.h"
#include <ArduinoJson.h>

struct IoTobject{
  IPAddress ip;
  char* id;
  char* device_type;
  
  void printconsole(){
     Serial.println("info into IoTObject ");
     Serial.print(" IP: ");
     Serial.println(ip);
     Serial.print(" id: ");
     Serial.println(id);
     Serial.print(" device: ");
     Serial.println(device_type);
  }
};
IoTobject monit;

const unsigned long BAUD_RATE = 9600;
const int BUFF_MAX=128;

const char* ssid = "IoE Server";
const char* password = "Jaedeo13ecmden13";

// Create an instance of the server
// specify the port to listen on as an argument
WiFiServer server(80);

boolean plg[6] ;
int plgPin[] ={9,10,14,12,13,15};

uint8_t timet[8];

void setup() {
  initSerial();
  Wire.begin();
  DS3231_init(DS3231_INTCN);
  StaticJsonBuffer<200> jsonBuffer;
  JsonObject& root = jsonBuffer.createObject();
  delay(100);


  // prepare GPIO
  pinMode(plgPin[1], OUTPUT);
  pinMode(plgPin[2], OUTPUT);
  pinMode(plgPin[3], OUTPUT);
  pinMode(plgPin[4], OUTPUT);
  pinMode(plgPin[5], OUTPUT);
  pinMode(plgPin[6], OUTPUT);
  digitalWrite(plgPin[1], 0);
  digitalWrite(plgPin[2], 0);
  digitalWrite(plgPin[3], 0);
  digitalWrite(plgPin[4], 0);
  digitalWrite(plgPin[5], 0);
  digitalWrite(plgPin[6], 0);

  // Connect to WiFi network
  Serial.println();
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);

  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi connected");

  // Start the server
  server.begin();
  Serial.println("Server started");

  // Print the IP address
  Serial.println(WiFi.localIP());


}

void loop() {
  PowerStrip ps;
  //for time management 
  char buff[BUFF_MAX];
  struct ts t;
  
  // Check if a client has connected
  WiFiClient client = server.available();
  if (!client) {
    return;
  }
   DS3231_get(&t);
   snprintf(buff, BUFF_MAX, "%d.%02d.%02d %02d:%02d:%02d %ld", t.year,
             t.mon, t.mday, t.hour, t.min, t.sec);
  // Wait until the client sends some data
  Serial.println(buff);
  Serial.println("- new client");
  while (!client.available()) {
    delay(1);
  }

  // Read the first line of the request
  String req = client.readStringUntil('\r');
  Serial.println(req);
  client.flush();

  // Match the request

   if (req.indexOf("/plug/1") != -1)
    plg[1] = (plg[1] == 0) ? 1 : 0;
  else if (req.indexOf("/plug/2") != -1)
    plg[2] = (plg[2] == 0) ? 1 : 0;
  else if (req.indexOf("/plug/3") != -1)
    plg[3] = (plg[3] == 0) ? 1 : 0;
  else if (req.indexOf("/plug/4") != -1)
    plg[4] = (plg[4] == 0) ? 1 : 0;
  else if (req.indexOf("/plug/5") != -1)
    plg[5] = (plg[5] == 0) ? 1 : 0;
  else if (req.indexOf("/plug/6") != -1)
    plg[6] = (plg[6] == 0) ? 1 : 0;
  else {
    Serial.println("invalid request");
    client.stop();
    return;
  }
 
  StaticJsonBuffer<500> jsonBuffer;
  JsonObject& json = prepareResponse(jsonBuffer);
  writeResponse(client, json);
  
  digitalWrite(plgPin[1], plg[1]);
  digitalWrite(plgPin[2], plg[2]);
  digitalWrite(plgPin[3], plg[3]);
  digitalWrite(plgPin[4], plg[4]);
  digitalWrite(plgPin[5], plg[5]);
  digitalWrite(plgPin[6], plg[6]);
  
  client.flush();

  delay(1);
  Serial.println("Client disonnected");

  // The client will actually be disconnected
  // when the function returns and 'client' object is detroyed
}
void initSerial() {
  Serial.begin(BAUD_RATE);
  while (!Serial) {
    ;  // wait for serial port to initialize
  }
  Serial.println("Serial ready");
}

JsonObject& prepareResponse(JsonBuffer& jsonBuffer) {
  char buff[BUFF_MAX];
  struct ts t;
   DS3231_get(&t);
   snprintf(buff, BUFF_MAX, "%d.%02d.%02d %02d:%02d:%02d %ld", t.year,
             t.mon, t.mday, t.hour, t.min, t.sec);
             
  JsonObject& root = jsonBuffer.createObject();
  root["device_type"] = "PowerStrip";
  root["ip_address"] = WiFi.localIP();
  root["id"] = 00000;
  root["nb_socket"] = ps.NbSocket;
  root["time"] = t.hour+":"+t.min;

  JsonArray& socket = root.createNestedArray("socket");
  for (int soc = 0; soc < ps.NbSocket; soc++) {
    socket.add("state",plg[soc]);
  }

  return root;
}
void writeResponse(WiFiClient& client, JsonObject& json) {
  client.println("HTTP/1.1 200 OK");
  client.println("Content-Type: application/json");
  client.println("Connection: close");
  client.println();

  json.prettyPrintTo(client);
}

bool sendIotInfo(){
  http.begin("http://192.168.42.1:5000/objectport/");
  http.addHeader("Content-Type", "application/json");
  String s= "{\"ip_adress\": ";
  s+=monit.ip;
  s+=", \"id\": ";
  s+=monit.id;
  s+=",\"device_type\": ";
  s+=monit.device_type;
  s+="}";
  int postCode = http.POST(s);
  // int postCode = http.POST("{\"ip_adress\": \"192.168.42.17\", \"id\": \"00001\",\"device_type\": \"monitor\"}");
  http.writeToStream(&Serial);
  if (postCode == HTTP_CODE_OK){
    return true; 
  }else{
    return false;
  }
}
