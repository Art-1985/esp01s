#ifndef __AT_call_H__
#define __AT_call_H__
#include "AT_ESP.h"
#include "sys.h"
#include "delay.h"
void ATcall(void);//配置at，通过usart1发送结果
void WiFiSendData(u16 IPD,u16 DATA);//通过WiFi发送数据，ipd为接入端id，DATA为数据
void WiFiReadData();//将WiFi接收到的数据通过usart1发送
#endif
