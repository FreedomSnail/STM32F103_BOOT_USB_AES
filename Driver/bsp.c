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
** Created by : 祝定一
** Created date :2012年2月22日16:18:26
** Version :V1.1.0
** Description :
**-----------------------------------------------------------------------------------------------
** Modified by :祝定一
** Modified date :2013年4月21日9:16:19
** Version :
** Description :
************************************************************************************************/
#include "includes.h"
#include "platform_config.h"

/* 头字节包序号，末两字节CRC */
u8 RX_BUFF[1024];

/************************************************************************************************
** Function name :			
** Description :配置48M系统时钟
** RCC配置(使用外部8MHz晶振)，6倍频
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
	/*将外设RCC寄存器重设为缺省值 */   
	RCC_DeInit();
	/*设置外部高速晶振（HSE）*/    
	RCC_HSEConfig(RCC_HSE_ON);   //RCC_HSE_ON——HSE晶振打开(ON)     
	/*等待HSE起振*/    
	HSEStartUpStatus = RCC_WaitForHSEStartUp();     
	if(HSEStartUpStatus == SUCCESS){      //SUCCESS：HSE晶振稳定且就绪 
	/*设置AHB时钟（HCLK）*/       
	RCC_HCLKConfig(RCC_SYSCLK_Div1);  //RCC_SYSCLK_Div1——AHB时钟 = 系统时钟        
	/* 设置高速AHB时钟（PCLK2）*/      
	RCC_PCLK2Config(RCC_HCLK_Div1); //RCC_HCLK_Div1——APB2时钟 = HCLK        
	/*设置低速AHB时钟（PCLK1）*/      
	RCC_PCLK1Config(RCC_HCLK_Div2);   //RCC_HCLK_Div2——APB1时钟 = HCLK /  2       
	/*设置FLASH存储器延时时钟周期数*/      
	FLASH_SetLatency(FLASH_Latency_2);    //FLASH_Latency_2  2延时周期       
	/*选择FLASH预取指缓存的模式*/       
	FLASH_PrefetchBufferCmd(FLASH_PrefetchBuffer_Enable); // 预取指缓存使能       
	/*设置PLL时钟源及倍频系数*/       
	RCC_PLLConfig(RCC_PLLSource_HSE_Div1, RCC_PLLMul_6);  // PLL的输入时钟 = HSE时钟频率；RCC_PLLMul_5——PLL输入时钟x 5         
	/*使能PLL */      
	RCC_PLLCmd(ENABLE);        
	/*检查指定的RCC标志位(PLL准备好标志)设置与否*/        
	while(RCC_GetFlagStatus(RCC_FLAG_PLLRDY) == RESET){        
	}       
	/*设置系统时钟（SYSCLK） */       
	RCC_SYSCLKConfig(RCC_SYSCLKSource_PLLCLK);   //RCC_SYSCLKSource_PLLCLK——选择PLL作为系统时钟       
	/* PLL返回用作系统时钟的时钟源*/      
	while(RCC_GetSYSCLKSource() != 0x08);   //0x08：PLL作为系统时钟         {         }      
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
	gpio_init.GPIO_Pin   = BOOT_DISCONNECT_PIN;//boot usb 启动引脚，这个引脚检测obd口的电压，如果obd接了电源则直接启动app，否则进入boot升级模式
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
	/* 写入0x5555,用于允许狗狗寄存器写入功能 */
	IWDG_WriteAccessCmd(IWDG_WriteAccess_Enable);
	
	/* 狗狗时钟分频,40K/256=156HZ(6.4ms)*/
	IWDG_SetPrescaler(IWDG_Prescaler_256);
	
	/* 喂狗时间 1.5s/6.4MS=234 .注意不能大于0xfff*/
	IWDG_SetReload(234);
	
	/* 喂狗*/
	IWDG_ReloadCounter();
	
	/* 使能狗狗*/
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
** 关闭调试接口，作GPIO使用
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
	/*开usart时钟*/
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1,ENABLE);  
	
	/* 配置USART1 Rx (PA.10) 为输入 */
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	/* 配置 USART1 Tx (PA.09) 为输出 */
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
        /*开DMA时钟*/
  	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE);		
  	/* DMA1 channel5 接收 */
  	DMA_Cmd(DMA1_Channel5, DISABLE);
  	DMA_DeInit(DMA1_Channel5);
	/* USART1_DR_Base串口接收寄存器地址 */
  	DMA_InitStructure.DMA_PeripheralBaseAddr = 0x40013804;           
  	DMA_InitStructure.DMA_MemoryBaseAddr = (uint32_t)&RX_BUFF;
  	DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralSRC;
	/* 此值会随接收长度改变 */
  	DMA_InitStructure.DMA_BufferSize = 5;
	/* 外设地址递增禁止 */					            
  	DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable; 
	/* 内存递增使能 */ 
  	DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;		
	/* 接收数据宽度一字节 */	
  	DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;
	/* 内存数据宽度一字节 */
  	DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;	
	/* 单次模式 */
  	DMA_InitStructure.DMA_Mode = DMA_Mode_Normal;   
	/* 优先级 */                 
  	DMA_InitStructure.DMA_Priority = DMA_Priority_VeryHigh;	
	/* 内存到内存传输禁止 */			
  	DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;
	/* 初始化DMA5 */						
  	DMA_Init(DMA1_Channel5, &DMA_InitStructure); 
	/* 未接收到数据头前禁止DMA接收 */						
  	//USART_DMACmd(USART1, USART_DMAReq_Rx, ENABLE);					
  	DMA_Cmd(DMA1_Channel5, ENABLE);
}

void USART2_config(u32 bound)
{
    //GPIO端口设置
    GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	 
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);	//使能USART1，GPIOA时钟
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2, ENABLE);
	
 	USART_DeInit(USART2);  //复位串口1
	 //USART1_TX   PA.9
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2; //PA.9
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;	//复用推挽输出
    GPIO_Init(GPIOA, &GPIO_InitStructure); //初始化PA9
   
    //USART1_RX	  PA.10
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;//浮空输入
    GPIO_Init(GPIOA, &GPIO_InitStructure);  //初始化PA10


   //USART 初始化设置

	USART_InitStructure.USART_BaudRate = bound;//一般设置为9600;
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;//字长为8位数据格式
	USART_InitStructure.USART_StopBits = USART_StopBits_1;//一个停止位
	USART_InitStructure.USART_Parity = USART_Parity_No;//无奇偶校验位
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//无硬件数据流控制
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;	//收发模式
    USART_Init(USART2, &USART_InitStructure); //初始化串口
	
    NVIC_InitStructure.NVIC_IRQChannel = USART2_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=3 ;//抢占优先级3
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;		//子优先级3
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			//IRQ通道使能
	NVIC_Init(&NVIC_InitStructure);	//根据指定的参数初始化VIC寄存器
    USART_ITConfig(USART2, USART_IT_RXNE, ENABLE);//开启中断
	
    USART_Cmd(USART2, ENABLE);                    //使能串口 
	/* CPU的小缺陷：串口配置好，如果直接Send，则第1个字节发送不出去
	如下语句解决第1个字节无法正确发送出去的问题 */
	USART_ClearFlag(USART2, USART_FLAG_TC);//清发送完成标志位

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

