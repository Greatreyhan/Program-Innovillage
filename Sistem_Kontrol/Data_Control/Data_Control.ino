#define D1 11
#define D2 10
#define D3 9
#define D4 8
#define DP 7
#define IN1 3
#define IN2 4
#define IN3 5
#define IN4 6
#define IN5 A1
#define IN6 A0
#define IN7 2
#define IN8 13

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
  pinMode(IN5, OUTPUT);
  pinMode(IN6, OUTPUT);
  pinMode(IN7, OUTPUT);
  pinMode(IN8, OUTPUT);
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
//      Serial.print(d1);
//      Serial.print(d2);
//      Serial.print(d3);
//      Serial.print(d4);
      integer = d4*1 + d3*2 + d2*4 + d1*8;
      Serial.print(integer);
      switch(integer){
        case 0:
          digitalWrite(IN1,LOW);
          digitalWrite(IN2,LOW);
          digitalWrite(IN3,LOW);
          digitalWrite(IN4,LOW);
          digitalWrite(IN5,LOW);
          digitalWrite(IN6,LOW);
          digitalWrite(IN7,LOW);
          digitalWrite(IN8,LOW);
          break;
        case 1:
          digitalWrite(IN1,HIGH);
          digitalWrite(IN2,LOW);
          digitalWrite(IN3,LOW);
          digitalWrite(IN4,LOW);
          digitalWrite(IN5,LOW);
          digitalWrite(IN6,LOW);
          digitalWrite(IN7,LOW);
          digitalWrite(IN8,LOW);
          break;
        case 2:
          digitalWrite(IN1,LOW);
          digitalWrite(IN2,HIGH);
          digitalWrite(IN3,LOW);
          digitalWrite(IN4,LOW);
          digitalWrite(IN5,LOW);
          digitalWrite(IN6,LOW);
          digitalWrite(IN7,LOW);
          digitalWrite(IN8,LOW);
          break;
        case 3:
          digitalWrite(IN1,LOW);
          digitalWrite(IN2,LOW);
          digitalWrite(IN3, HIGH);
          digitalWrite(IN4,LOW);
          digitalWrite(IN5,LOW);
          digitalWrite(IN6,LOW);
          digitalWrite(IN7,LOW);
          digitalWrite(IN8,LOW);
          break;
        case 4:
          digitalWrite(IN1,LOW);
          digitalWrite(IN2,LOW);
          digitalWrite(IN3,LOW);
          digitalWrite(IN4, HIGH);
          digitalWrite(IN5,LOW);
          digitalWrite(IN6,LOW);
          digitalWrite(IN7,LOW);
          digitalWrite(IN8,LOW);
          break;
         case 5:
          digitalWrite(IN1,LOW);
          digitalWrite(IN2,LOW);
          digitalWrite(IN3,LOW);
          digitalWrite(IN4,LOW);
          digitalWrite(IN5,HIGH);
          digitalWrite(IN6,LOW);
          digitalWrite(IN7,LOW);
          digitalWrite(IN8,LOW);
          break;
         case 6:
          digitalWrite(IN1,LOW);
          digitalWrite(IN2,LOW);
          digitalWrite(IN3,LOW);
          digitalWrite(IN4,LOW);
          digitalWrite(IN5,LOW);
          digitalWrite(IN6,HIGH);
          digitalWrite(IN7,LOW);
          digitalWrite(IN8,LOW);
          break;
         case 7:
          digitalWrite(IN1,LOW);
          digitalWrite(IN2,LOW);
          digitalWrite(IN3,LOW);
          digitalWrite(IN4,LOW);
          digitalWrite(IN5,LOW);
          digitalWrite(IN6,LOW);
          digitalWrite(IN7,HIGH);
          digitalWrite(IN8,LOW);
          break;
         case 8:
          digitalWrite(IN1,LOW);
          digitalWrite(IN2,LOW);
          digitalWrite(IN3,LOW);
          digitalWrite(IN4,LOW);
          digitalWrite(IN5,LOW);
          digitalWrite(IN6,LOW);
          digitalWrite(IN7,LOW);
          digitalWrite(IN8,HIGH);
          break;
        default:
          Serial.println("Error Transmission");
          break;
      }
    }
}
