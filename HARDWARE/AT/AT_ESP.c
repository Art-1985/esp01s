#include "AT_ESP.h"
#include "bsp_usart2.h"
#include "string.h"
#include "stdio.h"
#include "sys.h"
#include "delay.h"
char ESPBuffer[128];
u16 ESPRxIndex = 0x00;
const char SendResponse[] = "OK\r\n";

#define ESP_UART_CONFIG        USART2_Config
#define SendMessageToESP       USART2_SendString
#define SendMessageToESP2      USART2_SendData


//用中断接收ESP的反馈信息
void USART2_IRQHandler(void)
{
	if(USART_GetFlagStatus(USART2, USART_FLAG_ORE) != RESET)
	{
		USART_ReceiveData( USART2 );
		USART_ClearITPendingBit(USART2,USART_IT_ORE);
	}
	if(USART_GetITStatus(USART2, USART_IT_RXNE) != RESET)
	{
		if(ESPRxIndex < 128)
		{
			ESPBuffer[ESPRxIndex++] = USART_ReceiveData(USART2);
		}
		else
		{
			USART_ITConfig(USART2, USART_IT_RXNE, DISABLE);
		}
	}
}       
/*
-------------------------------------------------------------------------------
下面的是一些基础函数
-------------------------------------------------------------------------------
*/

static void delay()
{
	int i,j;
	i = 1000;
	while(i--)
	{
		j = 350;
		while(j--);
	}
}

void ClearESPBuffer()
{
	ESPRxIndex = 0x00;
	memset(ESPBuffer, 0, ESP8266_RX_BUFFER_LEN);
}


static void SendEspCommand(char *order)
{
	ClearESPBuffer();
	SendMessageToESP(order);
}

static void SendEspCommand2(char *data,int len)
{
	ClearESPBuffer();
	SendMessageToESP2(data,len);
}


char CheckResponse()
{
	if(strstr(ESPBuffer,SendResponse) == NULL)
	{
		return 0;
	}
	return 1;
	ClearESPBuffer();
}

/*
-------------------------------------------------------------------------------
基础函数结束
-------------------------------------------------------------------------------
*/

/*
-------------------------------------------------------------------------------
接下来是ESP01的API
-------------------------------------------------------------------------------
*/


/*
函数说明：检测单片机和ESP-01是否正确通信
参数说明：void
返回值说明:函数返回值为1是success,0为fail
注意事项：void
已测试
*/
u8 CheckEsp(void)
{
	SendEspCommand("AT\r\n");
	delay();
	if(CheckResponse())
	{
		return 1;
	}
	return 0;
}

/*
函数说明：设置ESP-01的当前的WIFI模式
参数说明：mode是想要设置的模式，具体模式详见AT指令手册
返回值说明:函数返回值为1是success,0为fail
注意事项：void
已测试
*/
u8 SetEspMode(char mode)
{	
	char tmp[3];
	tmp[0] = mode+48;//测试mode传回的为数字型，+48后改为字符型；
	tmp[1] = '\r';
	tmp[2] = '\n';
	SendEspCommand("AT+CWMODE=");
	SendEspCommand(tmp);
	delay();
	if(CheckResponse())
	{
		return 1;
	}
	return 0;
}
/*
函数说明：设置ESP-01的链接模式
参数说明：mode是想要设置的模式，具体模式详见AT指令手册
返回值说明:函数返回值为1是success,0为fail
注意事项：void
已测试
*/
u8 ConnectionMode(char mode)
{	
	char tmp[3];
	tmp[0] = mode+48;//测试mode传回的为数字型，+48后改为字符型；
	tmp[1] = '\r';
	tmp[2] = '\n';
	SendEspCommand("AT+CIPMUX=");
	SendEspCommand(tmp);
	delay();
	if(CheckResponse())
	{
		return 1;
	}
	return 0;
}
/*
函数说明：让ESP-01连接指定WIFI
参数说明：SSID:欲要连接的WIFI名，Passwd:对应的WIFI密码
返回值说明:函数返回值为1是success,0为fail
注意事项：void
已测试
*/
u8 ConnectWiFi(char *SSID,char *Passwd)
{
	SendEspCommand("AT+CWJAP=\"");
	SendEspCommand(SSID);
	SendEspCommand("\",\"");
	SendEspCommand(Passwd);
	SendEspCommand("\"\r\n");
	delay_ms(1000);
	delay_ms(1000);
	delay_ms(1000);
	delay_ms(1000);
	delay_ms(1000);
	delay_ms(1000);
	if(CheckResponse())
	{
		return 1;
	}
	return 0;
}


/*
函数说明：断开ESP-01的连接的WIFI
参数说明：void
返回值说明:函数返回值为1是success,0为fail
注意事项：void
已测试
*/
u8 DisconnectWiFi(void)
{
	SendEspCommand("AT+CWQAP\r\n");
	delay();
	if(CheckResponse())
	{
		return 1;
	}
	return 0;
}
/*
函数说明：TCP服务器配置
参数说明：mode:服务器配置模式，port:端口
返回值说明:函数返回值为1是success,0为fail
注意事项：void
已测试
*/
u8 TCPserver(char *mode,char *port)
{
	SendEspCommand("AT+CIPSERVER=");
	SendEspCommand(mode);
	SendEspCommand(",");
	SendEspCommand(port);
	SendEspCommand("\r\n");
	delay_ms(50);
	if(CheckResponse())
	{
		return 1;
	}
	return 0;
}
/*
函数说明：连接指定的TCPServer或者UDPServer
参数说明：IPAddress：TCPServer或者UDPServer的IPAddress，port：对应服务器的端口
					mode:连接Server的类型，0为TCP,1为UDP
返回值说明:函数返回值为1是success,0为fail
注意事项：void
已测试
*/

