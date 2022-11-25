#include <ESP8266WiFi.h>
#include <PubSubClient.h>
#include <NTPClient.h>
#include <WiFiUdp.h>

#define LEDP D7
#define LED1 D1
#define LED2 D2
#define LED3 D5
#define LED4 D6

// Master Time
long timeMaster = 0;

// Parameter Pewaktu Penyiraman -> Kontrol 1 Relay (Valve)
long tMulaiSiram1 = 6*3600; // Jam 6 Pagi
long tMulaiSiram2 = 11*3600; // Jam 11 Siang
long tMulaiSiram3 = 17*3600; // Jam 5 Sore
long tDurasiSiram = 30*60; // Durasi Penyiraman 30 menit

// Parameter Pewaktu Penyinaran -> Kontrol 2 Relay (Saklar Lampu)
long tMulaiSinar = 18*3600; // Waktu Mulai Penyinaran jam 6 sore
long tDurasiSinar = 5*3600; // Durasi Penyinaran selama 5 jam

// Parameter Pewaktu Pemupukan -> Kontrol 1  Relay (Pompa)
long dPupuk = 15; // Tanggal pemupukan 15
long tMulaiPupuk = 6*3600; // Waktu Mulai Pemupukan
long tDurasiPupuk = 15*60; // Durasi Pemupukan selama 15 menit

// Parameter Sistem Security
int sSecurity = 0; // kondisi tidak aktif

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
  client.publish(topic, "wowoo");
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
  timeClient.update();
  time_t epochTime = timeClient.getEpochTime(); // epochtime
  String formattedTime = timeClient.getFormattedTime(); //10:49:52
  int currentHour = timeClient.getHours(); // 10
  int currentMinute = timeClient.getMinutes(); // 49
  int currentSecond = timeClient.getSeconds(); //52  
  struct tm *ptm = gmtime ((time_t *)&epochTime); 
  int monthDay = ptm->tm_mday; // 11
  int currentMonth = ptm->tm_mon+1; // 11
  int currentYear = ptm->tm_year+1900; // 2022

  timeMaster = (currentHour*3600)+(currentMinute*60)+currentSecond;

  // Pewaktu Penyiraman -> ON : 1100
  if((tMulaiSiram1 <= timeMaster <= (tMulaiSiram1+tDurasiSiram)) || (tMulaiSiram2 <= timeMaster <= (tMulaiSiram2+tDurasiSiram))|| (tMulaiSiram3 <= timeMaster <= (tMulaiSiram3+tDurasiSiram))){
    sendBit(1,1,0,0);
  }

  // Pewaktu Penyinaran -> ON : 1101
  else if(tMulaiSinar <= timeMaster <= (tMulaiSinar+tDurasiSinar)){
    sendBit(1,1,0,1);
  }

  // Pewaktu Pemupukan -> ON : 1110
  else if(monthDay == dPupuk){
    if(tMulaiPupuk <= timeMaster <= (tMulaiPupuk+tDurasiPupuk)){
      sendBit(1,1,1,0);
    }
  }
  else{
    sendBit(0,0,0,0);
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
