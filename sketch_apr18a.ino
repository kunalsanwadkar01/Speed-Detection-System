


#include <ESP8266WiFi.h>


String apiKey = "6QEEITXCA6F1HWDC";
const char *ssid =  "Rushikesh";
const char *pass =  "12345678";
const char* server = "api.thingspeak.com";


#define sense1 D2
#define sense2 D3
WiFiClient client;

void setup()
{ 
  Serial.begin(115200);
  pinMode(sense1, INPUT);
  pinMode(sense2, INPUT);
  WiFi.begin(ssid, pass);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi connected");
}


void loop()
{
  double calibrate=0;
  float time_for_speed=0, velocity=0 ; 
  boolean active=0;
  calibrate= millis();

   
    if( digitalRead(sense1)==1)
    {
        while(digitalRead(sense2)!=1);
        time_for_speed= millis()- calibrate;
        while(digitalRead(sense2)!=0);
        velocity= 216/time_for_speed;   // check note 1 comments below to see how 216 came here
        active=1;                       // check note 2 comments below to understand the use of this variable
    }

    if(active==1)
    {
      Serial.print("speed ");
      Serial.print(velocity);
      Serial.println(" km/h");
      delay(2000);
      active=0;
    }

    if (client.connect(server, 80)) {
    String postStr = apiKey;
    postStr += "&field1=";
    postStr += String(velocity);
    postStr += "\r\n\r\n";

    client.print("POST /update HTTP/1.1\n");
    client.print("Host: api.thingspeak.com\n");
    client.print("Connection: close\n");
    client.print("X-THINGSPEAKAPIKEY: " + apiKey + "\n");
    client.print("Content-Type: application/x-www-form-urlencoded\n");
    client.print("Content-Length: ");
    client.print(postStr.length());
    client.print("\n\n");
    client.print(postStr);

  }
  client.stop();
  delay(1000);

}
