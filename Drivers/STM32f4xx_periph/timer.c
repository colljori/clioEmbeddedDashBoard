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
#include "stm32f4xx.h"

#include "timer.h"
/* Externs -------------------------------------------------------------------*/
/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private variable ----------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Exported functions --------------------------------------------------------*/


/* --------------------------------------------------------------------------
 * \brief init TIMER 9
 * \param [in]          None
 * \param [out]         None
 * -------------------------------------------------------------------------- */
void Timer_Init(void){
  __attribute__((unused)) __IO uint32_t tmpreg = 0x00U;
  //////////////////////ENABLE CLOCK///////////////
  /* Activate clock for TIMER 5 on APB1 */
  SET_BIT(RCC->APB1ENR, RCC_APB1ENR_TIM5EN);
  /* Delay after an RCC peripheral clock enabling */
  tmpreg = READ_BIT(RCC->APB1ENR, RCC_APB1ENR_TIM5EN);
  //////////////////////TIM5 CR1 ///////////////
  // divide the prescaler clock by 4. so finnally PSC 42MHz * 2 / 4 : 84MHz
  // cf clock configuration and TIMPRE value in RCC reg
  MODIFY_REG(TIM5->CR1, TIM_CR1_CKD_Msk,TIM_CR1_CKD_1);
  // prescaler value to 84 so timer clock at 1MHz
  MODIFY_REG(TIM5->PSC, TIM_PSC_PSC_Msk,84);
  // auto reload value, 500 so 1MHz / 500 = 2000Hz
  MODIFY_REG(TIM5->ARR, TIM_ARR_ARR_Msk,500);
  // enable counter
  MODIFY_REG(TIM5->CR1, TIM_CR1_CEN_Msk,TIM_CR1_CEN);
}


/* Private functions ---------------------------------------------------------*/

/* --------------------------------------------------------------------------
 * \brief
 * \param [in]          None
 * \param [out]         None
 * -------------------------------------------------------------------------- */

/* END OF FILE / [COLLJORI] --------------------------------------------------*/
