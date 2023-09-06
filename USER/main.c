#include "stm32f4xx.h"
#include "usart.h"
#include "delay.h"
#include "stm32_can.h"

extern CanRxMsg rxMessage;
extern CanTxMsg txMessage;

int main(void)
{	
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
	uint8_t txBuf[8];
	CanTransPort_Init();
	usart2_init(115200);
	delay_init(168);
	uint8_t i = 0;
	uint8_t cnt = 0;
	txBuf[0]=0x0e;
	txBuf[1]=0x1c;
	txBuf[2]=0xee;
	txBuf[3]=0xb0;
	txBuf[4]=0xe7;
	txBuf[5]=0xa1;
	txBuf[6]=0x99;
	txBuf[7]=0x35;
	
	USART2_printf("´«Êä¿ªÊ¼!\r\n");
	
	while(1)
	{
//		CanSendMsg(txBuf,8,0x00000000);
		sendMsgToCan(&txMessage,txBuf);
		txBuf[i]++;
		i++;
		if(i%8 == 0)
		{
			i = 0;
			cnt++;
		}
		if(cnt%4 == 0)
		{
			cnt = 0;
			for(i=0;i<8;i++)
			{
				txBuf[i]-=0x1F;
			}
			i=0;
		}
		delay_ms(2000);
		
	}
}



