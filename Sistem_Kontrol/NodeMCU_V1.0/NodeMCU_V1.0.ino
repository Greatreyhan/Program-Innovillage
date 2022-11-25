#include <ESP8266WiFi.h>
#include <PubSubClient.h>
#include <FirebaseESP8266.h>
#include <NTPClient.h>
#include <WiFiUdp.h>

#define LEDP D7
#define LED1 D1
#define LED2 D2
#define LED3 D5
#define LED4 D6

//Provide the token generation process info.
#include "addons/TokenHelper.h"
//Provide the RTDB payload printing info and other helper functions.
#include "addons/RTDBHelper.h"

// Master Time
long timeMaster = 0;

// Parameter Pewaktu Penyiraman -> Kontrol 1 Relay (Valve)
long timesiram1 = 6*3600; // Jam 6 Pagi
long timesiram2 = 11*3600; // Jam 11 Siang
long timesiram3 = 17*3600; // Jam 5 Sore
long durationsiram = 30*60; // Durasi Penyiraman 30 menit

// Parameter Pewaktu Penyinaran -> Kontrol 2 Relay (Saklar Lampu)
long timesinar = 18*3600; // Waktu Mulai Penyinaran jam 6 sore
long durationsinar = 5*3600; // Durasi Penyinaran selama 5 jam

// Parameter Pewaktu Pemupukan -> Kontrol 1  Relay (Pompa)
long daypupuk = 15; // Tanggal pemupukan 15
long timepupuk = 6*3600; // Waktu Mulai Pemupukan
long durationpupuk = 15*60; // Durasi Pemupukan selama 15 menit

// Parameter Sistem Security
int sSecurity = 0; // kondisi tidak aktif

// Parameter Monitoring
int monitor1, monitor2, monitor3, monitor4;

// Parameter Sensor LDR
int sensor1, sensor2, sensor3, sensor4;

// Parameter Sensor RWCL
int rcwl1, rcwl2, rcwl3, rcwl4;

// Parameter Timer
  String formattedTime;
  int currentHour,currentMinute,currentSecond ;
  struct tm *ptm; 
  int monthDay,currentMonth,currentYear;

// WiFi
const char *ssid = "infinergy"; // Enter your WiFi name
const char *password = "okeokeoke";  // Enter WiFi password

// Define NTP Client to get time
WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP, "pool.ntp.org");

// MQTT Broker
const char *mqtt_broker = "broker.emqx.io";
const char *topic = "aldeaInnovation/Control";
const char *mqtt_username = "aldeatech";
const char *mqtt_password = "aldeatech";
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

void setup() {
  // Set software serial baud to 115200;
  pinMode(LED1, OUTPUT);
  pinMode(LED2, OUTPUT);
  pinMode(LED3, OUTPUT);
  pinMode(LED4, OUTPUT);
  pinMode(LEDP, OUTPUT);
  Serial.begin(115200);
  // connecting to a WiFi network
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
      delay(500);
      Serial.println("Connecting to WiFi..");
  }
  Serial.println("Connected to the WiFi network");
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
  //connecting to a mqtt broker
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
  
  // publish and subscribe
  client.publish(topic, "ready");
  client.subscribe(topic);

  // NTPClient to get Time
  timeClient.begin();
  timeClient.setTimeOffset(28800-3600);
}

void callback(char *topic, byte *payload, unsigned int length) {
  digitalWrite(LEDP,HIGH);
  Serial.print("Message arrived in topic: ");
  Serial.println(topic);
  Serial.print("Message:");
  for (int i = 0; i < length; i++) {
      if(i == 0 && (char)payload[i] == '1'){
        digitalWrite(LED1, HIGH);
        Serial.print("1");
      }
      if(i == 0 && (char)payload[i] == '0'){
        digitalWrite(LED1, LOW);
        Serial.print("0");
      }
      if(i == 1 && (char)payload[i] == '1'){
        digitalWrite(LED2, HIGH);
        Serial.print("1");
      }
      if(i == 1 && (char)payload[i] == '0'){
        digitalWrite(LED2, LOW);
        Serial.print("0");
      }
      if(i == 2 && (char)payload[i] == '1'){
        digitalWrite(LED3, HIGH);
        Serial.print("1");
      }
      if(i == 2 && (char)payload[i] == '0'){
        digitalWrite(LED3, LOW);
        Serial.print("0");
      }
      if(i == 3 && (char)payload[i] == '1'){
        digitalWrite(LED4, HIGH);
        Serial.print("1");
      }
      if(i == 3 && (char)payload[i] == '0'){
        digitalWrite(LED4, LOW);
        Serial.print("0");
      }
  }
  Serial.println();
  Serial.println("-----------------------");
  delay(100);
  digitalWrite(LEDP, LOW);
}

