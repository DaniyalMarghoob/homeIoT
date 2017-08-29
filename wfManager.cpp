#include "includes.h"

int           _minimumQuality         = -1;
void handleRoot(AsyncWebServerRequest *request) 
{
  //DEBUG_WM(F("Handle root"));

  String page = FPSTR(_HTTP_HEAD);
  page.replace("{v}", "Options");
  page += FPSTR(HTTP_SCRIPT);
  page += FPSTR(HTTP_STYLE);
  //page += _customHeadElement;
  page += FPSTR(HTTP_HEAD_END);
  page += "<h1>";
  page += "WiFi Config";
  page += "</h1>";
  page += F("<h3>WiFiManager</h3>");
  page += FPSTR(HTTP_PORTAL_OPTIONS);
  page += FPSTR(HTTP_END);

  request->send(200, "text/html", page);

}

/** Wifi config page handler */
void handleWifi(AsyncWebServerRequest *request) 
{

  String page = FPSTR(_HTTP_HEAD);
  page.replace("{v}", "Config ESP");
  page += FPSTR(HTTP_SCRIPT);
  page += FPSTR(HTTP_STYLE);
 // page += _customHeadElement;
  page += FPSTR(HTTP_HEAD_END);


    int n = WiFi.scanComplete();
    if(n == -2)
    {
      WiFi.scanNetworks(true);
    }
    else if(n) 
    //DEBUG_WM(F("Scan done"));
    {

      //sort networks
      int indices[n];
      for (int i = 0; i < n; i++) {
        indices[i] = i;
      }

      // RSSI SORT

      // old sort
      for (int i = 0; i < n; i++) {
        for (int j = i + 1; j < n; j++) {
          if (WiFi.RSSI(indices[j]) > WiFi.RSSI(indices[i])) {
            std::swap(indices[i], indices[j]);
          }
        }
      }

      /*std::sort(indices, indices + n, [](const int & a, const int & b) -> bool
        {
        return WiFi.RSSI(a) > WiFi.RSSI(b);
        });*/

      // remove duplicates ( must be RSSI sorted )
      if (false) {
        String cssid;
        for (int i = 0; i < n; i++) {
          if (indices[i] == -1) continue;
          cssid = WiFi.SSID(indices[i]);
          for (int j = i + 1; j < n; j++) {
            if (cssid == WiFi.SSID(indices[j])) {
              //DEBUG_WM("DUP AP: " + WiFi.SSID(indices[j]));
              indices[j] = -1; // set dup aps to index -1
            }
          }
        }
      }

      //display networks in page
      for (int i = 0; i < n; i++) {
        if (indices[i] == -1) continue; // skip dups
        //DEBUG_WM(WiFi.SSID(indices[i]));
        //DEBUG_WM(WiFi.RSSI(indices[i]));
        int quality = getRSSIasQuality(WiFi.RSSI(indices[i]));

        if (_minimumQuality == -1 || _minimumQuality < quality) {
          String item = FPSTR(HTTP_ITEM);
          String rssiQ;
          rssiQ += quality;
          item.replace("{v}", WiFi.SSID(indices[i]));
          item.replace("{r}", rssiQ);
          if (WiFi.encryptionType(indices[i]) != ENC_TYPE_NONE) {
            item.replace("{i}", "l");
          } else {
            item.replace("{i}", "");
          }
          ////DEBUG_WM(item);
          page += item;
          delay(0);
        } else {
          //DEBUG_WM(F("Skipping due to quality"));
        }

      }
      page += "<br/>";
    }
  

  page += FPSTR(HTTP_FORM_START);
  char parLength[2];


  page += FPSTR(HTTP_FORM_END);
  page += FPSTR(HTTP_SCAN_LINK);

  page += FPSTR(HTTP_END);

  request->send(200, "text/html", page);


  //DEBUG_WM(F("Sent config page"));
}
void handleWifi0(AsyncWebServerRequest *request)
{
  String page = FPSTR(_HTTP_HEAD);
  page.replace("{v}", "Config ESP");
  page += FPSTR(HTTP_SCRIPT);
  page += FPSTR(HTTP_STYLE);
  //page += _customHeadElement;
  page += FPSTR(HTTP_HEAD_END);
  page += FPSTR(HTTP_FORM_START);
  char parLength[2];




  page += FPSTR(HTTP_FORM_END);
  page += FPSTR(HTTP_SCAN_LINK);

  page += FPSTR(HTTP_END);

  request->send(200, "text/html", page);


  //DEBUG_WM(F("Sent config page"));
}
/** Handle the WLAN save form and redirect to WLAN config page again */
void handleWifiSave(AsyncWebServerRequest *request) 
{
  //DEBUG_WM(F("WiFi save"));
  String _ssid = "";
  String _pass = "";
  //SAVE/connect here
  if(request->hasArg("s"))
    _ssid = request->arg("s");
  if(request->hasArg("p"))
    _pass = request->arg("p");

  String page = FPSTR(_HTTP_HEAD);
  page.replace("{v}", "Credentials Saved WiFi Will be restarted");
  page += FPSTR(HTTP_SCRIPT);
  page += FPSTR(HTTP_STYLE);
 // page += _customHeadElement;
  page += FPSTR(HTTP_HEAD_END);
  page += FPSTR(HTTP_SAVED);
  page += FPSTR(HTTP_END);

  request->send(200, "text/html", page);

  //DEBUG_WM(F("Sent wifi save page"));
  //DEBUG_WM(_ssid);
  //DEBUG_WM(_pass);
  WiFi.begin(_ssid.c_str(), _pass.c_str());
  if (WiFi.status() != WL_CONNECTED) {
    //DEBUG_WM(F("Failed to connect."));
  } 
  else 
  {
    //DEBUG_WM(F("Connected.."));
    WiFi.mode(WIFI_STA);
  }
  delay(5000);
  ESP.reset();
}

