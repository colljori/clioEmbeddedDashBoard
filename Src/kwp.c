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
#include "stm32f4xx.h"
#include "time.h"
/* Externs -------------------------------------------------------------------*/
/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private variable ----------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Exported functions --------------------------------------------------------*/

/* --------------------------------------------------------------------------
 * \brief               send a hardcodded frame to init the k-line and wait
                        for response
 * \param [in]          None
 * \param [out]         None
 * -------------------------------------------------------------------------- */
void Kwp_SlowInit(void){
  __attribute__((unused)) __IO uint32_t tmpreg = 0x00U;
  // Activate clock for GPIOA on AHB1
  SET_BIT(RCC->AHB1ENR, RCC_AHB1ENR_GPIOAEN);
  // Delay after an RCC peripheral clock enabling
  tmpreg = READ_BIT(RCC->AHB1ENR, RCC_AHB1ENR_GPIOAEN);
  // GPIOA 9 and 10 in output mode
  SET_BIT(GPIOA->MODER,GPIO_MODER_MODE9_0 | GPIO_MODER_MODE10_0);
  //.................................... send start bit
  CLEAR_BIT(GPIOA->ODR, GPIO_ODR_OD9);
  //systick interrupt happen every 50ms, so 5 baud is 200ms per bit so 4 systick for start
  WaitNSystick(4);

  //.................................... send 0x33
  SET_BIT(GPIOA->ODR, GPIO_ODR_OD9);
  //systick interrupt happen every 50ms, so 5 baud is 200ms per bit so 8 systick for 0b11
  WaitNSystick(8);

  CLEAR_BIT(GPIOA->ODR, GPIO_ODR_OD9);
  //systick interrupt happen every 50ms, so 5 baud is 200ms per bit so 8 systick for 0b00
  WaitNSystick(8);

  SET_BIT(GPIOA->ODR, GPIO_ODR_OD9);
  //systick interrupt happen every 50ms, so 5 baud is 200ms per bit so 8 systick for 0b11
  WaitNSystick(8);

  CLEAR_BIT(GPIOA->ODR, GPIO_ODR_OD9);
  //systick interrupt happen every 50ms, so 5 baud is 200ms per bit so 8 systick for 0b00
  WaitNSystick(8);

  //.................................... send stop  bit
  SET_BIT(GPIOA->ODR, GPIO_ODR_OD9);
  WaitNSystick(4);
}

/* Private functions ---------------------------------------------------------*/

/* --------------------------------------------------------------------------
 * \brief
 * \param [in]          None
 * \param [out]         None
 * -------------------------------------------------------------------------- */

/* END OF FILE / [COLLJORI] --------------------------------------------------*/
