/*
* This file is part of clio embedded dashboard
* Copyright (C) 2018  Joris Collomb
*
* This program is free software; you can redistribute it and/or
* modify it under the terms of the GNU General Public License
* as published by the Free Software Foundation; either version 2
* of the License, or (at your option) any later version.
*
* This program is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
* GNU General Public License for more details.
*
* You should have received a copy of the GNU General Public License
* along with this program; if not, write to the Free Software
* Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
*/


/* Includes ------------------------------------------------------------------*/
#include <stdio.h>
#include <string.h>
#include <stdarg.h>
#include "stm32f4xx.h"
#include "util.h"
/* Externs -------------------------------------------------------------------*/
/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
#define PRINT_MAX_SIZE 200
/* Private variable ----------------------------------------------------------*/
char printBuffer[PRINT_MAX_SIZE];
/* Private function prototypes -----------------------------------------------*/
void VcomSend(char* buffer);
/* Private variables ---------------------------------------------------------*/
/* Exported functions --------------------------------------------------------*/

/* --------------------------------------------------------------------------
 * \brief
 * \param [in]          None
 * \param [out]         None
 * -------------------------------------------------------------------------- */
void LedInit(void){
  __attribute__((unused)) __IO uint32_t tmpreg = 0x00U;
  /* Activate clock for GPIOA on AHB1 */
  SET_BIT(RCC->AHB1ENR, RCC_AHB1ENR_GPIOAEN);
  /* Delay after an RCC peripheral clock enabling */
  tmpreg = READ_BIT(RCC->AHB1ENR, RCC_AHB1ENR_GPIOAEN);
  // GPIOA 5 in output mode: LED
  SET_BIT(GPIOA->MODER,GPIO_MODER_MODE5_0);
}


/* --------------------------------------------------------------------------
 * \brief
 * \param [in]          None
 * \param [out]         None
 * -------------------------------------------------------------------------- */
void ToggleLed2(void){
  GPIOA->ODR ^= GPIO_ODR_OD5;
}


/* --------------------------------------------------------------------------
 * \brief
 * \param [in]          None
 * \param [out]         None
 * -------------------------------------------------------------------------- */
void ResetLed2(void){
  CLEAR_BIT(GPIOA->ODR, GPIO_ODR_OD5);
}


/* --------------------------------------------------------------------------
 * \brief
 * \param [in]          None
 * \param [out]         None
 * -------------------------------------------------------------------------- */
void SetLed2(void){
  SET_BIT(GPIOA->ODR, GPIO_ODR_OD5);
}


/* --------------------------------------------------------------------------
 * \brief
 * \param [in]          None
 * \param [out]         None
 * -------------------------------------------------------------------------- */
void VcomInit(void){
  __attribute__((unused)) __IO uint32_t tmpreg = 0x00U;
  //////////////////////ENABLE CLOCK///////////////
  /* Activate clock for GPIOA on AHB1 */
  SET_BIT(RCC->AHB1ENR, RCC_AHB1ENR_GPIOAEN);
  /* Activate clock for USART2 on APB1 */
  SET_BIT(RCC->APB1ENR, RCC_APB1ENR_USART2EN);
  /* Delay after an RCC peripheral clock enabling */
  tmpreg = READ_BIT(RCC->APB1ENR, RCC_APB1ENR_USART2EN);
  //////////////////////CONF OUTPUT///////////////
  // GPIOA 2 and 3 in AF mode
  SET_BIT(GPIOA->MODER,GPIO_MODER_MODE2_1 | GPIO_MODER_MODE3_1);
  // GPIOA 2 and 3 in AF mode 7 for USART2
  SET_BIT(GPIOA->AFR[0],GPIO_AFRL_AFSEL3_0 | GPIO_AFRL_AFSEL3_1 | GPIO_AFRL_AFSEL3_2 );
  SET_BIT(GPIOA->AFR[0],GPIO_AFRL_AFSEL2_0 | GPIO_AFRL_AFSEL2_1 | GPIO_AFRL_AFSEL2_2 );
  // GPIOA 2 and 3 in full speed
  SET_BIT(GPIOA->OSPEEDR,GPIO_OSPEEDR_OSPEED2_0 | GPIO_OSPEEDR_OSPEED2_1 );
  SET_BIT(GPIOA->OSPEEDR,GPIO_OSPEEDR_OSPEED3_0 | GPIO_OSPEEDR_OSPEED3_1 );
  //////////////////////CONF USART///////////////
  // Enable the USART by writing the UE bit in USART_CR1 register to 1.
  SET_BIT(USART2->CR1,USART_CR1_UE);
  // Select the desired baud rate using the USART_BRR register.
  //USART2 BRR 230400 baud, USARTDIV = 4.3125, OVER8=0
  WRITE_REG(USART2->BRR,0x45);
  // Set the TE bit in USART_CR1 to send an idle frame as first transmission.
  SET_BIT(USART2->CR1,USART_CR1_TE);
}


/* --------------------------------------------------------------------------
 * \brief
 * \param [in]          None
 * \param [out]         None
 * -------------------------------------------------------------------------- */
void LogPrint( const char * fmt, ...){
  va_list argp;
  va_start(argp,fmt);

  vsprintf(printBuffer, fmt, argp);

  VcomSend(printBuffer);
  va_end(argp);
}


/* --------------------------------------------------------------------------
 * \brief
 * \param [in]          None
 * \param [out]         None
 * -------------------------------------------------------------------------- */
void DbgPrint( const char * file_name,
               int          line_number,
               const char * function_name,
               const char * fmt, ...){
  va_list argp;
  va_start(argp,fmt);

  sprintf(printBuffer,"%s:%d (%s) ~ ",file_name,line_number,function_name);
  vsprintf(printBuffer + strlen(printBuffer), fmt, argp);

  VcomSend(printBuffer);
  va_end(argp);
}


/* --------------------------------------------------------------------------
 * \brief
 * \param [in]          None
 * \param [out]         None
 * -------------------------------------------------------------------------- */
void VcomSend(char * buffer){
  for(int i=0; buffer[i]!='\0'; i++){
    WRITE_REG(USART2->DR, buffer[i]);
    while(READ_BIT(USART2->SR,USART_SR_TXE) == RESET);
  }
}


/* Private functions ---------------------------------------------------------*/

/* --------------------------------------------------------------------------
 * \brief
 * \param [in]          None
 * \param [out]         None
 * -------------------------------------------------------------------------- */

/* END OF FILE / [COLLJORI] --------------------------------------------------*/
