#include "sys.h"  //����ϵͳͷ�ļ�
#include "delay.h"
#include "AT_ESP.h"
#include "usart.h"
#include "bsp_usart2.h"
#include "stdlib.h"
#include "delay.h"
#include "ATcall.h"
extern char ESPBuffer[128];
int main(void)
{
	ATcall();//����esp01s
	WiFiSendData(0,'a');
	while(1)
	{
	delay_ms(500);
	WiFiReadData();}
}