#include <Servo.h>// Library for Servomotor
#include <ThreeWire.h>//Library for RTC object
#include <RtcDS1302.h> //Library for Data and Time
#include <Wire.h> // Library for I2C communication
#include <LiquidCrystal_I2C.h> // Library for LCD
#include "DHT.h"
Servo myservo;
int pompaPin = 13;


#define DHTPIN 5
#define DHTTYPE DHT11
int servo_alim = 12;
int servo = 9;
int ridicare0 = 2;
int ridicare1 = 4;
int mancare1 = 11;
int mancare0 = 10;
int controlviteza = 3;
int nivelApa = A0;
int lumina = A1;
int butoane = A2;

LiquidCrystal_I2C lcd = LiquidCrystal_I2C(0x27, 16, 2);//Setting 16*2 screen
DHT dht(DHTPIN, DHTTYPE);
ThreeWire RtcWire(7, 6, 8);
RtcDS1302<ThreeWire> Rtc(RtcWire);

void setup() {
  dht.begin();
  lcd.init();
  lcd.backlight();
  Rtc.Begin();
  //RtcDateTime currentTime = RtcDateTime(__DATE__ , __TIME__);
  //Rtc.SetDateTime(currentTime);
  Serial.begin(9600);
  pinMode(lumina, INPUT);
  pinMode(nivelApa, INPUT);
  pinMode(butoane, INPUT);
  myservo.attach(servo);
  pinMode(servo_alim,OUTPUT);
  pinMode(pompaPin,OUTPUT);
}

void loop() {
  lcd.setCursor(0,0);
  //display data
  lcd.print("Temp Hum  Light");
  Serial.print(analogRead(lumina));
  Serial.print("\n");
  lcd.setCursor(0,1);
  lcd.print((float)dht.readTemperature());
  lcd.setCursor(6,1);
  lcd.print((int)dht.readHumidity());
  lcd.setCursor(10,1);
  if(analogRead(lumina) < 40){
    lcd.print("DARK");
  }
  if(analogRead(lumina) > 40 && analogRead(lumina) < 100){
    lcd.print("CLOUDY");
  }
  if(analogRead(lumina) > 100){
    lcd.print("SUNNY");
  }
  delay(500);
  lcd.clear();
  //water button press, put water
  if(analogRead(butoane) > 300 && analogRead(butoane) < 600){
    digitalWrite(servo_alim, HIGH);
    lcd.clear();
    lcd.setCursor(3,0);
    lcd.print("THIRSTY??");
    delay(3000);
    myservo.write(0);
    delay(1000);
    while(analogRead(nivelApa) < 300){
      digitalWrite(pompaPin, HIGH);
      if(analogRead(nivelApa) < 100){
        lcd.clear();
        lcd.setCursor(2,0);
        lcd.print("PUTTING WATER");
        delay(200);
      }
      if(analogRead(nivelApa) > 100){
        lcd.clear();
        lcd.setCursor(2,0);// set the cursor on the first row and column
        lcd.print("ALMOST DONE");
        delay(200);
      }
    }
    digitalWrite(pompaPin, LOW);
    if(analogRead(nivelApa) > 300){
        lcd.clear();
        lcd.setCursor(0,0);// set the cursor on the first row and column
        lcd.print("DRINK!! WATER");
        lcd.setCursor(0,1);//set the cursor on  the second row and first column
        lcd.print("IS FULL!!");
        delay(200);
      }
    delay(500);
    myservo.write(180);
    delay(3000);
    digitalWrite(servo_alim, LOW);
  }
  //food button press, give food or display current time
  if(analogRead(butoane) > 700){
      lcd.clear();
      lcd.setCursor(0,0);// set the cursor on the first row and column
      lcd.print("PRESS WATER");
      lcd.setCursor(0,1);//set the cursor on  the second row and first column
      lcd.print("FOR DATE");
      delay(1000);
      //delay(200);
      //hold button for current time
      if(analogRead(butoane) > 400){
        RtcDateTime now = Rtc.GetDateTime();
        lcd.clear();
        lcd.setCursor(0,0);// set the cursor on the first row and column
        lcd.print("Date: ");
        lcd.print(now.Day());
        lcd.print("/");
        lcd.print(now.Month());
        lcd.print("/");
        lcd.print(now.Year());
        lcd.setCursor(0, 1);
        lcd.print("Time: ");
        lcd.print(now.Hour());
        lcd.print(":");
        lcd.print(now.Minute());
        lcd.print(":");
        lcd.print(now.Second());
        delay(3000);
      }else{
        //give food
        lcd.clear();
        lcd.setCursor(0,0);// set the cursor on the first row and column
        lcd.print("FOOD");
        lcd.setCursor(0,1);//set the cursor on  the second row and first column
        lcd.print("READY?");
        //raise the bowl
        analogWrite(controlviteza, 255);
        digitalWrite(ridicare1,HIGH);
        digitalWrite(ridicare0, LOW);
        delay(8900);
        digitalWrite(ridicare0, LOW);
        digitalWrite(ridicare1, LOW);
        lcd.clear();
        lcd.setCursor(0,0);// set the cursor on the first row and column
        lcd.print("FOOD");
        lcd.setCursor(0,1);//set the cursor on  the second row and first column
        lcd.print("GO!!!");
        //shake food:))
        digitalWrite(mancare1, HIGH);
        digitalWrite(mancare0, LOW);
        delay(200);
        digitalWrite(mancare1, LOW);
        digitalWrite(mancare0, LOW);
        delay(100);
        digitalWrite(mancare1,LOW);
        digitalWrite(mancare0,HIGH);
        delay(200);
        digitalWrite(mancare1, LOW);
        digitalWrite(mancare0, LOW);
        delay(100);
        digitalWrite(mancare1, HIGH);
        digitalWrite(mancare0, LOW);
        delay(300);
        digitalWrite(mancare1, LOW);
        digitalWrite(mancare0, LOW);
        delay(100);
        digitalWrite(mancare1, LOW);
        digitalWrite(mancare0, HIGH);
        delay(300);
        digitalWrite(mancare1, LOW);
        digitalWrite(mancare0, LOW);
        delay(1000);
        digitalWrite(ridicare0, HIGH);
        digitalWrite(ridicare1, LOW);
        delay(6600);
        digitalWrite(ridicare0, LOW);
        digitalWrite(ridicare1, LOW);
      }
  }
}