void loop() {
  client.loop();
  Serial.println(timeMaster);
  settingTimer();
  logicTimer();
  getFirebase();
//  testing();
}

void testing(){
  sendBit(1,1,0,0);
  Serial.println("Penyiraman");
  delay(2000);
  sendBit(1,1,0,1);
  Serial.println("Penyinaran");
  delay(2000);
  sendBit(1,1,1,0);
  Serial.println("Pemupukan");
  delay(2000);
  sendBit(0,0,0,0);
  Serial.println("Selesai");
  delay(2000);
}

void logicTimer(){
  // Pewaktu Penyiraman -> ON : 1100
  if((timesiram1 <= timeMaster <= (timesiram1+durationsiram)) || (timesiram2 <= timeMaster <= (timesiram2+durationsiram))|| (timesiram3 <= timeMaster <= (timesiram3+durationsiram))){
    sendBit(1,1,0,0);
    Serial.println("Memulai Penyiraman");
  }

  // Pewaktu Penyinaran -> ON : 1101
  if(timesinar <= timeMaster <= (timesinar+durationsinar)){
    sendBit(1,1,0,1);
    Serial.println("Memulai Penyinaran");
  }

  // Pewaktu Pemupukan -> ON : 1110
  if(monthDay == daypupuk){
    if(timepupuk <= timeMaster <= (timepupuk+durationpupuk)){
      sendBit(1,1,1,0);
      Serial.println("Memulai Pemupukan");
    }
  }

  // Kondisi Keamanan Terdeteksi -> ON : 1111
  if((rcwl1 || rcwl2 || rcwl3 || rcwl4)){
    sendBit(1,1,1,1);
    Serial.println("Mendeteksi Manusia");
  }

  // Kondisi Pencahayaan -> ON : 1101
  if((sensor1+sensor2+sensor3+sensor4) > 2){
    sendBit(1,1,0,1);
    Serial.println("Memulai Penyinaran");
  }

  // default system
  if(!((timesiram1 <= timeMaster <= (timesiram1+durationsiram)) || (timesiram2 <= timeMaster <= (timesiram2+durationsiram))|| (timesiram3 <= timeMaster <= (timesiram3+durationsiram))) && !(timesinar <= timeMaster <= (timesinar+durationsinar)) && !(monthDay == daypupuk) ){
    sendBit(0,0,0,0);
    Serial.println("Menyelesaikan Tugas");
  }
}

void settingTimer(){
  timeClient.update();
  time_t epochTime = timeClient.getEpochTime();
  formattedTime = timeClient.getFormattedTime();
  currentHour = timeClient.getHours(); 
  currentMinute = timeClient.getMinutes();
  currentSecond = timeClient.getSeconds();  
  tm *ptm = gmtime ((time_t *)&epochTime); 
  monthDay = ptm->tm_mday; 
  currentMonth = ptm->tm_mon+1; 
  currentYear = ptm->tm_year+1900; 

  timeMaster = (currentHour*3600)+(currentMinute*60)+currentSecond;
}

