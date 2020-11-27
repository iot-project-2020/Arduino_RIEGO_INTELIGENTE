#include <ESP8266WiFi.h>                        // esp8266 library
#include <FirebaseArduino.h>                    // firebase library
#include <NTPClient.h>
#include <WiFiUdp.h>
#define FIREBASE_HOST "iot-crops.firebaseio.com"// the project name address from firebase id
#define FIREBASE_AUTH "IrkKghCs3Xvds66WsBub5h7GwAfKigptjhgHKpI7"// the secret key generated from firebase

#define WIFI_SSID "network_iot"                 // input your home or public wifi name 
#define WIFI_PASSWORD "12345678"                //password of wifi ssid
String crop = "/crop-2/";
const long utcOffsetInSeconds = 3600;
WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP, "pool.ntp.org", utcOffsetInSeconds);

void setup() {
  timeClient.begin();
  timeClient.setTimeOffset(3600);
  Serial.begin(9600);
  delay(1000);
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);   //try to connect with wifi
  Serial.print("Connecting to ");
  Serial.print(WIFI_SSID);
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(500);
  }
  Serial.println();
  Serial.print("Connected to ");
  Serial.println(WIFI_SSID);
  Serial.print("IP Address is : ");
  Serial.println(WiFi.localIP());   //print local IP address
  Firebase.begin(FIREBASE_HOST, FIREBASE_AUTH);    // connect to firebase

}

void loop() {
  while (!timeClient.update()) {
    timeClient.forceUpdate();
  }
  int currentDate = timeClient.getEpochTime();

  float co = random(200, 500) / 10.0;// Generate data CO2 (20 - 50)
  float h = 10.0 + random(100, 1000) / 10.0;// Generate data DHT humedity (10 - 100)
  float t = random(200, 500) / 10.0;// Generate data DHT temperature (20 - 50)
  float hg = random(100, 1000) / 10.0;// Generate data humedity ground (10 - 100)
  float rd = random(10, 1000) / 10.0;// Generate data ratiation (1 - 100)

  StaticJsonBuffer<200> jsonBuffer;


  JsonObject& co2 = jsonBuffer.createObject();
  co2["data"] = co;
  co2["date"] = currentDate;
  Firebase.push(crop + "CO2", co2);
  jsonBuffer.clear();

  JsonObject& hum = jsonBuffer.createObject();
  hum["data"] = hg;
  hum["date"] = currentDate;
  Firebase.push(crop + "Humidity", hum);
  jsonBuffer.clear();

  JsonObject& rad = jsonBuffer.createObject();
  rad["data"] = rd;
  rad["date"] = currentDate;
  Firebase.push(crop + "Radiation", rad);
  jsonBuffer.clear();

  JsonObject& tmpDHT = jsonBuffer.createObject();
  tmpDHT["data"] = h;
  tmpDHT["date"] = currentDate;
  Firebase.push(crop + "temDHT", tmpDHT);
  jsonBuffer.clear();

  JsonObject& humDHT = jsonBuffer.createObject();
  humDHT["data"] = t;
  humDHT["date"] = currentDate;
  Firebase.push(crop + "humDHT", humDHT);
  jsonBuffer.clear();

  // Await 20 seconds to send data
  delay(20000);

}
