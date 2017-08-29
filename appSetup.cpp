#include "includes.h"

void appInit(void)
{
	fil_loads(NULL);
	registerMqttCallBacks();
	//registerHttpCallBacks();
	registerOTACallBacks();
	//initSSDP();
	/*fauxmo.addDevice("zone one");
    fauxmo.addDevice("zone two");
    fauxmo.addDevice("zone three");
    fauxmo.addDevice("zone four");
    fauxmo.addDevice("living room");
    fauxmo.addDevice("Both Lights");
    fauxmo.onMessage([](unsigned char device_id, const char * device_name, bool state) {
        //Serial.printf("[MAIN] Device #%d (%s) state: %s\n", device_id, device_name, state ? "ON" : "OFF");
        switch(device_id)
        {
        	case 0:
        			if(state)
        			{
        				Serial.write(27);
        				Serial.print("cmd 1 100\r\n");
        			}
        			else
        			{
        				Serial.write(27);
        				Serial.print("cmd 1 0\r\n");
        			}
        			break;
        	case 1:
        			if(state)
        			{
        				Serial.write(27);
        				Serial.print("cmd 2 100\r\n");
        			}
        			else
        			{
        				Serial.write(27);
        				Serial.print("cmd 2 0\r\n");
        			}
        			break;
        	case 2:
        			if(state)
        			{
        				Serial.write(27);
        				Serial.print("cmd 3 100\r\n");
        			}
        			else
        			{
        				Serial.write(27);
        				Serial.print("cmd 3 0\r\n");
        			}
        			break;
        	case 3:
        			if(state)
        			{
        				Serial.write(27);
        				Serial.print("cmd 4 100\r\n");
        			}
        			else
        			{
        				Serial.write(27);
        				Serial.print("cmd 4 0\r\n");
        			}
        			break;
        	case 4:
        			if(state)
        			{
        				Serial.write(27);
        				Serial.print("cmd 8 100 100 100 100 100 100\r\n");
        			}
        			else
        			{
        				Serial.write(27);
        				Serial.print("cmd 8 0 0 0 0 0 0\r\n");
        			}
        			break;
        	case 5:
        			if(state)
        			{
        				Serial.write(27);
        				Serial.print("cmd 8 100 100\r\n");
        			}
        			else
        			{
        				Serial.write(27);
        				Serial.print("cmd 8 0 0\r\n");
        			}
        			break;
        	
        }
    });*/
}
void configWiFi()
{

	// //WiFi.disconnect();

	// WiFiManager wifiManager;
	// //wifiManager.setDebugOutput(false);
 //    wifiManager.autoConnect("PlutoLabsConfig");
 
	
}
void registerMqttCallBacks(void)
{
	mqttClient.onConnect(onMqttConnect);
	mqttClient.onDisconnect(onMqttDisconnect);
	mqttClient.onSubscribe(onMqttSubscribe);
	mqttClient.onUnsubscribe(onMqttUnsubscribe);
	mqttClient.onMessage(onMqttMessage);
	mqttClient.onPublish(onMqttPublish);
	mqttClient.setServer(IPAddress(52,42,39,67), 1883);
	int radix = 10;
	char buffer[33];
	char *strptr;
	mqttClient.setKeepAlive(5).setCleanSession(false).setWill("topic/online", 2, true, "no").setClientId(ltoa(ESP.getChipId(),buffer,radix));
	#ifdef DEBUG_EN
		Serial.println("Connecting to MQTT...");
	#endif
	mqttClient.connect();

}

void registerHttpCallBacks(void)
{
	//SPIFFS.begin();
	//server.serveStatic("/", SPIFFS, "/").setDefaultFile("index.htm");
    

	server.on("/", HTTP_GET,handleInfo);
    server.on("/command", HTTP_POST,HandleCommandRequestPOST);
	server.on("/command", HTTP_GET,HandleCommandRequestGET);
	server.onNotFound(onRequest);
	server.begin();
}