void getFirebase(){
  if (Firebase.ready()){
       if (Firebase.RTDB.getInt(&fbdo, "/action/monitoring/monitor1")) {
          if (fbdo.dataType() == "int") {
            monitor1 = fbdo.intData();
//            Serial.println(monitor1);
          }
        }
        if (Firebase.RTDB.getInt(&fbdo, "/action/monitoring/monitor2")) {
          if (fbdo.dataType() == "int") {
            monitor2 = fbdo.intData();
//            Serial.println(monitor2);
          }
        }
        if (Firebase.RTDB.getInt(&fbdo, "/action/monitoring/monitor3")) {
          if (fbdo.dataType() == "int") {
            monitor3 = fbdo.intData();
//            Serial.println(monitor3);
          }
        }
        if (Firebase.RTDB.getInt(&fbdo, "/action/monitoring/monitor4")) {
          if (fbdo.dataType() == "int") {
            monitor4 = fbdo.intData();
//            Serial.println(monitor4);
          }
        }
        if (Firebase.RTDB.getInt(&fbdo, "/action/pemupukan/durationpupuk")) {
          if (fbdo.dataType() == "int") {
            durationpupuk = fbdo.intData();
//            Serial.println(durationpupuk);
          }
        }
        if (Firebase.RTDB.getInt(&fbdo, "/action/pemupukan/timepupuk")) {
          if (fbdo.dataType() == "int") {
            timepupuk = fbdo.intData();
//            Serial.println(timepupuk);
          }
        }
        if (Firebase.RTDB.getInt(&fbdo, "/action/pemupukan/daypupuk")) {
          if (fbdo.dataType() == "int") {
            daypupuk = fbdo.intData();
//            Serial.println(daypupuk);
          }
        }
        if (Firebase.RTDB.getInt(&fbdo, "/action/penyinaran/durationsinar")) {
          if (fbdo.dataType() == "int") {
            durationsinar = fbdo.intData();
//            Serial.println(durationsinar);
          }
        }
        if (Firebase.RTDB.getInt(&fbdo, "/action/penyinaran/timesinar")) {
          if (fbdo.dataType() == "int") {
            timesinar = fbdo.intData();
//            Serial.println(timesinar);
          }
        }
        if (Firebase.RTDB.getInt(&fbdo, "/action/penyinaran/sensor1")) {
          if (fbdo.dataType() == "int") {
            sensor1 = fbdo.intData();
//            Serial.println(sensor1);
          }
        }
        if (Firebase.RTDB.getInt(&fbdo, "/action/penyinaran/sensor2")) {
          if (fbdo.dataType() == "int") {
            sensor2 = fbdo.intData();
//            Serial.println(sensor2);
          }
        }
        if (Firebase.RTDB.getInt(&fbdo, "/action/penyinaran/sensor3")) {
          if (fbdo.dataType() == "int") {
            sensor3 = fbdo.intData();
//            Serial.println(sensor3);
          }
        }
        if (Firebase.RTDB.getInt(&fbdo, "/action/penyinaran/sensor4")) {
          if (fbdo.dataType() == "int") {
            sensor4 = fbdo.intData();
//            Serial.println(sensor4);
          }
        }
        if (Firebase.RTDB.getInt(&fbdo, "/action/penyiraman/durationsiram")) {
          if (fbdo.dataType() == "int") {
            durationsiram = fbdo.intData();
//            Serial.println(durationsiram);
          }
        }
        if (Firebase.RTDB.getInt(&fbdo, "/action/penyiraman/timesiram1")) {
          if (fbdo.dataType() == "int") {
            timesiram1 = fbdo.intData();
//            Serial.println(timesiram1);
          }
        }
        if (Firebase.RTDB.getInt(&fbdo, "/action/penyiraman/timesiram2")) {
          if (fbdo.dataType() == "int") {
            timesiram2 = fbdo.intData();
//            Serial.println(timesiram2);
          }
        }
        if (Firebase.RTDB.getInt(&fbdo, "/action/penyiraman/timesiram3")) {
          if (fbdo.dataType() == "int") {
            timesiram3 = fbdo.intData();
//            Serial.println(timesiram3);
          }
        }
        if (Firebase.RTDB.getInt(&fbdo, "/action/keamanan/rcwl1")) {
          if (fbdo.dataType() == "int") {
              rcwl1 = fbdo.intData();
//            Serial.println(rcwl1);
          }
        }
        if (Firebase.RTDB.getInt(&fbdo, "/action/keamanan/rcwl2")) {
          if (fbdo.dataType() == "int") {
              rcwl2 = fbdo.intData();
//            Serial.println(rcwl2);
          }
        }
        if (Firebase.RTDB.getInt(&fbdo, "/action/keamanan/rcwl3")) {
          if (fbdo.dataType() == "int") {
              rcwl3 = fbdo.intData();
//            Serial.println(rcwl3);
          }
        }
        if (Firebase.RTDB.getInt(&fbdo, "/action/keamanan/rcwl4")) {
          if (fbdo.dataType() == "int") {
              rcwl4 = fbdo.intData();
//            Serial.println(rcwl4);
          }
        }
      }
}

void sendBit(byte a, byte b, byte c, byte d){
  digitalWrite(LEDP, HIGH);
  if(a == 1){
    digitalWrite(LED1, HIGH);
  }
  else{
    digitalWrite(LED1, LOW);
  }
  if(b == 1){
    digitalWrite(LED2, HIGH);
  }
  else{
    digitalWrite(LED2, LOW);
  }
  if(c == 1){
    digitalWrite(LED3, HIGH);
  }
  else{
    digitalWrite(LED3, LOW);
  }
  if(d == 1){
    digitalWrite(LED4, HIGH);
  }
  else{
    digitalWrite(LED4, LOW);
  }
  digitalWrite(LEDP, LOW);
}
