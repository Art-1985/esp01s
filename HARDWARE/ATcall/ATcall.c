#include "ATcall.h"
#include "usart.h"
#include "AT_ESP.H"
extern char ESPBuffer[128];
/*
-------------------------------------------------------------------------------
函数ATcall：自动配置atcall模块，并通过usart1发送结果
配置过程：
1，检测at模块
2，wifi模式配置为3
3，打开多链接模式
4，打开tcp服务器，ip192.168.4.1：8080
-------------------------------------------------------------------------------
*/
void ATcall(void)
{
	esp8266 test;
	esp8266Init(&test);
	uart_init(115200);
	delay_init();
	if(!test.CheckESP()) //检测ESP01
	{
		USART1_Info("无法检测到ESP01.....\r\n");
	}
	else{USART1_Info("ESP01已连接.....\r\n");}
	delay_ms(500);
	if(test.SetEspMode(3))  //wifi模式设置
	{
		USART1_Info("模式3已设置.....\r\n");
	}
	
//	if(test.ConnectWiFi("Lab","laboratory"))//WiFi连接超时时间6s；
//	{
//		USART1_Info("wifi[lab]已连接.....\r\n");
//	}
//	else{(USART1_Info("wifi[lab]连接失败.....\r\n"));}
	delay_ms(500);	
	if(test.ConnectionMode(1))
	{
		USART1_Info("多连接模式已打开.....\r\n");
	}
	else{(USART1_Info("多连接模式打开失败.....\r\n"));}
	
	if(test.TCPserver("1","8080"))//默认ip为192.168.4.1
	{
		USART1_Info("服务器已开启.....\r\n");
	}
	else{(USART1_Info("服务器开启失败.....\r\n"));}
	
}
/*
-------------------------------------------------------------------------------
函数WiFiSendData：通过WiFi发送单字符
IPD为接入客户端id
仅可在多连接模式下使用
发送字符串直接调用SendStringDataToMultServer(char *ipd,char *Data)
例如test.SendStringDataToMultServer("0","jiqirenxiehui");
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
函数WiFiReadData：通过WiFi接收字符串
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