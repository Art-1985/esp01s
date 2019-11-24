#ifndef __AT_ESP_H__
#define __AT_ESP_H__
#include "sys.h"
#define u8 unsigned char
#define ESP8266_RX_BUFFER_LEN 128 // Number of bytes in the serial receive buffer
#define TIMEOUT            1000

typedef struct esp8266
{
	u8 (*CheckESP)(void);
	u8 (*SetEspMode)(char mode);
	u8 (*ConnectWiFi)(char *SSID,char *Passwd);
	u8 (*DisconnectWiFi)(void);
	u8 (*ConnectionMode)(char mode);
	u8 (*ConnectServer)(char *IPAddress,char *port,char mode);
	u8 (*CloseTCPOrUDPConnect)(void);
	u8 (*SendStringDataToServer)(char *Data);
	u8 (*SendDataToServer)(char *Data,int len);
	u8 (*TCPserver)(char *mode,char *port);
	u8 (*SendStringDataToMultServer)(char *ipd,char *Data);
}esp8266;
void esp8266Init(esp8266 *handle);
void ClearESPBuffer();
char CheckResponse();

#endif
