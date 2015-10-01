/*********************************************************************************
POLLUTION MAPPER
*********************************************************************************/
#include <ESP8266WiFi.h>

#define SENSOR_LED                    14
#define CO_CONTENT_INPUT              A0
#define HEATER_5V                     60000
#define HEATER_1_4_V                  90000
#define READING_DELAY_MILLIS          5000

const char* g_ssid      = "ssid";
const char* g_password  = "password";
const char* g_host      = "184.106.153.149";
const char* g_apiKey   = "thingspeak-key";

unsigned long g_startMillis;
unsigned long g_switchTimeMillis;
boolean       g_heaterInHighPhase;

/*********************************************************************************
Function Name     : setup
Description       : Initialize the Serial Communication with baud 115200, connect 
                    to the Router with SSID and PASSWORD and turn on the heater 
                    on the CO sensor.
Parameters        : void
Return            : void
*********************************************************************************/

void setup(void){
  Serial.begin(115200);
  Serial.println();
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(g_ssid);
  
  WiFi.begin(g_ssid, g_password);
  
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("");
  Serial.println("WiFi connected");  
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
  
  pinMode(SENSOR_LED, OUTPUT);
  
  g_startMillis = millis();
  
  switchHeaterHigh();
  
  Serial.println("Elapsed Time (s), Gas Level");
}

/*********************************************************************************
Function Name     : loop
Description       : Switch between the Heating Cycle from 5v to 1.4v and 
                    read the Gas Level
Parameters        : void
Return            : void
*********************************************************************************/
void loop(void){

  if(g_heaterInHighPhase){
    // 5v phase of cycle. see if need to switch low yet
    if(millis() > g_switchTimeMillis) {
      switchHeaterLow();
    }
  }
  else {
    // 1.4v phase of cycle. see if need to switch high yet
    if(millis() > g_switchTimeMillis) {
      switchHeaterHigh();
    }
  }
  
  presentGasLevel();
  delay(READING_DELAY_MILLIS);
}

/*********************************************************************************
Function Name         : turnHeaterHigh
Description           : Trun on the Heater by Providing 5V to the CO Sensor
Parameters            : void
Return                : void
*********************************************************************************/
void switchHeaterHigh(void){
  // 5v phase
  digitalWrite(SENSOR_LED, LOW);
  g_heaterInHighPhase = true;
  g_switchTimeMillis = millis() + HEATER_5V;
}

/*********************************************************************************
Function Name         : turnHeaterLow
Description           : Trun off the Heater by Providing 1.4V to the CO Sensor
Parameters            : void
Return                : void
*********************************************************************************/
void switchHeaterLow(void){
  // 1.4v phase
  digitalWrite(SENSOR_LED, HIGH);
  g_heaterInHighPhase = false;
  g_switchTimeMillis = millis() + HEATER_1_4_V;
}


/*********************************************************************************
Function Name         : presentGasLevel
Description           : read the present CO content in the Air as a analog value
Parameters            : void
Return                : void
*********************************************************************************/
void presentGasLevel(){
  unsigned int l_gasLevel = analogRead(CO_CONTENT_INPUT);
  unsigned int time = (millis() - g_startMillis) / 1000;
  //Serial.print(l_gasLevel);
  WiFiClient client;
  const int l_httpPort = 80;
  if (!client.connect(g_host, l_httpPort)) {
    Serial.println("Thingspeak Connection Failed");
    return;
  }

  String url = "/update?key=";
  url += g_apiKey;
  url += "&field2=";
  url += String(l_gasLevel);
  //Serial.println(url);
  
  client.print(String("GET ") + url + " HTTP/1.1\r\n" +
             "Host: " + g_host + "\r\n" + 
             "Connection: close\r\n\r\n");
  delay(10);
  
  while(client.available()){
    String line = client.readStringUntil('\r');
    Serial.print(line);
  }
  Serial.println("ThingSpeak Connection Closed");
  delay(10000);
}

//End of the Program
