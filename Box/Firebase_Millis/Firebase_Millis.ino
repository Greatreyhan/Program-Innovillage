
#if defined(ESP32)
#include <WiFi.h>
#include <FirebaseESP32.h>
#elif defined(ESP8266)
#include <ESP8266WiFi.h>
#include <FirebaseESP8266.h>
#endif

//Provide the token generation process info.
#include "addons/TokenHelper.h"
//Provide the RTDB payload printing info and other helper functions.
#include "addons/RTDBHelper.h"

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

  pinMode(RCWL, INPUT);
  dht.begin ( );
}

float data = 52.3;
void loop()
{
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