/** Handle the info page */
void handleInfo(AsyncWebServerRequest *request) 
{
  //DEBUG_WM(F("Info"));
  String wfModes[15] = { "WiFi AP", "WiFi STA", "WiFi AP_STA","WiFi OFF" };
  String page = FPSTR(_HTTP_HEAD);
  page.replace("{v}", "Info");
  page += FPSTR(HTTP_SCRIPT);
  page += FPSTR(HTTP_STYLE);
 // page += _customHeadElement;
  page += FPSTR(HTTP_HEAD_END);
  page += F("<dl>");
  page += F("<dt>Switches States</dt><dd>");
  page += states_response;
  page += F("<dt>WiFi Mode</dt><dd>");
  page += wfModes[WiFi.getMode()];
  page += F("</dd>");
  page += F("<dt>Station IP</dt><dd>");
  page += WiFi.localIP().toString();
  page += F("</dd>");
  page += F("<dt>Connected To</dt><dd>");
  page += WiFi.SSID();
  page += F("</dd>");
  page += F("<dt>Chip ID</dt><dd>");
  page += ESP.getChipId();
  page += F("</dd>");
  page += F("<dt>Flash Chip ID</dt><dd>");
  page += ESP.getFlashChipId();
  page += F("</dd>");
  page += F("<dt>IDE Flash Size</dt><dd>");
  page += ESP.getFlashChipSize();
  page += F(" bytes</dd>");
  page += F("<dt>Real Flash Size</dt><dd>");
  page += ESP.getFlashChipRealSize();
  page += F(" bytes</dd>");
  page += F("<dt>Soft AP IP</dt><dd>");
  page += WiFi.softAPIP().toString();
  page += F("</dd>");
  page += F("<dt>Soft AP MAC</dt><dd>");
  page += WiFi.softAPmacAddress();
  page += F("</dd>");
  page += F("<dt>Station MAC</dt><dd>");
  page += WiFi.macAddress();
  page += F("</dd>");
  page += F("</dl>");
  
  page += FPSTR(HTTP_END);

  request->send(200, "text/html", page);

  //DEBUG_WM(F("Sent info page"));
}

/** Handle the reset page */
void handleReset(AsyncWebServerRequest *request) 
{
  //DEBUG_WM(F("Reset"));

  String page = FPSTR(_HTTP_HEAD);
  page.replace("{v}", "Info");
  page += FPSTR(HTTP_SCRIPT);
  page += FPSTR(HTTP_STYLE);
 // page += _customHeadElement;
  page += FPSTR(HTTP_HEAD_END);
  page += F("Module will reset in a few seconds.");
  page += FPSTR(HTTP_END);
  request->send(200, "text/html", page);

  //DEBUG_WM(F("Sent reset page"));
  delay(5000);
  ESP.reset();
  delay(2000);
}



//removed as mentioned here https://github.com/tzapu/WiFiManager/issues/114
// void WiFiManager::handle204() {
//   //DEBUG_WM(F("204 No Response"));
//   server->sendHeader("Cache-Control", "no-cache, no-store, must-revalidate");
//   server->sendHeader("Pragma", "no-cache");
//   server->sendHeader("Expires", "-1");
//   server->send ( 204, "text/plain", "");
// }

