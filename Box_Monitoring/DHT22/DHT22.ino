#include <DHT.h> 
 #define DHTPIN D5 // The data pin of DHT11/DHT22 should be connected to the digtal pin 9 of Arduino.   
 #define DHTTYPE DHT22
 DHT dht ( DHTPIN, DHTTYPE ) ;
 void setup ( ) { // Void setup is the function which technically created at the top of the program. It is used to initialize variables, pin modes, start using libraries, etc. 
   Serial.begin ( 9600 ) ;
   dht.begin (  ) ;    
 }
 void loop ( ) { // The loop is ran again and again and consists the main code.

float humidity = dht.readHumidity ( ) ;  
  float Temprature = dht.readTemperature ( ) ; 
   if ( isnan ( Temprature ) || isnan ( humidity ) ) {
     Serial.println ( " Sensor is not avaliable right now " ) ;  }

 else

{

Serial.print ( " Temprature is " ) ;  
     Serial.print ( Temprature ) ;         
     Serial.println ( " *C " ) ;      
     Serial.print ( " Humidity in % is : " ) ;                    
     Serial.print ( humidity ) ;   
     Serial.print ( " % \t " ) ;     
                                                           
   }
 }
