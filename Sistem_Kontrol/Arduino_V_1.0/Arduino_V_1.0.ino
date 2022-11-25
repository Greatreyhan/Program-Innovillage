#define D1 11
#define D2 10
#define D3 9
#define D4 8
#define DP 7
#define IN1 3
#define Buzzer 4
#define IN3 5
#define IN4 6
#define Valve A1
#define LampB A0
#define LampA 2
#define Pump 13

void setup() {
  pinMode(D1, INPUT);
  pinMode(D2, INPUT);
  pinMode(D3, INPUT);
  pinMode(D4, INPUT);
  pinMode(DP, INPUT);
  pinMode(IN1, OUTPUT);
  pinMode(Buzzer, OUTPUT);
  pinMode(IN3, OUTPUT);
  pinMode(IN4, OUTPUT);
  pinMode(Valve, OUTPUT);
  pinMode(LampB, OUTPUT);
  pinMode(LampA, OUTPUT);
  pinMode(Pump, OUTPUT);
  Serial.begin(9600);
}

byte d1,d2,d3,d4,dp;
byte integer = 0;
void loop() {
    d1 = digitalRead(D1);
    d2 = digitalRead(D2);
    d3 = digitalRead(D3);
    d4 = digitalRead(D4);
    dp = digitalRead(DP);
    Serial.print(dp);
    Serial.print(d1);
    Serial.print(d2);
    Serial.print(d3);
    Serial.println(d4);
      
      if(d1 == 0 && d2 == 0 && d3 == 0 && d4 == 0){
          digitalWrite(IN1,LOW);
          digitalWrite(Buzzer,LOW);
          digitalWrite(IN3,LOW);
          digitalWrite(IN4,LOW);
          digitalWrite(Valve,LOW);
          digitalWrite(LampB,LOW);
          digitalWrite(LampA,LOW);
          digitalWrite(Pump,LOW);
          delay(1000);
      }
      else if(d1 == 1 && d2 == 1 && d3 == 0 && d4 == 0){
          Serial.println("Penyiraman");
          digitalWrite(Valve,HIGH);
          delay(1000);
      }
      else if(d1 == 1 && d2 == 1 && d3 == 0 && d4 == 1){
          Serial.println("Penyinaran");
          digitalWrite(LampA, HIGH);
          digitalWrite(LampB, HIGH);
          delay(1000);
      }
      else if(d1 == 1 && d2 == 1 && d3 == 1 && d4 == 0){
          Serial.println("Pemupukan");
          digitalWrite(Pump,HIGH);
          delay(1000);
      }
      else if(d1 == 1 && d2 == 1 && d3 == 1 && d4 == 1){
          Serial.println("Keamanan");
          digitalWrite(Buzzer,HIGH);
          delay(1000);
      }
      
}
