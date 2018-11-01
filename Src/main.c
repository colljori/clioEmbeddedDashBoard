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
  //printf("Boot success\n");

  /* Activate clock for GPIOA on AHB1 */
  __attribute__((unused)) __IO uint32_t tmpreg = 0x00U;
  SET_BIT(RCC->AHB1ENR, RCC_AHB1ENR_GPIOAEN);
  /* Delay after an RCC peripheral clock enabling */
  tmpreg = READ_BIT(RCC->AHB1ENR, RCC_AHB1ENR_GPIOAEN);

  // GPIO in output mode
  SET_BIT(GPIOA->MODER,GPIO_MODER_MODE5_0);

  /* Configure system clock */
  SystemCoreClockUpdate();
  SysTick_Config(SystemCoreClock);

  while(1)
  {
    /* Nothing wait for interrupt */
  }
}
