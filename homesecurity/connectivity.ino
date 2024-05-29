//Include the NTP library
#include <NTPClient.h>
#if defined(ARDUINO_PORTENTA_C33)
#include <WiFiC3.h>
#elif defined(ARDUINO_UNOWIFIR4)
#include <WiFiS3.h>
#endif

#include <WiFiUdp.h>
#include "network_credentials.h"

char ssid[] = SSID;
char pass[] = PASS;

int wifiStatus = WL_IDLE_STATUS;

void connectToWifi(){
  int fails = 0;
  // checking to see if module can be communicated to
  while (WiFi.status() == WL_NO_MODULE && fails <= 5) {
    Serial.println("WiFi Error: Communication with WiFi module failed. Are you sure everything is connected up properly?");
    fails++;
  }
  if (fails >=5){
    Serial.println("WiFi errors occurred five times, stopping...");
    return;
  }

  // checking if firmware is up-to-date
  String fv = WiFi.firmwareVersion();
  if (fv < WIFI_FIRMWARE_LATEST_VERSION) {
    Serial.println("WiFi Warning: Firmware is out of date.");
  }

  // connecting to wifi network
  Serial.print("Attempting to connect to network ");
  Serial.print(SSID);
  Serial.println("...");
  wifiStatus = WiFi.begin(ssid,pass);
  delay(5000); // wait 5 sec to connect
  Serial.print("Successfully connected to network ");
  Serial.println(SSID);
}