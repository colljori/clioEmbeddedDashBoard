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

#include <stdint.h>
#include <stdbool.h>
#include <stdio.h>
#include "stm32f4xx.h"
#include "time.h"
#include "it.h"

int main(void)
{
  __attribute__((unused)) __IO uint32_t tmpreg = 0x00U;
  __IO char sendPayload = 0;
  //printf("Boot success\n");

  /* Activate clock for GPIOA on AHB1 */
  SET_BIT(RCC->AHB1ENR, RCC_AHB1ENR_GPIOAEN);
  /* Delay after an RCC peripheral clock enabling */
  tmpreg = READ_BIT(RCC->AHB1ENR, RCC_AHB1ENR_GPIOAEN);

  /* Activate clock for USART2 on APB1 */
  SET_BIT(RCC->APB1ENR, RCC_APB1ENR_USART2EN);
  /* Delay after an RCC peripheral clock enabling */
  tmpreg = READ_BIT(RCC->APB1ENR, RCC_APB1ENR_USART2EN);


  // GPIOA 5 in output mode: LED
  SET_BIT(GPIOA->MODER,GPIO_MODER_MODE5_0);
  // GPIOA 2 and 3 in AF mode
  SET_BIT(GPIOA->MODER,GPIO_MODER_MODE2_1 | GPIO_MODER_MODE3_1);
  // GPIOA 2 and 3 in AF mode 7 for USART2
  SET_BIT(GPIOA->AFR[0],GPIO_AFRL_AFSEL3_0 | GPIO_AFRL_AFSEL3_1 | GPIO_AFRL_AFSEL3_2 );
  SET_BIT(GPIOA->AFR[0],GPIO_AFRL_AFSEL2_0 | GPIO_AFRL_AFSEL2_1 | GPIO_AFRL_AFSEL2_2 );
  // GPIOA 2 and 3 in full speed
  SET_BIT(GPIOA->OSPEEDR,GPIO_OSPEEDR_OSPEED2_0 | GPIO_OSPEEDR_OSPEED2_1 );
  SET_BIT(GPIOA->OSPEEDR,GPIO_OSPEEDR_OSPEED3_0 | GPIO_OSPEEDR_OSPEED3_1 );

  /* Configure system clock */
  SystemCoreClockUpdate();
  SysTick_Config(SystemCoreClock);

//////////////////////CONF USART///////////////:
  // Enable the USART by writing the UE bit in USART_CR1 register to 1.
  SET_BIT(USART2->CR1,USART_CR1_UE);
  // Select the desired baud rate using the USART_BRR register.
  //USART2 BRR 230400 baud, USARTDIV = 4.3125, OVER8=0
  // WRITE_REG(USART2->BRR,0x8B);
  WRITE_REG(USART2->BRR,0x45);
  // Set the TE bit in USART_CR1 to send an idle frame as first transmission.
  SET_BIT(USART2->CR1,USART_CR1_TE);

  CLEAR_BIT(GPIOA->ODR,GPIO_ODR_OD5);

  while(1){
    WRITE_REG(USART2->DR, sendPayload++);
    while(READ_BIT(USART2->SR,USART_SR_TXE) == RESET);
  }

  while(1);
}
