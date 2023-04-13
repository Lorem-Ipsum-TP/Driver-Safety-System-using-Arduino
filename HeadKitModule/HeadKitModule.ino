// Required Libraries
#define IrSensor A0
#define buzzer 2
#include <SD.h> // need to include the SD library
#define SD_ChipSelectPin 10 //connect pin 4 of arduino to cs pin of sd card
#include <TMRpcm.h> //Arduino library for asynchronous playback of PCM/WAV files
#include <SPI.h> //  need to include the SPI library
#include <SoftwareSerial.h>

// Variables to store sensor input values
SoftwareSerial gsm(4, 5);
String msg;
char call;
int flag = 0;
int t1 = 0;
int t2 = 0;


TMRpcm tmrpcm; // create an object for use in this sketch
int ALCOHOL_sensor = A1;// MQ-3 SENSOR
int ALCOHOL_detected;

// Setting up the initial states of sensors and other hardware present in the project
void setup()
{
  // Setting up the SIM Module of alert messaging/calling 
  gsm.begin(9600);

  // Setting up the Serial Monitor, useful during test phase
  Serial.begin(9600);

  // Setting up ethanol sensor, IR Sensor, Buzzer
  pinMode(ALCOHOL_sensor, INPUT);
  pinMode(IrSensor, INPUT);
  pinMode(buzzer,OUTPUT);

  // Audio alert message output pin
  tmrpcm.speakerPin = 9;
  if (!SD.begin(SD_ChipSelectPin)) // returns 1 if the card is present
  {
    Serial.println("SD fail");
    return;
  }
  
  // Audio alert output volume
  tmrpcm.setVolume(5);
}

void loop()
{
  // Reading sensor input values
  ALCOHOL_detected = digitalRead(ALCOHOL_sensor);
  int val = analogRead(IrSensor);
  tmrpcm.setVolume(5);

  // alcohol sensor part

  // Serial monitor output of ethanol sensor for test phase
  //  int k = analogRead(A0);
  //  Serial.print("Analog");
  //  Serial.println(k);
  //  Serial.println(ALCOHOL_detected);
  
  if (ALCOHOL_detected == 1) // alcohol content detected
  {
    Serial.println("ALCOHOL detected...");
    SendMessage(2);
    delay(1000);
  }



  // IR Sensor Part
  if (digitalRead(IrSensor) == HIGH)
  {
    delay (4000);    
    // critical value indicating drowsiness drivers eyes has been shut for around 6 sec
    if (digitalRead(IrSensor) == HIGH) { 
      // Serial monitor alert for test phase
      Serial.println("Please be alert");

      // Buzzer Alert
      tone(buzzer, 500);
      delay(2000);
      noTone(buzzer);

      // Making call to driver to alert him via smartwatch vibration caused by the call and to concerned individuals
      MakeCall();

      // Sending alert message to the driver
      SendMessage(1);

      // Notifying the driver regarding his current drowsy state using audio message
      voiceMessage(1); 
    }
  }
  else{ // Normal state
    if (digitalRead(IrSensor) == LOW){
      noTone(buzzer);
    }
  }
}

// Function for playing audio messages
void voiceMessage (int val)
{
  // message type 1
  if (val == 1)
  {
    tmrpcm.play("alertMessage.wav");
  }
  // message type 2
  else if (val == 2)
  {
    tmrpcm.play("s2.wav");
  }
}

// function for message sending module
void SendMessage(int val)
{
  gsm.println("AT+CMGF=1");
  delay(1000);
  gsm.println("AT+CMGS=\"+917997059071\"\r");
  delay(1000);
  if (val == 1) {
    gsm.println("High Drowsiness Level Detected. Please be alert. Watch the steering and eyes on the road.");
  }
else if (val == 2) {
    gsm.println("Alcohol Content Detected in the driver's breath. Turning off the engines.....");
  }
  delay(100);
  gsm.println((char)26);
  delay(1000);
}

// message sending module 2 to concerned officials
void SendMessage2()
{
  gsm.println("AT+CMGF=1");
  delay(1000);
  gsm.println("AT+CMGS=\"+916374731507\"\r");
  delay(1000);
  gsm.println("Emergency, Driver Siddharth Das needs help!!!");
  delay(100);
  gsm.println((char)26);
  delay(1000);
}

// to make a call
void MakeCall()
{
  gsm.println("ATD+917997059071;");
  Serial.println("Calling  ");
  delay(1000);
}
