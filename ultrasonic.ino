/*
Reverse Gear Ultrasonic Module With Date/Time 
Arthur: Mehrdad Safari
*/


// include the library code:
#include <LiquidCrystal.h>


// Date and time functions using a DS1307 RTC connected via I2C and Wire lib
#include <Wire.h>
#include "RTClib.h"
#include "DateConvL.h"
RTC_DS1307 rtc;
uint16_t current_year, current_month, current_day;
DateConvL dateC;

char daysOfTheWeek[7][12] = {"Sunday", "Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday"};



// initialize the library with the numbers of the interface pins
LiquidCrystal lcd(8, 9, 4, 5, 6, 7);


const int buttonPin = 2;     // the number of the pushbutton pin
const int trigPin = 12;      // Arduino pin tied to trigger pin on the ultrasonic sensor.
const int echoPin = 13;      // Arduino pin tied to echo pin on the ultrasonic sensor.
#define MAX_DISTANCE 300     // Maximum distance we want to ping for (in centimeters). 


// defines variables
long duration;
int distance;
int Buzzer = 11;
int val=0;

// Creating Presin Charaters for Bar Graph and Reverse Mode Icon
byte Level0[8] = {
    0b00000,
    0b00000,
    0b00000,
    0b00000,
    0b00000,
    0b00000,
    0b11111,
    0b11111
};
byte Level1[8] = {
    0b00000,
    0b00000,
    0b00000,
    0b00000,
    0b11111,
    0b11111,
    0b11111,
    0b11111
};
byte Level2[8] = {
    0b00000,
    0b00000,
    0b11111,
    0b11111,
    0b11111,
    0b11111,
    0b11111,
    0b11111
};
byte Level3[8] = {
    0b11111,
    0b11111,
    0b11111,
    0b11111,
    0b11111,
    0b11111,
    0b11111,
    0b11111
};
byte NoLevel[8] = {
    0b00000,
    0b00000,
    0b00000,
    0b00000,
    0b00000,
    0b00000,
    0b00000,
    0b00000
};
byte R[8] = {
    0b11111,
    0b10000,
    0b10111,
    0b10101,
    0b10110,
    0b10101,
    0b10000,
    0b11111
};
byte B[8] = {
 
    0b11000,
    0b01000,
    0b01000,
    0b01000,
    0b01000,
    0b01000,
    0b01000,
    0b11000
};


void setup() {
   pinMode(trigPin, OUTPUT);   // Sets the trigPin as an Output
   pinMode(echoPin, INPUT);    // Sets the echoPin as an Input

// initialize the pushbutton pin as an input:
   pinMode(buttonPin, INPUT_PULLUP);
  
// set up the LCD's number of columns and rows:
  lcd.begin(16, 2);

// Print RTC Time message to the LCD.
  lcd.setCursor(0,0);
    if (! rtc.begin()) {
      lcd.print("Couldn't find RTC");
    while (1);
  }
  
  if (! rtc.isrunning()) { 
    lcd.print("RTC is NOT running!");
  }

// Creating Charaters for Bar Graph and Reverse Mode Icon
  lcd.createChar(0, Level0);
  lcd.createChar(1, Level1);
  lcd.createChar(2, Level2);
  lcd.createChar(3, Level3);
  lcd.createChar(4, NoLevel);
  lcd.createChar(5, R);
  lcd.createChar(6, B);
 
}

void loop() {

// Read the value of the input. It can either be 1 or 0
   int buttonValue = digitalRead(buttonPin);
// If button value is Low show Reverse Mode else show Time and Date
   if (buttonValue == LOW){
      distanceVal();
   } else {
       dateTime(); 
   }
   
}




void dateTime(){


//Set Cursor in right position
  lcd.setCursor(0, 0);
  lcd.print("                ");
  lcd.setCursor(0,1);
  lcd.print("                ");
  
//Set RTC Time 
    DateTime now = rtc.now();
    char hh;
    if (now.hour()==0){
      hh=12;
    }else{
      hh=now.hour();
    }

    lcd.setCursor(3,0);
// Convert Date to Shamsi/Jalali 
    dateC.ToShamsi(now.year(),now.month(),now.day()); // converts global values of date and stores them to dateC
//Print Date on LCD
    lcd.print(dateC.global_year, DEC);
    lcd.print('/');
    lcd.print(dateC.global_month, DEC);
    lcd.print('/');
    lcd.print(dateC.global_day, DEC);
  
        
// Print Time on LCD
    lcd.setCursor(3,1);
    lcd.print(hh, DEC);
    lcd.print(':');
    lcd.print(now.minute(), DEC);
    lcd.print(':');
    lcd.print(now.second(), DEC);
    delay(1000);

}


void distanceVal(){

// Clears the trigPin
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);

// Sets the trigPin on HIGH state for 10 micro seconds
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);

// Reads the echoPin, returns the sound wave travel time in microseconds
  duration = pulseIn(echoPin, HIGH);

// Calculating the distance
  val= duration*0.034/2;


    delay(50); // Wait 50ms between pings (about 20 pings/sec). 29ms should be the shortest delay between pings.
   
