#include "includes.h"

void onMqttConnect(bool sessionPresent) {
#ifdef DEBUG_EN
  Serial.println("Connected");
  Serial.printf("Subscribing to topic %s and %s\n",SUBTOPIC_ROOM,SUBTOPIC_HOME);
#endif
  uint16_t packetIdSub = mqttClient.subscribe(SUBTOPIC_ROOM, 0);
  uint16_t packetIdSub1 = mqttClient.subscribe(SUBTOPIC_HOME, 0);
}

void onMqttDisconnect(AsyncMqttClientDisconnectReason reason) {
#ifdef DEBUG_EN
  Serial.println("Disconnected from the broker");
  Serial.println("Reconnecting to MQTT");
#endif
  mqttClient.connect();
}

void onMqttSubscribe(uint16_t packetId, uint8_t qos) {
#ifdef DEBUG_EN
  Serial.println("Subscribed");
#endif
}

void onMqttUnsubscribe(uint16_t packetId) {
  
}

void onMqttMessage(char* topic, char* payload, AsyncMqttClientMessageProperties properties, size_t len, size_t index, size_t total) 
{
  #ifdef DEBUG_EN
	Serial.print(payload);
  #endif 
  if(strstr(payload,"cmd 9"))
  {
  	#ifdef DEBUG_EN
  		Serial.print(payload);
  		Serial.println("Scene Request");
  	#endif
	char *end_str;
	char *token = strtok_r(payload, " ", &end_str);
	uint8_t val,ind;
	token = strtok_r(NULL, " ", &end_str);
	token = strtok_r(NULL, ",", &end_str);
	while (token != NULL)
	{
		char *end_token;
		char *token2 = strtok_r(token, " ", &end_token);
		ind = (uint8_t)atoi(token2);
		token2 = strtok_r(NULL, " ", &end_token);
		val = (uint8_t)atoi(token2);
		pic_state[ind-1] = val;
		#ifdef DEBUG_EN
		Serial.printf("ind = %d val = %d\n",ind,val);
		#endif
		token = strtok_r(NULL, ",", &end_str);
	}

	Serial.write(27);
	Serial.print("cmd 8");
	for(int i = 0;i<6;i++)
	{
		Serial.print(" ");
		Serial.print(pic_state[i]);
	}
  }
  if(strstr(payload,"getstates"))
  {
  	uint16_t packetIdPub2 = mqttClient.publish(PUBTOPIC, 0, true, states_response.c_str());
  	// Serial.print(payload);
  	// Serial.println("Scene Request");
  }
  else
  {
  	Serial.write(27);
  	Serial.println(payload);
  }
}

void onMqttPublish(uint16_t packetId) {

}

void onRequest(AsyncWebServerRequest *request)
{
  request->send(404,"text/plain","Not Found");
  
}

void HandleIndexRequest(AsyncWebServerRequest *request)
{
  request->send(SPIFFS, "/index.htm");
  //request->send(200, "text/plain", String(ESP.getFreeHeap()));
}
void HandleCommandRequestPOST(AsyncWebServerRequest *request)
{
	int i= 0;
	int params = request->params();
	for(i=0;i<params;i++)
	{
	  AsyncWebParameter* p = request->getParam(i);
	  if(p->isPost())
	  {
	  	if(p->name().startsWith("cmd"))
	  	{
	  		Serial.write(27);
	    	Serial.print(p->value());
	    	Serial.print("\r\n");
	    	request->send(200, "text/plain", "OK");
	    	return;
	  	}
	  } 
	}
	//request->send(200, "text/plain", "ERR");
}
void HandleCommandRequestGET(AsyncWebServerRequest *request)
{
	int i= 0;
	int params = request->params();
	for(i=0;i<params;i++)
	{
	  	AsyncWebParameter* p = request->getParam(i);
	  
		if(p->name().startsWith("cmd"))
		{
			Serial.write(27);
			Serial.print(p->value());
			Serial.print("\r\n");
			request->send(200, "text/plain", "OK");
			return;
		}
	}
	request->send(200, "text/plain", "OK");
}
void HandleScanRequest(AsyncWebServerRequest *request)
{
	String json = "[";
	int n = WiFi.scanComplete();
	if(n == -2)
	{
		WiFi.scanNetworks(true);
	} 
	else if(n)
	{
		for (int i = 0; i < n; ++i)
		{
	  		if(i)
	  		{ 
	  			json += ",";
	  		}
  			json += "{";
  			json += "\"rssi\":"+String(WiFi.RSSI(i));
  			json += ",\"ssid\":\""+WiFi.SSID(i)+"\"";
  			json += ",\"bssid\":\""+WiFi.BSSIDstr(i)+"\"";
  			json += ",\"channel\":"+String(WiFi.channel(i));
  			json += ",\"secure\":"+String(WiFi.encryptionType(i));
  			json += ",\"hidden\":"+String(WiFi.isHidden(i)?"true":"false");
  			json += "}";
		}
		WiFi.scanDelete();
		if(WiFi.scanComplete() == -2)
		{
	  		WiFi.scanNetworks(true);
		}
	}
	json += "]";
	request->send(200, "text/json", json);
	json = String();
}
void singleStateRequest (AsyncWebServerRequest *request)
{
	int i= 0;
	int ind = 0;
	int params = request->params();
	for(i=0;i<params;i++)
	{
	  	AsyncWebParameter* p = request->getParam(i);
	  
		if(p->name().startsWith("l_id"))
		{
			ind = atoi(p->value().c_str());
			request->send(200, "text/plain", pic_state[ind - 1] ? "1" : "0");
		}
		else if(p->name().startsWith("b_id"))
		{
			ind = atoi(p->value().c_str());
			request->send(200, "text/plain", String(pic_state[ind - 1]));
		}
		
	}
	//request->send(200, "text/plain", "ERR");
}
void HandleStateRequest (AsyncWebServerRequest *request)
{
	
	request->send(200, "text/json", states_response);
	//AsyncResponseStream *response = request->beginResponseStream("application/xml");
	// SSDP.printSchema(response);
	// request->send(response);
}
