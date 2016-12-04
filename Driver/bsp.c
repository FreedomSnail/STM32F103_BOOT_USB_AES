/****************************************Copyright(c)********************************************
**
**                         2013-2023, Freedom ...
**
**----------------------------------------File Info----------------------------------------------
** File name :BSP.c
** Latest modified date :
** Latest version :
** Description :
**-----------------------------------------------------------------------------------------------
** Created by : ף��һ
** Created date :2012��2��22��16:18:26
** Version :V1.1.0
** Description :
**-----------------------------------------------------------------------------------------------
** Modified by :ף��һ
** Modified date :2013��4��21��9:16:19
** Version :
** Description :
************************************************************************************************/
#include "includes.h"
#include "platform_config.h"

/* ͷ�ֽڰ���ţ�ĩ���ֽ�CRC */
u8 RX_BUFF[1024];

/************************************************************************************************
** Function name :			
** Description :����48Mϵͳʱ��
** RCC����(ʹ���ⲿ8MHz����)��6��Ƶ
** Input :
** Output :
** Return :
** Others :
** 
************************************************************************************************/
void RCC_Configuration(void)
{	
	ErrorStatus HSEStartUpStatus;
  	//SystemInit(); 
	/*������RCC�Ĵ�������Ϊȱʡֵ */   
	RCC_DeInit();
	/*�����ⲿ���پ���HSE��*/    
	RCC_HSEConfig(RCC_HSE_ON);   //RCC_HSE_ON����HSE�����(ON)     
	/*�ȴ�HSE����*/    
	HSEStartUpStatus = RCC_WaitForHSEStartUp();     
	if(HSEStartUpStatus == SUCCESS){      //SUCCESS��HSE�����ȶ��Ҿ��� 
	/*����AHBʱ�ӣ�HCLK��*/       
	RCC_HCLKConfig(RCC_SYSCLK_Div1);  //RCC_SYSCLK_Div1����AHBʱ�� = ϵͳʱ��        
	/* ���ø���AHBʱ�ӣ�PCLK2��*/      
	RCC_PCLK2Config(RCC_HCLK_Div1); //RCC_HCLK_Div1����APB2ʱ�� = HCLK        
	/*���õ���AHBʱ�ӣ�PCLK1��*/      
	RCC_PCLK1Config(RCC_HCLK_Div2);   //RCC_HCLK_Div2����APB1ʱ�� = HCLK /  2       
	/*����FLASH�洢����ʱʱ��������*/      
	FLASH_SetLatency(FLASH_Latency_2);    //FLASH_Latency_2  2��ʱ����       
	/*ѡ��FLASHԤȡָ�����ģʽ*/       
	FLASH_PrefetchBufferCmd(FLASH_PrefetchBuffer_Enable); // Ԥȡָ����ʹ��       
	/*����PLLʱ��Դ����Ƶϵ��*/       
	RCC_PLLConfig(RCC_PLLSource_HSE_Div1, RCC_PLLMul_6);  // PLL������ʱ�� = HSEʱ��Ƶ�ʣ�RCC_PLLMul_5����PLL����ʱ��x 5         
	/*ʹ��PLL */      
	RCC_PLLCmd(ENABLE);        
	/*���ָ����RCC��־λ(PLL׼���ñ�־)�������*/        
	while(RCC_GetFlagStatus(RCC_FLAG_PLLRDY) == RESET){        
	}       
	/*����ϵͳʱ�ӣ�SYSCLK�� */       
	RCC_SYSCLKConfig(RCC_SYSCLKSource_PLLCLK);   //RCC_SYSCLKSource_PLLCLK����ѡ��PLL��Ϊϵͳʱ��       
	/* PLL��������ϵͳʱ�ӵ�ʱ��Դ*/      
	while(RCC_GetSYSCLKSource() != 0x08);   //0x08��PLL��Ϊϵͳʱ��         {         }      
	}     
}
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
void GPIO_Configuration(void)
{
	#if 1
	GPIO_InitTypeDef  gpio_init;
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
				   
	gpio_init.GPIO_Pin   = GPIO_Pin_10;	  //LED
	gpio_init.GPIO_Speed = GPIO_Speed_50MHz;
	gpio_init.GPIO_Mode  = GPIO_Mode_Out_PP;
	GPIO_Init(GPIOB, &gpio_init); 

	RCC_APB2PeriphClockCmd(BOOT_GPIO_CLK, ENABLE);
	gpio_init.GPIO_Pin   = BOOT_DISCONNECT_PIN;//boot usb �������ţ�������ż��obd�ڵĵ�ѹ�����obd���˵�Դ��ֱ������app���������boot����ģʽ
	gpio_init.GPIO_Speed = GPIO_Speed_50MHz;
	gpio_init.GPIO_Mode  = GPIO_Mode_IPU;
	GPIO_Init(BOOT_DISCONNECT, &gpio_init);
	#endif
}
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
void IWDG_Configuration(void)
{
	/* д��0x5555,�����������Ĵ���д�빦�� */
	IWDG_WriteAccessCmd(IWDG_WriteAccess_Enable);
	
	/* ����ʱ�ӷ�Ƶ,40K/256=156HZ(6.4ms)*/
	IWDG_SetPrescaler(IWDG_Prescaler_256);
	
	/* ι��ʱ�� 1.5s/6.4MS=234 .ע�ⲻ�ܴ���0xfff*/
	IWDG_SetReload(234);
	
	/* ι��*/
	IWDG_ReloadCounter();
	
	/* ʹ�ܹ���*/
//	IWDG_Enable();
}
/************************************************************************************************
** Function name :			
** Description :
** 
** Input :
** Output :
** Return :
** Others :
** �رյ��Խӿڣ���GPIOʹ��
************************************************************************************************/
void UnableJTAG(void)
{
   RCC->APB2ENR |= RCC_APB2ENR_AFIOEN;                     // enable clock for Alternate Function
   AFIO->MAPR &= ~(7UL<<24); // clear used bit
   AFIO->MAPR |= (4UL<<24); // set used bits
}
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
void USART1_config(u32 bound)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	/*��usartʱ��*/
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1,ENABLE);  
	
	/* ����USART1 Rx (PA.10) Ϊ���� */
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	/* ���� USART1 Tx (PA.09) Ϊ��� */
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	USART_InitStructure.USART_BaudRate = bound;
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;
	USART_InitStructure.USART_StopBits = USART_StopBits_1;
	USART_InitStructure.USART_Parity = USART_Parity_No ;
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
	USART_Init(USART1,&USART_InitStructure); 
		
	USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);
	USART_Cmd(USART1, ENABLE);  

} 
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
void Usart_DMA_config(void)
{

  	DMA_InitTypeDef DMA_InitStructure;
        /*��DMAʱ��*/
  	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE);		
  	/* DMA1 channel5 ���� */
  	DMA_Cmd(DMA1_Channel5, DISABLE);
  	DMA_DeInit(DMA1_Channel5);
	/* USART1_DR_Base���ڽ��ռĴ�����ַ */
  	DMA_InitStructure.DMA_PeripheralBaseAddr = 0x40013804;           
  	DMA_InitStructure.DMA_MemoryBaseAddr = (uint32_t)&RX_BUFF;
  	DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralSRC;
	/* ��ֵ������ճ��ȸı� */
  	DMA_InitStructure.DMA_BufferSize = 5;
	/* �����ַ������ֹ */					            
  	DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable; 
	/* �ڴ����ʹ�� */ 
  	DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;		
	/* �������ݿ��һ�ֽ� */	
  	DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;
	/* �ڴ����ݿ��һ�ֽ� */
  	DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;	
	/* ����ģʽ */
  	DMA_InitStructure.DMA_Mode = DMA_Mode_Normal;   
	/* ���ȼ� */                 
  	DMA_InitStructure.DMA_Priority = DMA_Priority_VeryHigh;	
	/* �ڴ浽�ڴ洫���ֹ */			
  	DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;
	/* ��ʼ��DMA5 */						
  	DMA_Init(DMA1_Channel5, &DMA_InitStructure); 
	/* δ���յ�����ͷǰ��ֹDMA���� */						
  	//USART_DMACmd(USART1, USART_DMAReq_Rx, ENABLE);					
  	DMA_Cmd(DMA1_Channel5, ENABLE);
}