u8 ConnectServer(char *IPAddress,char *port,char mode)
{
	if(mode == 0)
	{
		SendEspCommand("AT+CIPSTART=\"TCP\",\"");
		SendEspCommand(IPAddress);
		SendEspCommand("\",");
		SendEspCommand(port);
		SendEspCommand("\r\n");
		delay_ms(1000);
		if(CheckResponse())
		{
			return 1;
		}
		return 0;
	}
	else if(mode == 1)
	{
		SendEspCommand("AT+CIPSTART=\"UDP\",\"");
		SendEspCommand(IPAddress);
		SendEspCommand("\",");
		SendEspCommand(port);
		SendEspCommand("\r\n");
		delay_ms(1000);
		if(CheckResponse())
		{
			return 1;
		}
		return 0;
	}
	else
	{
		return 0;
	}
}

/*
函数说明：关闭已连接的TCPServer or UDPServer
参数说明：void
返回值说明:函数返回值为1是success,0为fail
注意事项：void
已测试
*/
u8 CloseTCPOrUDPConnect(void)
{
	SendEspCommand("AT+CIPCLOSE\r\n");
	delay_ms(1000);
	if(CheckResponse())
	{
		return 1;
	}
	return 0;
}

/*
函数说明：向已经连接的Sever发送字符串数据
参数说明：Data:要发送的字符串信息指针
返回值说明:函数返回值为1是success,0为fail
注意事项：这里的Server可以是TCPServer或者UDPServer，视乎于你之前连接的Server
					即如果你在使用这个函数之前使用的是TCPServer，那么该函数就向你所连接的
					TCPServer发送字符串数据；当是UDPServer，也是如此。
已测试
*/
u8 SendStringDataToServer(char *Data)
{
	char params[8];
	int len;
	len = strlen(Data);
	sprintf(params,"%d",len);
	SendEspCommand("AT+CIPSEND=");
	SendEspCommand(params);
	SendEspCommand("\r\n");
	delay();
	if(CheckResponse())
	{
		SendEspCommand(Data);
		delay();
		delay();
		return 1;
	}
	return 0;
}
/*
函数说明：向已经连接的Sever发送字符串信息,发送中文会无法解码
参数说明：Data:要发送数据的头指针，ipd：客户端id
返回值说明:函数返回值为1是success,0为fail
注意事项：这里的Server可以是TCPServer或者UDPServer，视乎于你之前连接的Server
					即如果你在使用这个函数之前使用的是TCPServer，那么该函数就向你所连接的
					TCPServer发送字符串数据；当是UDPServer，也是如此。
已测试
*/
u8 SendStringDataToMultServer(char *ipd,char *Data)
{
	char params[8];
	int len;
	len = strlen(Data);
	sprintf(params,"%d",len);
	SendEspCommand("AT+CIPSEND=");
	SendEspCommand(ipd);
	SendEspCommand(",");
	SendEspCommand(params);
	SendEspCommand("\r\n");
	delay();
	if(CheckResponse())
	{
		SendEspCommand2(Data,len);
		delay();
		delay();
		return 1;
	}
	return 0;
}
/*
函数说明：向已经连接的Sever发送非字符串信息(发送例如结构体等内存对象信息)
参数说明：Data:要发送数据的头指针，len：发送数据长度
返回值说明:函数返回值为1是success,0为fail
注意事项：这里的Server可以是TCPServer或者UDPServer，视乎于你之前连接的Server
					即如果你在使用这个函数之前使用的是TCPServer，那么该函数就向你所连接的
					TCPServer发送字符串数据；当是UDPServer，也是如此。
已测试
*/
u8 SendDataToServer(char *Data,int len)
{
	char params[8];
	sprintf(params,"%d",len);
	SendEspCommand("AT+CIPSEND=");
	SendEspCommand(params);
	SendEspCommand("\r\n");
	delay();
	if(CheckResponse())
	{
		SendEspCommand2(Data,len);
		delay();
		delay();
		return 1;
	}
	return 0;
}

/*
函数说明：esp8266结构体的初始化和对应UART的初始化
参数说明：IPAddress：TCPServer的IPAddress，port：对应服务器的端口
返回值说明:函数返回值为1是success,0为fail
注意事项：void
已测试
*/
void esp8266Init(esp8266 *handle)
{
	ESP_UART_CONFIG();                  //初始化ESP-01对应的串口
	handle->CheckESP = CheckEsp;        //这些是让函数指针对应相应的函数
	handle->SetEspMode = SetEspMode;
	handle->ConnectionMode = ConnectionMode;
	handle->ConnectWiFi = ConnectWiFi;
	handle->DisconnectWiFi = DisconnectWiFi;
	handle->TCPserver = TCPserver;
	handle->ConnectServer = ConnectServer;
	handle->CloseTCPOrUDPConnect = CloseTCPOrUDPConnect;
	handle->SendStringDataToServer = SendStringDataToServer;
	handle->SendStringDataToMultServer = SendStringDataToMultServer;
	handle->SendDataToServer = SendDataToServer;
}
