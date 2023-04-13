// Necessary Headers
#include <dht.h>
#define Temp A2
#define echoPin 9
#define trigPin 10
#include <LiquidCrystal_I2C.h>
LiquidCrystal_I2C lcd(0x27, 16, 2);
#include "Wire.h"

// Variables to store different sensor inputs
dht DHT;
const int buzzerPin = A0;
const int flamePin = A3;
long Flame = 0;
int vibrationPin = A1;
int red = 11;
int green = 12;
int blue = 13;
long duration;
int distance;

// Setting up the initial states of sensors and other hardware present in the project

void setup()
{
  pinMode(buzzerPin, OUTPUT);
  pinMode(flamePin, INPUT);
  pinMode(vibrationPin, INPUT);
  lcd.init();
  lcd.clear();
  lcd.backlight();
  pinMode(red, OUTPUT);
  pinMode(blue, OUTPUT);
  pinMode(green, OUTPUT);
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);


  Serial.begin(9600);

  Serial.println("Vehicle Security System");
}

void loop()
{
  // reading and setting values
  Flame = digitalRead(flamePin); // flame sensor input
  DHT.read11(Temp); // temperature sensor input
  long vibrationValue = vibration(); // collision sensor input

  // setting up the lcd screen
  lcd.clear();
  lcd.setCursor(0, 1);
  lcd.print("Drive Safely!!!");

  // initial RGB light state, RGB light's normal state
  digitalWrite(red, 0);
  digitalWrite(green, 0);
  digitalWrite(blue, 0);

  // flame sensor part
  Serial.print("Flame:");
  Serial.println(Flame);

  if (Flame == 0) // 0, Fire detected
  {
    // Serial output message
    Serial.println("Fire!!!");
    
    // RGB light changed state
    digitalWrite(red, 255);
    digitalWrite(green, 0);
    digitalWrite(blue, 10);
    
    // LCD Alert Output
    lcd.clear();
    lcd.setCursor(0, 1);
    lcd.println("Vehicle on Fire!!");
    
    // Buzzer Alert
    tone(buzzerPin, 500);
  }
  else
  {
    noTone(buzzerPin); // turning off the buzzer
  }

  // Temperature Sensor Part
  // Serial output to display the detected value
  Serial.print("Temperature:");
  Serial.print(DHT.temperature);
  Serial.println(" C");

  if (DHT.temperature > 60) { // Critical Value Detected

    // LCD Alert message
    lcd.clear();
    lcd.setCursor(0, 1);
    lcd.println("Vehicle about to blow");
    
    // Serial output test message
    Serial.println("Fire has started to spread into the vehicle");
    Serial.print("Temperature: ");
    Serial.print(DHT.temperature);
    Serial.println(" C");

    // RGB alert changed state
    digitalWrite(red, 255);
    digitalWrite(green, 255);
    digitalWrite(blue, 255);
  }

  //Vibration sensor
  Serial.println(vibrationValue);
  
  if (vibrationValue > 60000) { // Critical Value detected
    // Serial Monitor output for test phase
    Serial.println("Collision Sensed!!");

    // LCD alert message
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Collision");
    lcd.setCursor(0, 1);
    lcd.print("Occurred!!");

    // Buzzer Alert
    tone(buzzerPin, 100);

    // RGB Alert    
    digitalWrite(red, 0);
    digitalWrite(green, 0);
    digitalWrite(blue, 255);
    delay(500);
  }
  else {
    noTone(buzzerPin); // Normal Buzzer State
  }


  //Ultrasonic sensor Part

  // Clears the trigPin condition
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  // Sets the trigPin HIGH (ACTIVE) for 10 microseconds
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);

  // Reads the echoPin, returns the sound wave travel time in microseconds
  duration = pulseIn(echoPin, HIGH);


  // calculating the distance
  distance = duration * 0.034 / 2;

  if (distance < 10) { // Critical proximity range

    // LCD Alert
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Mind the");
    lcd.setCursor(0, 1);
    lcd.print("distance!!");

    // Serial Monitor Output for test phase
    Serial.println("Very Very Close");

    // Buzzer Alert
    tone(buzzerPin, 100);

    // RGB alert, RGB changed state
    digitalWrite(red, 0);
    digitalWrite(green, 255);
    digitalWrite(blue, 0);
  }

  else {
    noTone(buzzerPin); // Buzzer normal state
  }
}

// function get sensor input from collision sensor
long vibration() {
  long m = pulseIn(vibrationPin, HIGH);
  return m;
}

// function to get sensor input from flame sensor
long flame() {
  long f = pulseIn(flamePin, HIGH);
  return f;
}
