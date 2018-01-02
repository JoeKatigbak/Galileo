#include <SPI.h>
#include <ESP8266WiFi.h>

char ssd[] = "GALILEO-NET";
char password[] = "GALILEO@JNC";

IPAddress server(192, 168, 2, 135);
WiFiClient client;

int pir = 14;
int pirState = LOW;
int gsmtrigger = 2;
int calibrationTime = 30;
String c;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  WiFi.begin(ssd, password);
  while (WiFi.status() != WL_CONNECTED)
  {
    Serial.print(".");
    delay(500);
    //delay(calibrationTime);
  }

  client.connect(server, 80);
  client.println("GET /on HTTP/1.1\r");
  pinMode(pir, INPUT);
  pinMode(gsmtrigger, OUTPUT);
}

void loop() {
  // put your main code here, to run repeatedly:
  client.connect(server, 80);
  int answer = digitalRead(pir);
  if (answer == 1)
  {
    client.println("HIGH\r");
    pinMode(gsmtrigger, HIGH);
    delay(1000);
    client.flush();
    delay(1000);
  }
  if (answer == 0)
  {
    client.println("LOW\r");
    pinMode(gsmtrigger, LOW);
    delay(100);
    client.flush();
    delay(100);
  }
}

