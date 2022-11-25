int sensorPin = A0; 
int sensorValue;  
int limit = 300; 

void setup() {
 Serial.begin(9600);
}

void loop() {

 sensorValue = analogRead(sensorPin); 
 Serial.println("Analog Value : ");
 Serial.println(sensorValue);
 
 delay(1000); 
}
