#include "Arduino_LED_Matrix.h"
#include "RTC.h"
#include "connectivity.h"
#include "network_credentials.h"
//Include the NTP library
#include <NTPClient.h>
#if defined(ARDUINO_PORTENTA_C33)
#include <WiFiC3.h>
#elif defined(ARDUINO_UNOWIFIR4)
#include <WiFiS3.h>
#endif

#include <WiFiUdp.h>

ArduinoLEDMatrix leds; // 8x12 led matrix object
int trig = 2; // pin number for trig of proximity sensor
int echo = 3; // pin number for echo of proximity sensor
int buzz = 11; // pin number to output to buzzer

WiFiUDP Udp; // A UDP instance to let us send and receive packets over UDP
NTPClient timeClient(Udp);

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  connectToWifi(); // connect to Wifi
  RTC.begin(); // important for timekeeping

  // begin the time client
  timeClient.begin();
  timeClient.update();

  // Get the current date and time from an NTP server and convert
  // it to local time. For me, that is UTC-6.
  auto unixTime = timeClient.getEpochTime() - (6* 3600) + (DST * 3600);
  RTCTime timeToSet = RTCTime(unixTime);
  RTC.setTime(timeToSet);
  
  leds.begin(); //initialize LED matrix
  Serial.println("Securty is now active."); // welcome message

  // io definitions
  pinMode(trig,OUTPUT); // trigger is output because u have to output a signal to the trigger to activate sensor
  pinMode(echo,INPUT); // echo is input cuz it gives u the duration of impulse
  pinMode(buzz,OUTPUT); // buzz is output cuz we make buzz sound
}

void loop() {
  // put your main code here, to run repeatedly:
  // TIMEKEEPING BEFORE ANYTHING
  RTCTime currTime; // create current time object
  RTC.getTime(currTime); // get the time and store it properly

  digitalWrite(trig,LOW); // deactivate trigger briefly before taking measurement
  
  // briefly make trigger high, activating the module
  digitalWrite(trig,HIGH);
  delayMicroseconds(20);
  digitalWrite(trig,LOW);

  /* The proximity sensor feeds us the duration of an impulse in microseconds as the sound wave bounces forward
  and backwards towards the object it hits. We can use this with an approximation of the speed of sound to calculate
  an approximate distance away from the sensor an object (or in this case, person) is.
  For our purposes, the log should only be made at a very close distance (<1m).*/
  long time = pulseIn(echo,HIGH) / 2;
  float distance = time * 0.000343; // 300 here is speed of light in m/us (microseconds), which is necessary as pulseIn gives us microseconds

  if (distance <= 1.2){
    tone(buzz,400); // emit a sound using the buzzer if close enough
    // this might just be the shittiest code i have ever written :)
    Serial.print("[");
    Serial.print(currTime.getYear());
    Serial.print("-");
    Serial.print(Month2int(currTime.getMonth()));
    Serial.print("-");
    Serial.print(currTime.getDayOfMonth());
    Serial.print(" ");
    Serial.print(currTime.getHour());
    Serial.print(":");
    Serial.print(currTime.getMinutes());
    Serial.print(":");
    Serial.print(currTime.getSeconds());
    Serial.print("] Movement detected!");
    Serial.print(" (");
    Serial.print(distance);
    Serial.println("m from sensor)");
    exclamationOn(&leds);
  }
  else{
    noTone(buzz);
    exclamationOff(&leds);
  }
  delay(300);
}

void exclamationOn(ArduinoLEDMatrix * matrix){
  byte exclamation[8][12] = {
    {0,0,0,0,0,0,0,0,0,0,0,0},
    {0,0,0,0,0,0,0,0,0,0,0,0},
    {1,1,1,1,1,1,1,0,0,1,1,1},
    {1,1,1,1,1,1,1,0,0,1,1,1},
    {1,1,1,1,1,1,1,0,0,1,1,1},
    {1,1,1,1,1,1,1,0,0,1,1,1},
    {0,0,0,0,0,0,0,0,0,0,0,0},
    {0,0,0,0,0,0,0,0,0,0,0,0}
  }; // exclamation pattern
  matrix->renderBitmap(exclamation,8,12); // put exclamation onto matrix
  return;
}

void exclamationOff(ArduinoLEDMatrix * matrix){
  byte safe[8][12] = {
    {0,0,0,0,0,0,0,0,0,0,0,0},
    {0,1,1,1,1,0,0,0,1,1,0,0},
    {0,1,1,1,1,0,0,0,0,1,1,0},
    {0,0,0,0,0,0,0,0,0,0,1,0},
    {0,0,0,0,0,0,0,0,0,0,1,0},
    {0,1,1,1,1,0,0,0,0,1,1,0},
    {0,1,1,1,1,0,0,0,1,1,0,0},
    {0,0,0,0,0,0,0,0,0,0,0,0}
  }; // blank
  matrix->renderBitmap(safe,8,12); // put blank onto matrix
  return;
}