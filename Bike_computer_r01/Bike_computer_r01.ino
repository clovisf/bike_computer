#include <LiquidCrystal.h>
LiquidCrystal lcd(12, 11, 5, 4, 3, 2);
#include "DHT.h"
#define DHTPIN A3 
#define DHTTYPE DHT11
DHT dht(DHTPIN, DHTTYPE);

/* 
 This is the code for a bike-computer (odometer). It was developed by Clovis Fritzen in
 02/10/2015. It uses the ´Debounce´ Arduino sketch for reading pulses from the bycicle wheel;
 I then do some math for calculating both distance and speed (kilometers per hour and 
 meters per second). The last step is to throw all this data in a LCD display, using the 
 16x2 ´hello world´ Arduino sketch. 
 
 */

// Bycicle wheel perimeter calculation:
        // For a 26´ (inches) wheel, the actual measured radius was 340mm
        // and Perimeter= 2*pi*radius , so Perimeter= 2* 3.14159265* 0.34m
        // Perimeter= 2.136283 meters 
        // Speed (m/s)= distante (meters)/ time (seconds)
        // Speed (km/h) = 1000/3600= 3.6* m/s         


unsigned long timet;
unsigned long tempoativo1inicio;
unsigned long tempoativo1final;
unsigned long tempototal1;
unsigned long previousTime;
unsigned long previousTime2;
boolean enterFunction = true;
boolean enterFunction2 = true;
        
// constants won't change. They're used here to 
// set pin numbers:
const int buttonPin = 7;    // the number of the pushbutton pin
const int ledPin = 13;      // the number of the LED pin

float lasttime = 0;
float speedms = 0;
float speedkmh = 0;
float holdtime = 0;
float t = 0;
float perimetroRoda = 2.136283; // meters
float distanciaTotal = 0.00; // distancia percorrida em metros
unsigned long tempoAtual;
unsigned long tempoAnterior;

// the following variables are long's because the time, measured in miliseconds,
// will quickly become a bigger number than can be stored in an int.
long lastDebounceTime = 0;  // the last time the output pin was toggled
long debounceDelay = 30 ;    // the debounce time; increase if the output flickers
int estadoAnterior = LOW;

void setup() {
  dht.begin();
  lcd.begin(16, 2);
  lcd.print("Start Pedaling");
  //Serial.begin(115200);
  pinMode(buttonPin, INPUT);
  pinMode(ledPin, OUTPUT);

  // set initial LED state
  //digitalWrite(ledPin, ledState);
}

void loop() {
  
   timet = micros();
   estadoAnterior = LOW;
   lasttime= millis();
   int reading = digitalRead(buttonPin);
   
  if ((enterFunction == true) && (reading = HIGH)){
    previousTime= timet;
    //Serial.println(previousTime); // for debugging

    // Write your first code below 
    //-----------------------
    

  // read the state of the switch into a local variable:
  


    estadoAnterior = HIGH;
    tempoAtual = millis() - tempoAnterior; 
    tempoAnterior = tempoAtual;
    distanciaTotal = distanciaTotal + perimetroRoda;
    speedms = perimetroRoda / tempoAtual;
    speedkmh = 3.6 * speedms;
        
       
        
        
        

//-----------------------------------------------------------------------------------
// if I stop the bike for more than 5 seconds, both m/s and km/h will go to 0.00
  
  
  /*if (millis() - lasttime > 5000 && speedms > 0) {
     speedms = 0.00;
     speedkmh = 0.00;
      
     lcd.clear(); 
     lcd.setCursor(0, 0);
        // print the number of seconds since reset:
        lcd.print(speedms,DEC);
        lcd.setCursor(4, 0);        
        lcd.print(" m/s        ");
        
        lcd.setCursor(0, 1);
        // print the number of seconds since reset:
        lcd.print(speedkmh,DEC);
        lcd.setCursor(4, 1);
        lcd.print(" km/h  ");
        lcd.setCursor(10, 1);
        lcd.print(distanciaTotal);
        lcd.setCursor(15, 1);
        lcd.print("m     ");
        lcd.setCursor(11, 0);
        lcd.print(t,0);
        lcd.setCursor(14, 0);
        lcd.print("oC");*/
        
  
  }

   //-----------------------
    // End of your first code
    

 //------------------------
  // Your second code (different timing) starts here

  if (enterFunction2 == true){
    previousTime2= timet;
    //Serial.println(previousTime2); // for debugging

    // Write your second code below 
    //-----------------------
    
        // set the cursor to column 0, line 1
        // (note: line 1 is the second row, since counting begins with 0):
        lcd.clear();
        lcd.setCursor(0, 0);
        // print the number of seconds since reset:
        lcd.print(speedms);
        lcd.setCursor(4, 0);
        lcd.print(" m/s        ");
        lcd.setCursor(11, 0);
        lcd.print(t,0);
        lcd.setCursor(14, 0);
        lcd.print("oC");
        
        lcd.setCursor(0, 1);
        // print the number of seconds since reset:
        lcd.print(speedkmh);
        lcd.setCursor(4, 1);
        lcd.print(" km/h  ");
        lcd.setCursor(10, 1);
        lcd.print(distanciaTotal);
        lcd.setCursor(15, 1);
        lcd.print("m     ");
        
    //-----------------------
    // End of your second code    
    }   


//------------------------

if (timet - previousTime < 49999){ // 1 million microsencods= 1 second delay
    /* I have actually used 0.999990 seconds, in a trial to compensate the time that
       this IF function takes to be executed. this is really a point that
       need improvement in my code */   
    enterFunction= false;
  } 
  else {
    enterFunction= true;
  }
if (timet - previousTime2 < 499999){ // 1 million microsencods= 1 second delay
    /* I have actually used 0.999990 seconds, in a trial to compensate the time that
       this IF function takes to be executed. this is really a point that
       need improvement in my code */   
    enterFunction2= false;
  } 
  else {
    enterFunction2= true;
  }
    
}
  
