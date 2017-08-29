#ifndef __GLOBALS_H__
#define __GLOBALS_H__
#include "includes.h"
extern AsyncMqttClient mqttClient;
extern AsyncWebServer server;
//extern fauxmoESP fauxmo;
extern uint8_t pic_state[8];
extern String states_response;
extern uint8_t okToProceed;
#endif