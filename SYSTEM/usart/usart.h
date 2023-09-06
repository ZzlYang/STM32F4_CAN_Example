#ifndef __USART_H
#define __USART_H

#include <stdarg.h>
#include <stdlib.h>
#include <string.h>
#include "stdio.h"	
#include "stm32f4xx_conf.h"
#include "sys.h" 

#define USART_REC_LEN  			200  	//定义最大接收字节数 200
#define EN_USART1_RX 			1		//使能（1）/禁止（0）串口1接收
#define EN_USART2_RX			1		//enable 1 disalble 0
#define EN_USART3_RX 			1		//使能（1）/禁止（0）串口1接收
#define EN_USART4_RX			1		//enable 1 disalble 0

#define EN_USART6_RX			1 

extern u8  USART1_RX_BUF[USART_REC_LEN]; //接收缓冲,最大USART_REC_LEN个字节.末字节为换行符 
extern u8  USART2_RX_BUF[USART_REC_LEN];
extern u8  USART3_RX_BUF[USART_REC_LEN];
extern u8  USART4_RX_BUF[USART_REC_LEN];

extern u8  USART6_RX_BUF[USART_REC_LEN];

extern u16 USART1_RX_STA;         		//接收状态标记
extern u16 USART2_RX_STA;
extern u16 USART3_RX_STA;
extern u16 USART4_RX_STA;

extern u16 USART6_RX_STA;
//如果想串口中断接收，请不要注释以下宏定义
void Usart_Init(u32 bound);
void usart1_init(u32 bound);
void usart2_init(u32 bound);
void usart3_init(u32 bound);
void usart4_init(u32 bound);
void usart5_init(u32 bound);
void usart6_init(u32 bound);

void USART1_printf(char *fmt, ...);
void USART2_printf(char *fmt, ...);
void USART3_printf(char *fmt, ...);
void USART4_printf(char *fmt, ...);
void USART5_printf(char *fmt, ...);
void USART6_printf(char *fmt, ...);
#endif


