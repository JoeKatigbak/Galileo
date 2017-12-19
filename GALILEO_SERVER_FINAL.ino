#include <SPI.h>
#include <ESP8266WiFi.h>
#include <Sim800l.h>
#include <SoftwareSerial.h>

Sim800l Sim800l;
char* text;
char* number;
bool error;

char ssid[]="GALILEO-NET";
char password[]="GALILEO@JNC";
WiFiServer server(80);

IPAddress ip(192,168,2,135);
IPAddress gateway(192,168,1,1);
IPAddress subnet(255,255,255,0);

int relay1 =14;
int relay2 =12;

String reading;

void setup() {
  // put your setup code here, to run once:
  Sim800l.begin();
  Serial.begin(115200);
  WiFi.config(ip,gateway,subnet);
  WiFi.begin(ssid,password);
  pinMode(LED_BUILTIN,OUTPUT);
  pinMode(relay1,OUTPUT);
  pinMode(relay2,OUTPUT);
  

  while ((!(WiFi.status()==WL_CONNECTED)))
  {
     delay(300);
    Serial.print(".");
   
  }
  Serial.print("Wifi Connected");
  server.begin();
  
}

void loop() {
  WiFiClient client= server.available();
  
  if (client)
  { Serial.println("Client Connected");
    if (client.connected()){
     String reading=client.readStringUntil('\r');
     if(reading==("GET /on HTTP/1.1"))
        {
          Serial.println("sensor on");
          digitalWrite(relay1,HIGH);
          delay(3000);
          String reading=client.readStringUntil('\r');
          Serial.println(reading);
        }
     else if(reading=="HIGH")
          {
          Serial.println("PIR Motion Detected");
          Serial.println("Sending Alert to +639175603150");
          delay(100);
          text="Intruder Detected";
          number="+639453463030";
          error=Sim800l.sendSms(number,text);
          Serial.println(error);
          Serial.println("message sent");
          client.println("GET /off HTTP/1.1");
          }
     else if(reading=="LOW") 
           {
          Serial.println("PIR detecting...");
          delay(1000);
           }
     if(reading=="GET /off HTTP/1.1")
          {
          Serial.println("Sensor is deactivated");
          digitalWrite(relay1, LOW);
          delay(1000);
          }
       }
          
        }

     
    client.flush();
    }

