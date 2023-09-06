#ifndef __STM32_CAN_H_
#define __STM32_CAN_H_

#include "stm32f4xx.h"
#include "sys.h"

#define CAN_RX_INT_CONFIG 1 //����CAN�����жϽṹ��

void CanTransPort_Init(void);

//uint8_t CanSendMsg(uint8_t *data,u8 len,uint32_t message_id); //CAN���ͱ���
uint8_t sendMsgToCan(CanTxMsg *msg,uint8_t *data);
uint16_t CanRecMsg(uint8_t *msg); 	//���ձ���



#endif

