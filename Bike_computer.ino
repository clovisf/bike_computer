#include <LiquidCrystal.h>
LiquidCrystal lcd(12, 11, 5, 4, 3, 2);

/* 
 This is the code for a bike-computer (odometer). It was developed by Clovis Fritzen in
 02/10/2015. It uses the ´Debounce´ Arduino sketch for reading pulses from the bycicle wheel;
 I then do some math for calculating both distance and speed (kilometers per hour and 
 meters per second). The last step is to throw all this data in a LCD display, using the 
 16x2 ´hello world´ Arduino sketch. 
 
 */

// constants won't change. They're used here to 
// set pin numbers:
const int buttonPin = 7;    // the number of the pushbutton pin
const int ledPin = 13;      // the number of the LED pin

// Variables will change:
int ledState = HIGH;         // the current state of the output pin
int buttonState;             // the current reading from the input pin
int lastButtonState = LOW;   // the previous reading from the input pin
float actualtime = 0;
float lasttime = 0;
float speedms = 0;
float speedkmh = 0;
float distancemeters = 0;
float holdtime = 0;
boolean hasstarted = false;

// the following variables are long's because the time, measured in miliseconds,
// will quickly become a bigger number than can be stored in an int.
long lastDebounceTime = 0;  // the last time the output pin was toggled
long debounceDelay = 25 ;    // the debounce time; increase if the output flickers

void setup() {
  lcd.begin(16, 2);
  Serial.begin(115200);
  pinMode(buttonPin, INPUT);
  pinMode(ledPin, OUTPUT);

  // set initial LED state
  digitalWrite(ledPin, ledState);
}

void loop() {
  // read the state of the switch into a local variable:
  int reading = digitalRead(buttonPin);
  
  // if I stop the bike for more than 5 seconds, both m/s and km/h will go to 0.00
  holdtime = millis();
  if (holdtime - lasttime > 5000 && speedms > 0) {
     speedms = 0.00;
     speedkmh = 0.00;
     
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
        lcd.print(distancemeters);
        lcd.setCursor(15, 1);
        lcd.print("m     ");
     
  }

  // check to see if you just pressed the button 
  // (i.e. the input went from LOW to HIGH),  and you've waited 
  // long enough since the last press to ignore any noise:  

  // If the switch changed, due to noise or pressing:
  if (reading != lastButtonState) {
    // reset the debouncing timer
    lastDebounceTime = millis();
    } 
  
  if ((millis() - lastDebounceTime) > debounceDelay) {
    // whatever the reading is at, it's been there for longer
    // than the debounce delay, so take it as the actual current state:
    
    // if the button state has changed:
    if (reading != buttonState) {
      buttonState = reading;

      // only toggle the LED if the new button state is HIGH
      if (buttonState == HIGH) {
        ledState = !ledState;
        actualtime = millis()-lasttime;
        
        
        // Bycicle wheel perimeter calculation:
        // For a 26´ (inches) wheel, the actual measured radius was 340mm
        // and Perimeter= 2*pi*radius , so Perimeter= 2* 3.14159265* 0.34m
        // Perimeter= 2.136283 meters 
        // Speed (m/s)= distante (meters)/ time (seconds)
        // Speed (km/h) = 1000/3600= 3.6* m/s         
        speedms = 2.136283/(actualtime/1000); 
        speedkmh = 3.6* speedms;
        
        if (hasstarted == true){
        distancemeters = distancemeters + 2.136283;
        }
        hasstarted = true;
        
        // end of speed calculations
        /*
        Serial.print(speedms,4);
        Serial.println(" m/s");
        Serial.print(speedkmh,4);
        Serial.println(" km/h");
        */
        
        // set the cursor to column 0, line 1
        // (note: line 1 is the second row, since counting begins with 0):
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
        lcd.print(distancemeters);
        lcd.setCursor(15, 1);
        lcd.print("m     ");
        
        lasttime= millis();
      } 
      
    }
  }
  
  // set the LED:
  digitalWrite(ledPin, ledState);

  // save the reading.  Next time through the loop,
  // it'll be the lastButtonState:
     lastButtonState = reading;
}

