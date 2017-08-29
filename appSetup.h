#ifndef __APPSETUP_H__
#define __APPSETUP_H__

void appInit(void);
void registerMqttCallBacks();
void registerHttpCallBacks();
void registerOTACallBacks();
void configWiFi();
void initSSDP(void);
void fil_loads(char* states_str);
void checkSerial();

#endif