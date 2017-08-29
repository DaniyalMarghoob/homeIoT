#include "globals.h"

AsyncMqttClient mqttClient;
AsyncWebServer server(PLUTO_HTTP_PORT);
//fauxmoESP fauxmo;
uint8_t pic_state[8];
String states_response;
uint8_t okToProceed = 0;


