#ifndef __AT_call_H__
#define __AT_call_H__
#include "AT_ESP.h"
#include "sys.h"
#include "delay.h"
void ATcall(void);//����at��ͨ��usart1���ͽ��
void WiFiSendData(u16 IPD,u16 DATA);//ͨ��WiFi�������ݣ�ipdΪ�����id��DATAΪ����
void WiFiReadData();//��WiFi���յ�������ͨ��usart1����
#endif