void handleNotFound(AsyncWebServerRequest *request) 
{
  
  
  request->send ( 404, "text/plain", "Not Found" );
}

template <typename Generic>
void DEBUG_WM(Generic text) {
  // if (_debug) {
    Serial.print("*WM: ");
    Serial.println(text);
  }
// }
int getRSSIasQuality(int RSSI) 
{
  int quality = 0;

  if (RSSI <= -100) {
    quality = 0;
  } else if (RSSI >= -50) {
    quality = 100;
  } else {
    quality = 2 * (RSSI + 100);
  }
  return quality;
}

void WiFiEvent(WiFiEvent_t event) 
{
    // String test(event_table[event]);
    // Serial.println(test);
    // switch(event) {
    //     case WIFI_EVENT_STAMODE_GOT_IP:
    //         okToProceed = 1;  
    //         Serial.println("WiFi connected");
    //         Serial.println("IP address: ");
    //         Serial.println(WiFi.localIP());
    //     break;
    //     case WIFI_EVENT_STAMODE_DISCONNECTED:
    //         Serial.print("WiFi lost connection ");
    //         switch(WiFi.status()){
    //             case WL_NO_SSID_AVAIL:
    //                 Serial.println("SSID Not Found");
    //                 if(WiFi.SSID() && millis() > 40000)
    //                 {
    //                   if(WiFi.getMode() != WIFI_AP_STA)
    //                   {
    //                     WiFi.mode(WIFI_AP_STA);
    //                     WiFi.softAP("plutoLabs", "supsupyo");
    //                   }
    //                 }
    //                 else
    //                 {
    //                   Serial.println("Waiting For Timeout!");
    //                 }
    //             break;
    //             case WL_CONNECT_FAILED:
    //                 Serial.println("Incorrect PassWord Starting AP");
    //                 if(millis() > 20000)
    //                 {
    //                   if(WiFi.getMode() != WIFI_AP_STA)
    //                   {
    //                     WiFi.mode(WIFI_AP_STA);
    //                     WiFi.softAP("plutoLabs", "supsupyo");
    //                   }
    //                 }
                    
    //             break;
    //             case WL_IDLE_STATUS:
    //                 Serial.println("IDLE State");
    //             break;

    //         }
    //     break;
    //     case WIFI_EVENT_SOFTAPMODE_STACONNECTED:
            
    //         Serial.println("WiFi AP Server Started");
    //     break;

    // }
}
void startServer()
{
  server.on("/",HTTP_GET, handleRoot).setFilter(ON_AP_FILTER);
  server.on("/wifi",HTTP_GET,handleWifi).setFilter(ON_AP_FILTER);
  server.on("/0wifi",HTTP_GET,handleWifi0).setFilter(ON_AP_FILTER);
  server.on("/wifisave",HTTP_GET,handleWifiSave).setFilter(ON_AP_FILTER);
  server.on("/i", HTTP_GET,handleInfo).setFilter(ON_AP_FILTER);
  server.on("/r", HTTP_GET,handleReset).setFilter(ON_AP_FILTER);
  server.on("/fwlink",HTTP_GET,handleRoot).setFilter(ON_AP_FILTER);  //Microsoft captive portal. Maybe not needed. Might be handled by notFound handler.
  server.onNotFound (handleNotFound);
  server.on("/", HTTP_GET,handleInfo).setFilter(ON_STA_FILTER);
  server.on("/command", HTTP_POST,HandleCommandRequestPOST);
  server.on("/command", HTTP_GET,HandleCommandRequestGET);
  server.on("/getstates", HTTP_GET,HandleStateRequest).setFilter(ON_STA_FILTER);
  server.on("/getstate", HTTP_GET,singleStateRequest).setFilter(ON_STA_FILTER);
  server.onNotFound(onRequest);
  server.begin(); // Web server start
  if(WiFi.getMode() == WIFI_STA)
    Serial.printf("Station Server Started at: %s",WiFi.localIP().toString().c_str());
  else if(WiFi.getMode() == WIFI_AP)
    Serial.printf("AP Server Started at: %s",WiFi.softAPIP().toString().c_str());
  else if(WiFi.getMode() == WIFI_AP_STA)
    Serial.printf("STA_AP Server Started at: %s and %s is something wrong?",WiFi.softAPIP().toString().c_str(),WiFi.localIP().toString().c_str());
  else
    Serial.println("Something is wrong bro you should never be here:("); 
}