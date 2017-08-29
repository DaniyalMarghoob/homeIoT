#ifndef __CALLBACKS_H__
#define __CALLBACKS_H__

void onMqttConnect(bool sessionPresent);
void onMqttDisconnect(AsyncMqttClientDisconnectReason reason);
void onMqttSubscribe(uint16_t packetId, uint8_t qos);
void onMqttUnsubscribe(uint16_t packetId);
void onMqttMessage(char* topic, char* payload, AsyncMqttClientMessageProperties properties, size_t len, size_t index, size_t total);
void onMqttPublish(uint16_t packetId);

void HandleScanRequest(AsyncWebServerRequest *request);
void HandleIndexRequest(AsyncWebServerRequest *request);
void onRequest(AsyncWebServerRequest *request);
void HandleCommandRequestPOST(AsyncWebServerRequest *request);
void HandleCommandRequestGET(AsyncWebServerRequest *request);
void singleStateRequest (AsyncWebServerRequest *request);
void HandleStateRequest (AsyncWebServerRequest *request);

#endif