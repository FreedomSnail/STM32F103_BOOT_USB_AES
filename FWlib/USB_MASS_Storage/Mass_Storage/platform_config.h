/**
  ******************************************************************************
  * @file    platform_config.h
  * @author  MCD Application Team
  * @version V4.0.0
  * @date    21-January-2013
  * @brief   Evaluation board specific configuration file.
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; COPYRIGHT 2013 STMicroelectronics</center></h2>
  *
  * Licensed under MCD-ST Liberty SW License Agreement V2, (the "License");
  * You may not use this file except in compliance with the License.
  * You may obtain a copy of the License at:
  *
  *        http://www.st.com/software_license_agreement_liberty_v2
  *
  * Unless required by applicable law or agreed to in writing, software 
  * distributed under the License is distributed on an "AS IS" BASIS, 
  * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
  * See the License for the specific language governing permissions and
  * limitations under the License.
  *
  ******************************************************************************
  */


/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __PLATFORM_CONFIG_H
#define __PLATFORM_CONFIG_H

/* Includes ------------------------------------------------------------------*/

#include "stm32f10x_gpio.h"
#include "stm32f10x_rcc.h"
#include "stm32f10x_usart.h"
#include "stm32f10x_exti.h"
#include "stm32f10x_pwr.h"

/*Unique Devices IDs register set*/
#define         ID1          (0x1FFFF7E8)
#define         ID2          (0x1FFFF7EC)
#define         ID3          (0x1FFFF7F0)

#define	 IN_FLASH_STAR      		0x08005000	/* 内部flash，用户程序起始地址 */
#define	 IN_FLASH_END      			0x08040000	/* 内部flash，flash最大值 256kb stm32f103rc */
#define  IN_FLASH_SECTOR                2048            /* 内部flash 2k一个扇区，一次要擦除一个扇区单位*/
#define  FLASH_WAIT_TIMEOUT             100000
 
typedef  void (*user_app)(void);    
extern u8  FLASH_TMP_BUF[];                             /* bin文件缓存，一个扇区 */					/* 跳转地址 */
extern u8  KEY[];	

/* Define the STM32F10x hardware depending on the used evaluation board */
#define USB_DISCONNECT                      GPIOB
#define USB_DISCONNECT_PIN                  GPIO_Pin_15
#define RCC_APB2Periph_GPIO_DISCONNECT      RCC_APB2Periph_GPIOB
#if 0
#define BOOT_DISCONNECT                      GPIOA
#define BOOT_DISCONNECT_PIN                  GPIO_Pin_0
#define BOOT_GPIO_CLK                        RCC_APB2Periph_GPIOA
#else
#define BOOT_DISCONNECT                      GPIOA
#define BOOT_DISCONNECT_PIN                  GPIO_Pin_15
#define BOOT_GPIO_CLK                        RCC_APB2Periph_GPIOA
#endif
/* Exported macro ------------------------------------------------------------*/
/* Exported functions ------------------------------------------------------- */

#endif /* __PLATFORM_CONFIG_H */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
