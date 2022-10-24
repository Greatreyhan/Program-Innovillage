#define D1 5
#define D2 6
#define D3 7
#define D4 8
#define DP 9
#define IN1 10
#define IN2 11
#define IN3 12
#define IN4 13

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
          break;
        case 1:
          digitalWrite(IN1,HIGH);
          digitalWrite(IN2,LOW);
          digitalWrite(IN3,LOW);
          digitalWrite(IN4,LOW);
          break;
        case 2:
          digitalWrite(IN1,LOW);
          digitalWrite(IN2,HIGH);
          digitalWrite(IN3,LOW);
          digitalWrite(IN4,LOW);
          break;
        case 3:
          digitalWrite(IN1,LOW);
          digitalWrite(IN2,LOW);
          digitalWrite(IN3, HIGH);
          digitalWrite(IN4,LOW);
          break;
        case 4:
          digitalWrite(IN1,LOW);
          digitalWrite(IN2,LOW);
          digitalWrite(IN3,LOW);
          digitalWrite(IN4, HIGH);
          break;
        default:
          Serial.println("Error Transmission");
          break;
      }
    }
}