void initSSDP(void)
{
	#ifdef DEBUG_EN
		Serial.printf("Starting SSDP...\n");
	#endif`
    SSDP.setSchemaURL("description.xml");
    SSDP.setHTTPPort(80);
    SSDP.setName("Philips hue clone");
    SSDP.setSerialNumber("001788102201");
    SSDP.setURL("index.html");
    SSDP.setModelName("Philips hue bridge 2012");
    SSDP.setModelNumber("929000226503");
    SSDP.setModelURL("http://www.meethue.com");
    SSDP.setManufacturer("Royal Philips Electronics");
    SSDP.setManufacturerURL("http://www.philips.com");
    //urn:schemas-upnp-org:device:Basic:1 
    SSDP.begin();
}

void registerOTACallBacks()
{
	ArduinoOTA.setPort(8266);
	ArduinoOTA.setHostname(SERVICENAME);
	ArduinoOTA.setPassword((const char *)"4334");

    
	ArduinoOTA.onStart([]() {
	#ifdef DEBUG_EN
		Serial.println("OTA Started");
	#endif
	});
	ArduinoOTA.onEnd([]() {
		#ifdef DEBUG_EN
			Serial.println("OTA Ended\n");
		#endif
        ESP.reset();
	});
	ArduinoOTA.onProgress([](unsigned int progress, unsigned int total) {
		#ifdef DEBUG_EN
			Serial.printf("Progress: %u%%\r", (progress / (total / 100)));
		#endif
	});
	ArduinoOTA.onError([](ota_error_t error) {
		#ifdef DEBUG_EN
		Serial.printf("Error[%u]: ", error);
		if (error == OTA_AUTH_ERROR) 
			Serial.println("Auth Failed");
		else if (error == OTA_BEGIN_ERROR) 
			Serial.println("Begin Failed");
		else if (error == OTA_CONNECT_ERROR) 
			Serial.println("Connect Failed");
		else if (error == OTA_RECEIVE_ERROR) 
			Serial.println("Receive Failed");
		else if (error == OTA_END_ERROR) 
			Serial.println("End Failed");
		#endif
	});
	ArduinoOTA.begin();
    MDNS.addService("http", "tcp", 80);
}
//GET /description.xml HTTP/1.1\r\n Host: 192.168.16.105\r\n Connection: close\r\n Accept: */*\r\n User-Agent: Mozilla/4.0 (compatible; esp8266 Lua; Windows NT 5.1)\r\n\r\n


void fil_loads(char* states_str)
{
  char *p = strtok(states_str, " ");
  p = strtok(NULL, " ");
  int i =0;
  while (p) 
  {
    pic_state[i++] = atoi(p);
    p = strtok(NULL, " ");
  }
  states_response = "";
  states_response += "{";
  states_response += "\"roomID\":\""+String(ROOM_ID)+"\"";
  states_response += ",\"switches\":[";
  states_response += "\""+String(pic_state[0])+"\",";
  states_response += "\""+String(pic_state[1])+"\",";
  states_response += "\""+String(pic_state[2])+"\",";
  states_response += "\""+String(pic_state[3])+"\",";
  states_response += "\""+String(pic_state[4])+"\",";
  states_response += "\""+String(pic_state[5])+"\"],\"Temperature\":";
  states_response += "\""+String(pic_state[6])+"\",\"Electricity\":";
  states_response += "\""+String(pic_state[7])+"kwh\"}";
  #ifdef DEBUG_EN
    Serial.println(states_response);
  #endif
}

void checkSerial()
{
    static char t;
    String comd;
    char * states_str;
    while(Serial.available()>0)
    {
        t = Serial.read();
        comd.concat(t);
        if(t=='\n')
        {
            #ifdef DEBUG_EN
                Serial.print(comd);
            #endif
            states_str = (char *)comd.c_str();
        }
        delay(3);
    }
    if(states_str != NULL)
    {
        if(strstr(states_str,"loads"))
        {
            fil_loads(states_str);
            #ifdef DEBUG_EN
                Serial.printf("states : %d %d %d %d %d %d",pic_state[0],pic_state[1],pic_state[2],pic_state[3],pic_state[4],pic_state[5]);
            #endif
        }
        else
        {
            #ifdef DEBUG_EN
                Serial.println("invalid command");
            #endif
        }
    states_str = NULL;
    comd = "";
    }
}