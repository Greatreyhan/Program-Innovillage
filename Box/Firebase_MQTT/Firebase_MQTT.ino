
#if defined(ESP32)
#include <WiFi.h>
#include <FirebaseESP32.h>
#elif defined(ESP8266)
#include <ESP8266WiFi.h>
#include <PubSubClient.h>
#include <FirebaseESP8266.h>
#endif

//Provide the token generation process info.
#include "addons/TokenHelper.h"
//Provide the RTDB payload printing info and other helper functions.
#include "addons/RTDBHelper.h"

// WiFi
const char *ssid = "infinergy"; // Enter your WiFi name
const char *password = "okeokeoke";  // Enter WiFi password

// MQTT Broker
const char *mqtt_broker = "broker.emqx.io";
const char *topic = "aldeaInnovation/B1";
const char *mqtt_username = "emqx";
const char *mqtt_password = "public";
const int mqtt_port = 1883;

WiFiClient espClient;
PubSubClient client(espClient);

/* 1. Define the WiFi credentials */
#define WIFI_SSID "infinergy"
#define WIFI_PASSWORD "okeokeoke"

/* 2. Define the API Key */
#define API_KEY "AIzaSyBJlVizGNjbAkK6L1MGS_IFEeTEVjjzm2Q"

/* 3. Define the RTDB URL */
#define DATABASE_URL "aldeainnovillage-default-rtdb.firebaseio.com" 

/* 4. Define the user Email and password that alreadey registerd or added in your project */
#define USER_EMAIL "maulanareyhan252@gmail.com"
#define USER_PASSWORD "Maulana252"

//Define Firebase Data object
FirebaseData fbdo;

FirebaseAuth auth;
FirebaseConfig config;

#include <DHT.h> 
#define DHTPIN D5   
#define DHTTYPE DHT22

#define soil A0
#define RCWL D7

DHT dht ( DHTPIN, DHTTYPE ) ;
 
unsigned long sendDataPrevMillis = 0;
unsigned long previousMillis = 0;
const long interval = 10000;

int count = 0;

void setup()
{

  Serial.begin(115200);

  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  Serial.print("Connecting to Wi-Fi");
  while (WiFi.status() != WL_CONNECTED)
  {
    Serial.print(".");
    delay(300);
  }
  Serial.println();
  Serial.print("Connected with IP: ");
  Serial.println(WiFi.localIP());
  Serial.println();

  Serial.printf("Firebase Client v%s\n\n", FIREBASE_CLIENT_VERSION);

  /* Assign the api key (required) */
  config.api_key = API_KEY;

  /* Assign the user sign in credentials */
  auth.user.email = USER_EMAIL;
  auth.user.password = USER_PASSWORD;

  /* Assign the RTDB URL (required) */
  config.database_url = DATABASE_URL;

  /* Assign the callback function for the long running token generation task */
  config.token_status_callback = tokenStatusCallback; //see addons/TokenHelper.h

  Firebase.begin(&config, &auth);

  client.setServer(mqtt_broker, mqtt_port);
  client.setCallback(callback);
  while (!client.connected()) {
      String client_id = "esp8266-client-";
      client_id += String(WiFi.macAddress());
      Serial.printf("The client %s connects to the public mqtt broker\n", client_id.c_str());
      if (client.connect(client_id.c_str(), mqtt_username, mqtt_password)) {
          Serial.println("Public emqx mqtt broker connected");
      } else {
          Serial.print("failed with state ");
          Serial.print(client.state());
          delay(2000);
      }
  }
  client.subscribe(topic);

  pinMode(RCWL, INPUT);
  dht.begin ( );
}

void callback(char *topic, byte *payload, unsigned int length) {
//  digitalWrite(LED0,HIGH);
  Serial.print("Message arrived in topic: ");
  Serial.println(topic);
  Serial.print("Message:");

  Serial.println();
  Serial.println("-----------------------");
  delay(100);
//  digitalWrite(LED0, LOW);
}
void loop()
{
   client.loop();
   if(digitalRead(RCWL)== 1){
    client.publish(topic, "1");
   }
   else{
    client.publish(topic, "0");
   }
   unsigned long currentMillis = millis();
   float humidity = dht.readHumidity( );
   float temprature = dht.readTemperature( ); 
   int soil = analogRead(soil);
   
   if (currentMillis - previousMillis >= interval) {
    // save the last time you blinked the LED
    previousMillis = currentMillis;

      if (Firebase.ready())
      {
        if (Firebase.pushInt(fbdo, "/test/humidity", humidity)) {
    
            Serial.println(fbdo.dataPath());
          
            Serial.println(fbdo.pushName());
          
            Serial.println(fbdo.dataPath() + "/"+ fbdo.pushName());
          
          } else {
            Serial.println(fbdo.errorReason());
          }
          if (Firebase.pushInt(fbdo, "/test/temperature", temprature)) {
    
            Serial.println(fbdo.dataPath());
          
            Serial.println(fbdo.pushName());
          
            Serial.println(fbdo.dataPath() + "/"+ fbdo.pushName());
          
          } else {
            Serial.println(fbdo.errorReason());
          }
          if (Firebase.pushInt(fbdo, "/test/soil", soil)) {
    
            Serial.println(fbdo.dataPath());
          
            Serial.println(fbdo.pushName());
          
            Serial.println(fbdo.dataPath() + "/"+ fbdo.pushName());
          
          } else {
            Serial.println(fbdo.errorReason());
          }
      }

  }
  
}
