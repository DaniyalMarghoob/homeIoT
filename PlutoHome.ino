
#include "includes.h"


int failCase = 0;
uint32_t now;

void setup() {
  Serial.begin(38400);
  delay(500);
  Serial.println("\nApplication Started!");
 // WiFi.onEvent(WiFiEvent);
  WiFi.mode(WIFI_STA);
  WiFi.printDiag(Serial);
  now=millis();
  if(WiFi.SSID())
  {
    Serial.printf("\nFound Saved SSID %s\n",WiFi.SSID().c_str());
    WiFi.begin();
    delay(2000);
    while(WiFi.status() != WL_CONNECTED)
    {
      Serial.print("-");
      delay(250);
      Serial.print("_");
      delay(250);
      if(WiFi.status() == STATION_WRONG_PASSWORD)
      {
        Serial.printf("\nWrong Password for %s",WiFi.SSID().c_str());
        break;
      }
      else if(WiFi.status() == STATION_CONNECT_FAIL)
      {
        Serial.printf("\nConnect Failed To %s",WiFi.SSID().c_str());
        break;
      }
      if(millis()-now>40000)
      {
        Serial.println("\nTimeout !");
        break;
      }
    }
  }
  else
  {
      Serial.println("couldnt Found Saved SSID starting Config");
      WiFi.mode(WIFI_AP_STA);
      WiFi.softAP("plutoLabs"ROOM_ID, "supsupyo");
      delay(2000);
      startServer();
      while(!okToProceed)
      {
        delay(500);
        if(WiFi.status()==WL_CONNECTED)
        {
          Serial.println("Connected to AP");
          okToProceed = 1;
          WiFi.mode(WIFI_STA);
          break;
        }
      }
  }
  if(WiFi.status() != WL_CONNECTED)
  {
    Serial.println("Starting AP After Timeout!");
    WiFi.mode(WIFI_AP_STA);
    WiFi.softAP("plutoLabs"ROOM_ID, "123qweasdzxc");
    delay(2000);
    startServer();
    while(!okToProceed)
    {
      delay(500);
      if(WiFi.status()==WL_CONNECTED)
      {
        Serial.println("Connected to AP");
        okToProceed = 1;
        WiFi.mode(WIFI_STA);
        startServer();
        break;
      }
    }
  }
  else
  {
    WiFi.mode(WIFI_STA);
    startServer();
  }
  appInit();
  Serial.write(27);
  now = millis();
}

void loop() {
  ArduinoOTA.handle();
  //fauxmo.handle();
  checkSerial();
  if(millis() - now >= 1000)
  {
    now = millis();
    if(WiFi.status()!=WL_CONNECTED)
    {
      failCase++;
      #ifdef DEBUG_EN
         Serial.println("Disconnected!");
       #endif
      if(failCase == 10)
      {
        
        ESP.reset();
      }
    }
    else
    {
      failCase = 0;
    }
  }
}
