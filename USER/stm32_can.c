#include "stm32_can.h"
#include "usart.h"

CanRxMsg rxMessage;
CanTxMsg txMessage;

void CanTransPort_Init(void)
{
	GPIO_InitTypeDef 			 GPIO_InitStructure;		//GPIO物理接口
	CAN_InitTypeDef        CAN_InitStructure;	//CAN外设初始结构体	用于决定收发
	CAN_FilterInitTypeDef  CAN_FilterInitStructure;	//CAN过滤器结构体 用于控制收发
	
#if CAN_RX_INT_CONFIG
	NVIC_InitTypeDef NVIC_InitStructure;
#endif
	//PD0 RX PD1 TX
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD,ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_CAN1,ENABLE);
	
	GPIO_PinAFConfig(GPIOD,GPIO_PinSource0,GPIO_AF_CAN1);
	GPIO_PinAFConfig(GPIOD,GPIO_PinSource1,GPIO_AF_CAN1);
	
	//Configure CAN1 TX
	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_1;
	GPIO_InitStructure.GPIO_PuPd=GPIO_PuPd_UP;
	GPIO_InitStructure.GPIO_OType=GPIO_OType_PP;
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_AF;
	GPIO_InitStructure.GPIO_Speed=GPIO_Fast_Speed;
	
	GPIO_Init(GPIOD,&GPIO_InitStructure);
	
	//Configure CAN1 RX
	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_0;
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_AF;
	GPIO_Init(GPIOD,&GPIO_InitStructure);
	
	
	//CAN外设初始化
	CAN_DeInit(CAN1);
	CAN_StructInit(&CAN_InitStructure);
	
	CAN_InitStructure.CAN_Mode=CAN_Mode_Normal;
	CAN_InitStructure.CAN_ABOM=ENABLE; //自动离线管理
	CAN_InitStructure.CAN_AWUM=ENABLE;	//自动唤醒
	//42MHZ / (1+4+2)/ 12 = 500kbps
	CAN_InitStructure.CAN_BS1=CAN_BS1_4tq;
	CAN_InitStructure.CAN_BS2=CAN_BS2_2tq;
	CAN_InitStructure.CAN_SJW=CAN_SJW_2tq;
	CAN_InitStructure.CAN_Prescaler=12;
	CAN_InitStructure.CAN_RFLM=DISABLE; //锁定模式
	CAN_InitStructure.CAN_NART=DISABLE; //自动重传
	CAN_InitStructure.CAN_TTCM=DISABLE; //时间触发模式
	CAN_InitStructure.CAN_TXFP=DISABLE; //报文发送优先级判定方法
	
	CAN_Init(CAN1,&CAN_InitStructure);
	
	//CAN过滤器初始化
	CAN_FilterInitStructure.CAN_FilterNumber=0;
	CAN_FilterInitStructure.CAN_FilterMode=CAN_FilterMode_IdMask;
	CAN_FilterInitStructure.CAN_FilterFIFOAssignment=CAN_FilterFIFO0; //安装过滤器
	CAN_FilterInitStructure.CAN_FilterIdHigh=0x00000000;
	CAN_FilterInitStructure.CAN_FilterIdLow=0x0000000;
	CAN_FilterInitStructure.CAN_FilterScale=CAN_FilterScale_32bit;
	CAN_FilterInitStructure.CAN_FilterActivation=ENABLE;	//过滤器使能
	
	CAN_FilterInit(&CAN_FilterInitStructure);
	
#if CAN_RX_INT_CONFIG
	NVIC_InitStructure.NVIC_IRQChannel=CAN1_RX0_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=0x01;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority=0x00;
	NVIC_InitStructure.NVIC_IRQChannelCmd=ENABLE;
	NVIC_Init(&NVIC_InitStructure);
	CAN_ITConfig(CAN1,CAN_IT_FMP0,ENABLE);
#endif
}
	

#if CAN_RX_INT_CONFIG

void CAN1_RX0_IRQHandler(void)
{	
	printf("enter CAN1_RX0_IRQHandler \r\n");
	//	if(CAN_GetFlagStatus(CAN1,CAN_IT_FMP0) != RESET)		//添加中断会发送程序会死循环在中断里
//	{
		CAN_Receive(CAN1,CAN_FIFO0,&rxMessage);
		printf("Data:%s\r\n",rxMessage.Data);
		printf("ExtId:%d\r\n",rxMessage.ExtId);
//		CAN_ClearITPendingBit(CAN1,CAN_IT_FMP0);
//	}
	printf("exti CAN1_RX0_IRQHandler \r\n");
}

#endif


//uint8_t CanSendMsg(uint8_t *msg,uint8_t len)
//{
//	uint8_t mBox=0x00;
//	uint16_t i=0;
//	CanTxMsg TxMessage;
//	TxMessage.StdId=0x12;
//	TxMessage.ExtId=0x24;
//	TxMessage.IDE=CAN_Id_Extended; //使用拓展帧模式
//	TxMessage.RTR=CAN_RTR_Data;
//	TxMessage.DLC=len;
//	
//	for(i=0;i<len;i++)
//	{
//		TxMessage.Data[i] = msg[i];
//	}
//	mBox = CAN_Transmit(CAN1,&TxMessage);
//	i=0;
//	while((CAN_TransmitStatus(CAN1,mBox) == CAN_TxStatus_Failed) && (i<0xFFFF))	//超时等待
//	{
//		i++;
//	}
//	if( i>=0xFFF ) return 1;
//	return 0;
//}

//CanRxMsg CAN1_Rx_Msg;

//uint8_t CanSendMsg(uint8_t *data,u8 len,uint32_t message_id)
//{
//	uint8_t mailBox;
//	uint32_t cnt=0;
//	CanTxMsg Msg;
//	if(len>8)
//	{
//		return 1;
//	}
//	Msg.IDE =CAN_Id_Extended;
//	Msg.RTR=CAN_RTR_Data;
//	Msg.DLC=len;
//	Msg.ExtId=message_id;
//	Msg.StdId=message_id;
//	for(uint8_t i=0;i<len;i++)
//	{
//		Msg.Data[i]=data[i];
//	}
//	
//	mailBox = CAN_Transmit(CAN1,&Msg);
//	while(CAN_TransmitStatus(CAN1,mailBox) !=CAN_TxStatus_Ok && cnt < 2000)
//	{
//		cnt++;
//	}
//	if(cnt >1000)
//	{
//		return 1;
//	}
//	else 
//		return 0;
//}
uint8_t sendMsgToCan(CanTxMsg *msg,uint8_t *data)
{
	uint16_t i = 0;
	uint8_t mailBox;
	msg->ExtId=0x1e2c;
	msg->IDE=CAN_Id_Extended;
	msg->RTR=CAN_RTR_Data;
	msg->DLC=8;
	
	for(i = 0;i<8;i++)
	{
		msg->Data[i] = data[i];
	}
	i=0;
	mailBox = CAN_Transmit(CAN1,msg);
	while(CAN_TransmitStatus(CAN1,mailBox) !=CAN_TxStatus_Ok && i <= 2000)
	{
		i++;
	}
	if(i >= 2000)
	{
		return 1;
	}
	else 
		return 0;
}

//uint16_t CanRecMsg(uint8_t *buf)
//{
//	uint32_t i;
//	CanRxMsg RxMessage;
//	if(CAN_MessagePending(CAN1,CAN_FIFO0) == 0)return 0;
//	CAN_Receive(CAN1,CAN_FIFO0,&RxMessage);
//	for(i=0;i<8;i++)
//	{
//		buf[i]=RxMessage.Data[i];
//	}
//	return RxMessage.DLC;
//}