// Print Distance value on LCD  
    lcd.setCursor(0, 0);
    lcd.print("                ");
    lcd.setCursor(1,1);
    lcd.print("                ");
    lcd.setCursor(0, 0);
    lcd.print("Distance: ");
  
    lcd.print(val);
    lcd.print("cm    ");
    lcd.setCursor(8, 1);
    lcd.write(byte(5));
    lcd.setCursor(9, 1);
    lcd.write(byte(6));
    lcd.setCursor(10, 1);

 
 // Tone BUZZER and update LCD data if Distance is between 1 and 10 CM
  if(val<=10&&val>0)
  {

    lcd.setCursor(0,0);
    tone(Buzzer, 440); 
    lcd.print("******Stop******");
    lcd.setCursor(0,1);
    lcd.print("****************");
 
   
  }

  else if(val<=25&&val>10)
  {
    tone(Buzzer, 440 ,100); 
    delay(200);
    tone(Buzzer, 330 ,100); 
  
    lcd.setCursor(0, 1); // bottom left
    lcd.write(byte(0));
    lcd.setCursor(1, 1); // bottom left
    lcd.write(byte(0));
    lcd.setCursor(2, 1); // bottom left
    lcd.write(byte(1));
    lcd.setCursor(3, 1); // bottom left
    lcd.write(byte(1));
    lcd.setCursor(4, 1);
    lcd.write(byte(2));
    lcd.setCursor(5, 1);
    lcd.write(byte(2));
    lcd.setCursor(6, 1);
    lcd.write(byte(3));
    lcd.setCursor(7, 1);
    lcd.write(byte(3));
 
  }

  else if(val<=50&&val>25)

  {
 
    tone(Buzzer, 440 ,500); 
    delay(1000);
    tone(Buzzer, 330 ,500); 
    
    lcd.setCursor(0, 1); // bottom left
    lcd.write(byte(0));
    lcd.setCursor(1, 1); // bottom left
    lcd.write(byte(0));
    lcd.setCursor(2, 1); // bottom left
    lcd.write(byte(1));
    lcd.setCursor(3, 1); // bottom left
    lcd.write(byte(1));
    lcd.setCursor(4, 1);
    lcd.write(byte(2));
    lcd.setCursor(5, 1);
    lcd.write(byte(2));
    lcd.setCursor(6, 1);
    lcd.write(byte(4));
    lcd.setCursor(7, 1);
    lcd.write(byte(4));


  }
    else if(val<=75&&val>50)
  {
 
    tone(Buzzer, 440 ,1000); 
    delay(2000);
    tone(Buzzer, 330 ,1000); 
    
    lcd.setCursor(0, 1); // bottom left
    lcd.write(byte(0));
    lcd.setCursor(1, 1); // bottom left
    lcd.write(byte(0));
    lcd.setCursor(2, 1); // bottom left
    lcd.write(byte(1));
    lcd.setCursor(3, 1); // bottom left
    lcd.write(byte(1));
    lcd.setCursor(4, 1);
    lcd.write(byte(4));
    lcd.setCursor(5, 1);
    lcd.write(byte(4));
    lcd.setCursor(6, 1);
    lcd.write(byte(4));
    lcd.setCursor(7, 1);
    lcd.write(byte(4));



  }
    else if(val<=100&&val>75)
  {
 
    tone(Buzzer, 440 ,1200); 
    delay(2400);
    tone(Buzzer, 330 ,1200); 
    
    lcd.setCursor(0, 1); // bottom leftlcd.setCursor(0, 1); // bottom left
    lcd.write(byte(0));
    lcd.write(byte(0));
    lcd.setCursor(1, 1); // bottom leftlcd.setCursor(1, 1); // bottom left
    lcd.write(byte(0));
    lcd.write(byte(0));
    lcd.setCursor(2, 1); // bottom left
    lcd.write(byte(4));
    lcd.setCursor(3, 1); // bottom left
    lcd.write(byte(4));
    lcd.setCursor(4, 1);
    lcd.write(byte(4));
    lcd.setCursor(5, 1);
    lcd.write(byte(4));
    lcd.setCursor(6, 1);
    lcd.write(byte(4));
    lcd.setCursor(7, 1);
    lcd.write(byte(4));
 
 
 
  }
 
      else if(val<MAX_DISTANCE)
  {
    lcd.setCursor(0, 1); // bottom leftlcd.setCursor(0, 1); // bottom left
    lcd.write(byte(4));
    lcd.write(byte(0));
    lcd.setCursor(1, 1); // bottom leftlcd.setCursor(1, 1); // bottom left
    lcd.write(byte(4));
    lcd.write(byte(0));
    lcd.setCursor(2, 1);
    lcd.write(byte(4));
    lcd.setCursor(3, 1);
    lcd.write(byte(4));
    lcd.setCursor(4, 1);
    lcd.write(byte(4));
    lcd.setCursor(5, 1);
    lcd.write(byte(4));
    lcd.setCursor(6, 1);
    lcd.write(byte(4));
    lcd.setCursor(7, 1);
    lcd.write(byte(4));

  
  }

}
