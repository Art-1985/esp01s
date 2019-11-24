#include "ATcall.h"
#include "usart.h"
#include "AT_ESP.H"
extern char ESPBuffer[128];
/*
-------------------------------------------------------------------------------
����ATcall���Զ�����atcallģ�飬��ͨ��usart1���ͽ��
���ù��̣�
1�����atģ��
2��wifiģʽ����Ϊ3
3���򿪶�����ģʽ
4����tcp��������ip192.168.4.1��8080
-------------------------------------------------------------------------------
*/
void ATcall(void)
{
	esp8266 test;
	esp8266Init(&test);
	uart_init(115200);
	delay_init();
	if(!test.CheckESP()) //���ESP01
	{
		USART1_Info("�޷���⵽ESP01.....\r\n");
	}
	else{USART1_Info("ESP01������.....\r\n");}
	delay_ms(500);
	if(test.SetEspMode(3))  //wifiģʽ����
	{
		USART1_Info("ģʽ3������.....\r\n");
	}
	
//	if(test.ConnectWiFi("Lab","laboratory"))//WiFi���ӳ�ʱʱ��6s��
//	{
//		USART1_Info("wifi[lab]������.....\r\n");
//	}
//	else{(USART1_Info("wifi[lab]����ʧ��.....\r\n"));}
	delay_ms(500);	
	if(test.ConnectionMode(1))
	{
		USART1_Info("������ģʽ�Ѵ�.....\r\n");
	}
	else{(USART1_Info("������ģʽ��ʧ��.....\r\n"));}
	
	if(test.TCPserver("1","8080"))//Ĭ��ipΪ192.168.4.1
	{
		USART1_Info("�������ѿ���.....\r\n");
	}
	else{(USART1_Info("����������ʧ��.....\r\n"));}
	
}
/*
-------------------------------------------------------------------------------
����WiFiSendData��ͨ��WiFi���͵��ַ�
IPDΪ����ͻ���id
�����ڶ�����ģʽ��ʹ��
�����ַ���ֱ�ӵ���SendStringDataToMultServer(char *ipd,char *Data)
����test.SendStringDataToMultServer("0","jiqirenxiehui");
-------------------------------------------------------------------------------
*/
void WiFiSendData(u16 IPD,u16 DATA)
{	esp8266 test;
	char ipd[1];
	char data[1];
	ipd[0]=IPD+48;
	data[0]=DATA;
	esp8266Init(&test);
	test.SendStringDataToMultServer(ipd,data);

}
/*
-------------------------------------------------------------------------------
����WiFiReadData��ͨ��WiFi�����ַ���
-------------------------------------------------------------------------------
*/
void WiFiReadData()
{	esp8266 test;
	esp8266Init(&test);
	if(!CheckResponse())
	{
	USART1_SendString(ESPBuffer);
	}
	ClearESPBuffer();

}