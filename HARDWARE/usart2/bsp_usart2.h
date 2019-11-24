#ifndef __BSP_USART2_H__
#define __BSP_USART2_H__
#include "sys.h"

void USART2_Config(void);
void USART2_SendString(char *buffer);
void USART2_SendData(char *buffer,int len);
void GetEspData(char *RecvBuffer);

#endif
