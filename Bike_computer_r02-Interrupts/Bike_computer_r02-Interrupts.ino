/* 
 This is the code for a bike-computer (odometer). It was developed by Clovis Fritzen in
 02/10/2015 (R01) and modified in 07/02/2017 to include code from https://github.com/lukkry/speedometer . 
 
 */

// Bycicle wheel perimeter calculation:
        // For a 26´ (inches) wheel, the actual measured radius was 340mm
        // and Perimeter= 2*pi*radius , so Perimeter= 2* 3.14159265* 0.34m
        // Perimeter= 2.136283 meters 
        // Speed (m/s)= distante (meters)/ time (seconds)
        // Speed (km/h) = 1000/3600= 3.6* m/s         


#include <LiquidCrystal.h>

// pin connected to reed switch
#define reed A0

int reedVal;
int maxDebounceTicks = 100;
int currentDebounceTicks;
int time = 0;
// the wheel radius, in kilometers
float radius = 0.00034;
float circumference;
float velocity = 0.00;
float distance = 0.00;

// initialize LCD with the numbers of the interface pins
LiquidCrystal lcd(12, 11, 5, 4, 3, 2);

void setup() {
  // set up the LCD's number of columns and rows
  lcd.begin(8, 2);

  Serial.begin(9600);

  currentDebounceTicks = maxDebounceTicks;

  circumference = 2 * 3.14 * radius;

  pinMode(reed, INPUT);

  cli();

  // set entire TCCR1A register to 0
  TCCR1A = 0;

  // set entire TCCR1B register to 0
  TCCR1B = 0;

  // initialize counter value to 0;
  TCNT1  = 0;

  // set timer count for 1khz increments
  // = (16*10^6) / (1000*8) - 1
  OCR1A = 1999;

  // turn on CTC mode
  TCCR1B |= (1 << WGM12);

  // Set CS11 bit for 8 prescaler
  TCCR1B |= (1 << CS11);

  // enable timer compare interrupt
  TIMSK1 |= (1 << OCIE1A);

  sei();
}

void loop() {
  displayText();
  delay(1000);
}

ISR(TIMER1_COMPA_vect) {
  reedVal = digitalRead(reed);
  if (reedVal) {
    // wait the given number of ticks, before calculating the velocity
    if (currentDebounceTicks == 0) {
      // circumference in kilometers, time in hours
      velocity = circumference/(float(time)/1000/3600);
      time = 0;
      currentDebounceTicks = maxDebounceTicks;
      distance = distance + circumference;

    } else {
      if (currentDebounceTicks > 0) {
        currentDebounceTicks -= 1;
      }
    }
  } else {
    if (currentDebounceTicks > 0) {
      currentDebounceTicks -= 1;
    }
  }

  if (time > 2000) {
    // set velocity to 0 when tire is still for 2 seconds
    velocity = 0;
  } else {
    time += 1;
  }
}

void displayText(){
  lcd.clear();

  lcd.setCursor(0, 0);
  lcd.print(int(velocity));
  lcd.print(" km/h");

  lcd.setCursor(0, 1);
  if (distance > 1) {
    lcd.print(distance);
    lcd.print(" km");
  } else {
    lcd.print(int(distance * 1000));
    lcd.print(" m");
  }

  Serial.println(velocity);
  Serial.println(distance);
}
