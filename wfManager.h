#ifndef WF_MANAGER_H_
#define WF_MANAGER_H_

#include <ESP8266WiFi.h>
#include <DNSServer.h>
#include "ESPAsyncWebServer.h"
#include <memory>
//#include "includes.h"

extern "C" {
  #include "user_interface.h"
}

const char _HTTP_HEAD[] PROGMEM            = "<!DOCTYPE html><html lang=\"en\"><head><meta name=\"viewport\" content=\"width=device-width, initial-scale=1, user-scalable=no\"/><title>{v}</title>";
const char HTTP_STYLE[] PROGMEM           = "<style>.c{text-align: center;} div,input{padding:5px;font-size:1em;} input{width:95%;} body{text-align: center;font-family:verdana;} button{border:0;border-radius:0.3rem;background-color:#1fa3ec;color:#fff;line-height:2.4rem;font-size:1.2rem;width:100%;} .q{float: right;width: 64px;text-align: right;} .l{background: url(\"data:image/png;base64,iVBORw0KGgoAAAANSUhEUgAAACAAAAAgCAMAAABEpIrGAAAALVBMVEX///8EBwfBwsLw8PAzNjaCg4NTVVUjJiZDRUUUFxdiZGSho6OSk5Pg4eFydHTCjaf3AAAAZElEQVQ4je2NSw7AIAhEBamKn97/uMXEGBvozkWb9C2Zx4xzWykBhFAeYp9gkLyZE0zIMno9n4g19hmdY39scwqVkOXaxph0ZCXQcqxSpgQpONa59wkRDOL93eAXvimwlbPbwwVAegLS1HGfZAAAAABJRU5ErkJggg==\") no-repeat left center;background-size: 1em;}</style>";
const char HTTP_SCRIPT[] PROGMEM          = "<script>function c(l){document.getElementById('s').value=l.innerText||l.textContent;document.getElementById('p').focus();}</script>";
const char HTTP_HEAD_END[] PROGMEM        = "</head><body><div style='text-align:left;display:inline-block;min-width:260px;'>";
const char HTTP_PORTAL_OPTIONS[] PROGMEM  = "<form action=\"/wifi\" method=\"get\"><button>Configure WiFi</button></form><br/><form action=\"/0wifi\" method=\"get\"><button>Configure WiFi (No Scan)</button></form><br/><form action=\"/i\" method=\"get\"><button>Info</button></form><br/><form action=\"/r\" method=\"post\"><button>Reset</button></form>";
const char HTTP_ITEM[] PROGMEM            = "<div><a href='#p' onclick='c(this)'>{v}</a>&nbsp;<span class='q {i}'>{r}%</span></div>";
const char HTTP_FORM_START[] PROGMEM      = "<form method='get' action='wifisave'><input id='s' name='s' length=32 placeholder='SSID'><br/><input id='p' name='p' length=64 type='password' placeholder='password'><br/>";
const char HTTP_FORM_PARAM[] PROGMEM      = "<br/><input id='{i}' name='{n}' length={l} placeholder='{p}' value='{v}' {c}>";
const char HTTP_FORM_END[] PROGMEM        = "<br/><button type='submit'>save</button></form>";
const char HTTP_SCAN_LINK[] PROGMEM       = "<br/><div class=\"c\"><a href=\"/wifi\">Scan</a></div>";
const char HTTP_SAVED[] PROGMEM           = "<div>Credentials Saved<br />Trying to connect ESP to network.<br />If it fails reconnect to AP to try again</div>";
const char HTTP_END[] PROGMEM             = "</div></body></html>";

const char string_0[] PROGMEM = 	"WIFI_EVENT_STAMODE_CONNECTED";   
const char string_1[] PROGMEM = 	"WIFI_EVENT_STAMODE_DISCONNECTED";
const char string_2[] PROGMEM =    	"WIFI_EVENT_STAMODE_AUTHMODE_CHANGE";
const char string_3[] PROGMEM =    	"WIFI_EVENT_STAMODE_GOT_IP";
const char string_4[] PROGMEM =    	"WIFI_EVENT_STAMODE_DHCP_TIMEOUT";
const char string_5[] PROGMEM =    	"WIFI_EVENT_SOFTAPMODE_STACONNECTED";
const char string_6[] PROGMEM =    	"WIFI_EVENT_SOFTAPMODE_STADISCONNECTED";
const char string_7[] PROGMEM =    	"WIFI_EVENT_SOFTAPMODE_PROBEREQRECVED";
const char string_8[] PROGMEM =    	"WIFI_EVENT_MAX";
const char string_9[] PROGMEM =    	"WIFI_EVENT_ANY = WIFI_EVENT_MAX";
const char string_10[] PROGMEM =    "WIFI_EVENT_MODE_CHANGE";

