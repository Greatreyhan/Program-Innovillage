#define D1 11
#define D2 10
#define D3 9
#define D4 8
#define DP 7
#define IN1 3
#define IN2 4
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
  pinMode(IN2, OUTPUT);
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
    if(dp){
      integer = d4*1 + d3*2 + d2*4 + d1*8;
      Serial.print(integer);
      switch(integer){
        case 0:
          digitalWrite(IN1,LOW);
          digitalWrite(IN2,LOW);
          digitalWrite(IN3,LOW);
          digitalWrite(IN4,LOW);
          digitalWrite(Valve,LOW);
          digitalWrite(LampB,LOW);
          digitalWrite(LampA,LOW);
          digitalWrite(Pump,LOW);
          break;
        case 12: // Menyiram Tanaman
          digitalWrite(Valve,HIGH);
          break;
        case 13: // Penyinaran Taaman
          digitalWrite(LampB,HIGH);
          digitalWrite(LampA,HIGH);
          break;
        case 14: // Pemupukan Tanaman
          digitalWrite(Pump,HIGH);
          break;
        default:
          Serial.println("Error Transmission");
          break;
      }
    }
}
