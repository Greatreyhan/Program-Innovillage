#include "WiFiEsp.h"
#include <PubSubClient.h>

char ssid[] = "infinergy";
char pass[] = "okeokeoke";
int status = WL_IDLE_STATUS;

// MQTT Broker
const char *mqtt_broker = "broker.emqx.io";
const char *topic = "lestari";
const char *mqtt_username = "emqx";
const char *mqtt_password = "public";
const int mqtt_port = 1883;

WiFiEspClient espClient;
PubSubClient client(espClient);

void setup() {
  Serial.begin(9600);

  Serial1.begin(9600);
  WiFi.init(&Serial1);
  if (WiFi.status() == WL_NO_SHIELD) {
    Serial.println("WiFi Module Tidak Ada");
    while (true);
  }
  while ( status != WL_CONNECTED) {
    Serial.print("Mencoba Terhubung ke WiFi dengan SSID: ");
    Serial.println(ssid);
    status = WiFi.begin(ssid, pass);
  }
  Serial.print("Kamu Telah Terhubung Ke WiFi: ");
  Serial.println(ssid);
  
  client.setServer(mqtt_broker, mqtt_port);
  client.setCallback(callback);
  while (!client.connected()) {
      String client_id = "esp8266-client-";
      byte mac[6];
      WiFi.macAddress(mac);
      client_id += String(mac[5])+String(mac[4])+String(mac[3])+String(mac[2])+String(mac[1])+String(mac[0]);
      Serial.print("The client %s connects to the public mqtt broker\n");
      Serial.println(client_id.c_str());
      if (client.connect(client_id.c_str(), mqtt_username, mqtt_password)) {
          Serial.println("Public emqx mqtt broker connected");
      } else {
          Serial.print("failed with state ");
          Serial.print(client.state());
          delay(2000);
      }
  }
  client.publish(topic,"oke");
  client.subscribe(topic);
}

void callback(char *topic, byte *payload, unsigned int length) {
//  digitalWrite(LED0,HIGH);
  Serial.print("Message arrived in topic: ");
  Serial.println(topic);
  Serial.print("Message:");
//  for (int i = 0; i < length; i++) {
//      if(i == 0 && (char)payload[i] == '1'){
//        digitalWrite(LED1, HIGH);
//        Serial.print("1");
//      }
//      if(i == 0 && (char)payload[i] == '0'){
//        digitalWrite(LED1, LOW);
//        Serial.print("0");
//      }
//      if(i == 1 && (char)payload[i] == '1'){
//        digitalWrite(LED2, HIGH);
//        Serial.print("1");
//      }
//      if(i == 1 && (char)payload[i] == '0'){
//        digitalWrite(LED2, LOW);
//        Serial.print("0");
//      }
//      if(i == 2 && (char)payload[i] == '1'){
//        digitalWrite(LED3, HIGH);
//        Serial.print("1");
//      }
//      if(i == 2 && (char)payload[i] == '0'){
//        digitalWrite(LED3, LOW);
//        Serial.print("0");
//      }
//  }
  Serial.println();
  Serial.println("-----------------------");
  delay(100);
}

void loop() {
  client.loop();
}

void printWifiStatus() {
  Serial.print("SSID: ");
  Serial.println(WiFi.SSID());
  IPAddress ip = WiFi.localIP();
  Serial.print("Alamat IP: ");
  Serial.println(ip);
  long rssi = WiFi.RSSI();
  Serial.print("Kekuatan Sinyal (RSSI):");
  Serial.print(rssi);
  Serial.println(" dBm");
}
