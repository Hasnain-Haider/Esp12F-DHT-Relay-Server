#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <DHT.h>
#define DHTPIN D7
#define RELAYPIN D4 

const char* ssid     = "Esa's Crib";
const char* password = "********";
ESP8266WebServer server(80);
float temp=0, humid=0, HI=0;
DHT dht(DHTPIN, DHT22);
String rstat = "?";

void setup()
{
  Serial.begin(115200);
  pinMode(DHTPIN, INPUT);
  pinMode(RELAYPIN, OUTPUT);
  delay(500);
  scan();
  Serial.print("Connecting to ");
  Serial.println(ssid);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED)
  {
    delay(500);
    Serial.print(".");
  }
 
  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());

  server.on("/",dod);
  server.on("/RELAYON",ron);
  server.on("/RELAYOFF",roff);
  server.begin();
  WiFi.mode(WIFI_STA); 
}
//place holder
void dod(){}
void readDHT()
{
    humid = dht.readHumidity();          // Read humidity (percent)
    temp  = dht.readTemperature(true);     // Read temperature as Fahrenheit
    HI   = dht.computeHeatIndex(dht.readTemperature(), humid);
}
void loop()
{
    readDHT();
    String dis = Site();
    server.send(200,"text/html", dis);
    server.handleClient();
}

String Site()
{
  String site= " <!DOCTYPE html>";
  site += ("<Html><Head>");
  site += ("<Title>ESA's Webpage</title>");
  site += ("");
  site += ("</Head>");
  site += ("<Body>\n");
  site += ("<H5>RELAY</H5>\n<ul>\n ");
  site += ("status : ");
  site += rstat;
  site += (" value");
  site += ("<li><a href=\"/RELAYON\"><button>RELAYON</button></a></li>");
  site += ("<li><a href=\"/RELAYOFF\"><button>RELAYOFF</button></a></li></ul>");
  site += ("\n<H5>DHT 22</H5>\nDHT Temperature Reading : \n<ul>\n<li>");
  site += temp; 
  site += ("F</li>\n");
  site += ("<li>DHT Humidity Reading : ");
  site += humid;
  site += ("%</li>");
  site += ("\n");
  site += ("<li>Heat Index : ");
  site += HI;
  site += ("</li>\n</ul>");
  site += ("<FORM>\n<INPUT TYPE=\"button\" onClick=\"history.go(0)\" VALUE=\"Refresh\">\n</FORM>");
  site +=  ("</Body></Html>");    
  return site;
}

void scan()
{  int n = WiFi.scanNetworks();
  for (int i = 0; i < n; ++i)
  {
    // Print SSID and RSSI for each network found
    Serial.print(i + 1);
    Serial.print(": ");
    Serial.print(WiFi.SSID(i));
    Serial.print(" (");
    Serial.print(WiFi.RSSI(i));
    Serial.print(")");
    Serial.println((WiFi.encryptionType(i) == ENC_TYPE_NONE) ? " " : "*");
    delay(10);
  }
}

void ron()
{
    digitalWrite(RELAYPIN, HIGH);
    rstat = "ON";
}

void roff()
{
    digitalWrite(RELAYPIN, LOW);
    rstat = "OFF";
}
