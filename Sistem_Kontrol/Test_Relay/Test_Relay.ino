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


void loop() {
//    digitalWrite(IN1,HIGH);
//    digitalWrite(IN2,HIGH);
//    digitalWrite(IN3,HIGH);
//    digitalWrite(IN4,HIGH);
//    digitalWrite(IN5,HIGH);
//    digitalWrite(IN6,HIGH);
//    digitalWrite(IN7,HIGH);
//    digitalWrite(IN8,HIGH);
//    delay(1000);
//    digitalWrite(IN1,LOW);
//    digitalWrite(IN2,LOW);
//    digitalWrite(IN3,LOW);
//    digitalWrite(IN4,LOW);
//    digitalWrite(IN5,LOW);
//    digitalWrite(IN6,LOW);
//    digitalWrite(IN7,LOW);
//    digitalWrite(IN8,LOW);
//    delay(1000);
      digitalWrite(IN4, HIGH);
      delay(1000);
      digitalWrite(IN4, LOW);
      delay(1000);

}
