@[基于stm32f1的esp01s的驱动库及使用](这里写自定义目录标题)

> 基于用户zero_rose的驱动改进，文章来源[STM32的ESP01驱动库_面向对象的形式](https://blog.csdn.net/zero_rose/article/details/80955738)
> 
第一次写博客，如有错误请多包涵。
话不多说，直接上部分代码，剩下的下面慢慢解释：

```c
int main(void)
{
	ATcall();//配置esp01s，此处配置的为多路连接tcp模式
	WiFiSendData(0,'a');//通过esp01s向0号客户端发送数据‘a’
	while(1)
	{
	delay_ms(500);
	WiFiReadData();//通过esp01s读取客户端发送的数值，并通过usart1发送出去
	}
}
```
以上为主函数部分，通过打包好的函数实现了通过WiFi的数据收发；接下来我将贴上配置esp的代码：

```c
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
```
 通过创建的库函数，我们直接对esp01配置，省下了繁琐的at命令配置。接下来是WiFi收发的代码
 

```c
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
```
 目前就实现了最基本的配置，更多的接下来会慢慢更新，当然你也可以自行编写，原理都差不多，通过编写的库函数很容易将其他at命令集成进来。
 

 完整的工程我[在我的github里，下载后](https://github.com/zhanglu-2001/esp01s)，可以直接烧录到stmf103c8t6中使用。
 github地址：https://github.com/zhanglu-2001/esp01s