void USART2_config(u32 bound)
{
    //GPIO�˿�����
    GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	 
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);	//ʹ��USART1��GPIOAʱ��
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2, ENABLE);
	
 	USART_DeInit(USART2);  //��λ����1
	 //USART1_TX   PA.9
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2; //PA.9
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;	//�����������
    GPIO_Init(GPIOA, &GPIO_InitStructure); //��ʼ��PA9
   
    //USART1_RX	  PA.10
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;//��������
    GPIO_Init(GPIOA, &GPIO_InitStructure);  //��ʼ��PA10


   //USART ��ʼ������

	USART_InitStructure.USART_BaudRate = bound;//һ������Ϊ9600;
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;//�ֳ�Ϊ8λ���ݸ�ʽ
	USART_InitStructure.USART_StopBits = USART_StopBits_1;//һ��ֹͣλ
	USART_InitStructure.USART_Parity = USART_Parity_No;//����żУ��λ
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//��Ӳ������������
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;	//�շ�ģʽ
    USART_Init(USART2, &USART_InitStructure); //��ʼ������
	
    NVIC_InitStructure.NVIC_IRQChannel = USART2_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=3 ;//��ռ���ȼ�3
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;		//�����ȼ�3
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			//IRQͨ��ʹ��
	NVIC_Init(&NVIC_InitStructure);	//����ָ���Ĳ�����ʼ��VIC�Ĵ���
    USART_ITConfig(USART2, USART_IT_RXNE, ENABLE);//�����ж�
	
    USART_Cmd(USART2, ENABLE);                    //ʹ�ܴ��� 
	/* CPU��Сȱ�ݣ��������úã����ֱ��Send�����1���ֽڷ��Ͳ���ȥ
	�����������1���ֽ��޷���ȷ���ͳ�ȥ������ */
	USART_ClearFlag(USART2, USART_FLAG_TC);//�巢����ɱ�־λ

}
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
void INIT(void)
{
	 SystemInit();
	 RCC_APB2PeriphClockCmd( RCC_APB2Periph_GPIOA 
                              |RCC_APB2Periph_GPIOB | RCC_APB2Periph_GPIOC
                              |RCC_APB2Periph_GPIOD | RCC_APB2Periph_AFIO, ENABLE );  
                                                                                                                                                    						                                                    
	 NVIC_Configuration();
	 GPIO_Configuration();
	 USART2_config(57600);
	 //USART2_Config(USART2, 57600);
}