const char* const event_table[] PROGMEM = {string_0, string_1, string_2, string_3, string_4, string_5, string_6, string_7, string_8, string_9, string_10};

// enum {
//     STATION_IDLE = 0,
//     STATION_CONNECTING,
//     STATION_WRONG_PASSWORD,
//     STATION_NO_AP_FOUND,
//     STATION_CONNECT_FAIL,
//     STATION_GOT_IP
// };


const char * const RST_REASONS[] =
{
    "REASON_DEFAULT_RST",
    "REASON_WDT_RST",
    "REASON_EXCEPTION_RST",
    "REASON_SOFT_WDT_RST",
    "REASON_SOFT_RESTART",
    "REASON_DEEP_SLEEP_AWAKE",
    "REASON_EXT_SYS_RST"
};

const char * const OP_MODE_NAMES[] 
{
    "NULL_MODE",
    "STATION_MODE",
    "SOFTAP_MODE",
    "STATIONAP_MODE"
};
extern uint32_t now;
// const char * const EVENT_NAMES[]
// {
//     "EVENT_STAMODE_CONNECTED",
//     "EVENT_STAMODE_DISCONNECTED",
//     "EVENT_STAMODE_AUTHMODE_CHANGE",
//     "EVENT_STAMODE_GOT_IP",
//     "EVENT_SOFTAPMODE_STACONNECTED",
//     "EVENT_SOFTAPMODE_STADISCONNECTED",
//     "EVENT_MAX"
// };

// const char * const EVENT_REASONS[]
// {
//     "",
//     "REASON_UNSPECIFIED",
//     "REASON_AUTH_EXPIRE",
//     "REASON_AUTH_LEAVE",
//     "REASON_ASSOC_EXPIRE",
//     "REASON_ASSOC_TOOMANY",
//     "REASON_NOT_AUTHED",
//     "REASON_NOT_ASSOCED",
//     "REASON_ASSOC_LEAVE",
//     "REASON_ASSOC_NOT_AUTHED",
//     "REASON_DISASSOC_PWRCAP_BAD",
//     "REASON_DISASSOC_SUPCHAN_BAD",
//     "REASON_IE_INVALID",
//     "REASON_MIC_FAILURE",
//     "REASON_4WAY_HANDSHAKE_TIMEOUT",
//     "REASON_GROUP_KEY_UPDATE_TIMEOUT",
//     "REASON_IE_IN_4WAY_DIFFERS",
//     "REASON_GROUP_CIPHER_INVALID",
//     "REASON_PAIRWISE_CIPHER_INVALID",
//     "REASON_AKMP_INVALID",
//     "REASON_UNSUPP_RSN_IE_VERSION",
//     "REASON_INVALID_RSN_IE_CAP",
//     "REASON_802_1X_AUTH_FAILED",
//     "REASON_CIPHER_SUITE_REJECTED",
// };
//Serial.println(EVENT_REASONS[eventu->event_info.disconnected.reason]);
            
// const char * const EVENT_REASONS_200[]
// {
//     "REASON_BEACON_TIMEOUT",
//     "REASON_NO_AP_FOUND"
// };



void handleRoot(AsyncWebServerRequest *request);
void handleWifi(AsyncWebServerRequest *request);
void handleWifi0(AsyncWebServerRequest *request);
void handleWifiSave(AsyncWebServerRequest *request);
void handleInfo(AsyncWebServerRequest *request); 
void handleReset(AsyncWebServerRequest *request);
void handleNotFound(AsyncWebServerRequest *request);
int getRSSIasQuality(int RSSI);
void startServer();
void WiFiEvent(WiFiEvent_t event); 


template <typename Generic>
void          DEBUG_WM(Generic text);

#endif