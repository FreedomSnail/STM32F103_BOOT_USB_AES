/****************************************Copyright(c)********************************************
**
**                         2013-2023, Freedom ...
**
**----------------------------------------File Info----------------------------------------------
** File name :main.c
** Latest modified date :
** Latest version :
** Description :
**-----------------------------------------------------------------------------------------------
** Created by : ף��һ
** Created date :2016��1��12��15:50:30
** Version :V1.0
** Description :
**-----------------------------------------------------------------------------------------------
** Modified by :
** Modified date :
** Version :
** Description :
************************************************************************************************/
#include "includes.h"
#include "AES.h"
#include "hw_config.h"
#include "usb_pwr.h"
u8  FLASH_TMP_BUF[IN_FLASH_SECTOR];         /* bin�ļ����棬һ������ */
u8 *P_CHK=(u8*)IN_FLASH_STAR; 		 		/* flash���ָ�� */
user_app Jump_To_Application;		 		/* ��ת����ָ�� */
u32 JumpAddress;							/* ��ת��ַ */
u8  KEY[32]="1234567890ABCDEFGHIJabcdefghijOK";	
/************************************************************************************************
** Function name :			
** Description :
** 
** Input :
** Output :
** Return :
** Others :
** 
************************************************************************************************/
int main(void)
{
	INIT();
	//USB_HW_INIT();
	#if 1
	//��usb_boot��������
	//����ȷ�������ļ�����U�̼������app�������
	if( GPIO_ReadInputDataBit(BOOT_DISCONNECT, BOOT_DISCONNECT_PIN)==0 ) {
		USART_Send_Buf(USART2, "123456\r\n", 8);
	//	USB_HW_INIT();
	} 
	#else
	else {
	    FLASH_Lock();
	    GPIO_ResetBits(USB_DISCONNECT, USB_DISCONNECT_PIN);  
	    RCC_APB1PeriphClockCmd(RCC_APB1Periph_USB,DISABLE);
	    GPIO_DeInit(GPIOA);
	    GPIO_DeInit(GPIOB);
	    GPIO_DeInit(GPIOC);
	    GPIO_DeInit(GPIOD);
	    GPIO_DeInit(GPIOE);
		if (((*(vu32*)IN_FLASH_STAR) & 0x2FFE0000 ) == 0x20000000) {
	             JumpAddress = *(vu32*) (IN_FLASH_STAR+4);
	             Jump_To_Application = (user_app) JumpAddress;
	             __set_MSP(*(vu32*) IN_FLASH_STAR );
	             Jump_To_Application();
	    }     
	    *((u32 *)0xE000ED0C) = 0x05fa0004;     
	}
	#endif
	while(1) {
		if( GPIO_ReadInputDataBit(BOOT_DISCONNECT, BOOT_DISCONNECT_PIN)==0 ) {
			USART_Send_Buf(USART2, "key\r\n", sizeof("key\r\n")-1);
			USB_HW_INIT();
			break;
		}
	}
	while(1);
    return 0;
}

