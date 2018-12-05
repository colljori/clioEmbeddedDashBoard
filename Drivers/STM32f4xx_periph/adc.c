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

#include "adc.h"
/* Externs -------------------------------------------------------------------*/
/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private variable ----------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Exported functions --------------------------------------------------------*/

/* --------------------------------------------------------------------------
 * \brief
 * \param [in]          None
 * \param [out]         None
 * -------------------------------------------------------------------------- */
void Adc_Init(void){
  __attribute__((unused)) __IO uint32_t tmpreg = 0x00U;
  //////////////////////ENABLE CLOCK///////////////
  /* Activate clock for GPIOB on AHB1 */
  SET_BIT(RCC->AHB1ENR, RCC_AHB1ENR_GPIOBEN);
  /* Delay after an RCC peripheral clock enabling */
  tmpreg = READ_BIT(RCC->AHB1ENR, RCC_AHB1ENR_GPIOBEN);
  /* Activate clock for ADC1 on APB2 */
  SET_BIT(RCC->APB2ENR, RCC_APB2ENR_ADC1EN);
  /* Delay after an RCC peripheral clock enabling */
  tmpreg = READ_BIT(RCC->APB2ENR, RCC_APB2ENR_ADC1EN);
  //////////////////////CONF OUTPUT///////////////
  // GPIOB 0(ADC12_IN8) in analog mode
  MODIFY_REG(GPIOB->MODER,
    GPIO_MODER_MODE0_Msk,
    GPIO_MODER_MODE0_1 |
    GPIO_MODER_MODE0_0);
  //////////////////////ADC REG ///////////////////
  // adc on
  SET_BIT(ADC1->CR2, ADC_CR2_ADON);
  // continious conversion
  SET_BIT(ADC1->CR2, ADC_CR2_CONT);
  // ajust sampling time, here 15 cycle to filter a bit and have good reaction time
  MODIFY_REG(ADC1->SMPR1, ADC_SMPR1_SMP10_Msk, ADC_SMPR1_SMP10_0);
  // regular channel sequence description, only one channel read (channel 8 on PB0 ADC12_IN8)
  MODIFY_REG(ADC1->SQR3, ADC_SQR3_SQ1_Msk , ADC_SQR3_SQ1_3);
  // adc start conversion
  SET_BIT(ADC1->CR2, ADC_CR2_SWSTART);
}


/* Private functions ---------------------------------------------------------*/

/* --------------------------------------------------------------------------
 * \brief
 * \param [in]          None
 * \param [out]         None
 * -------------------------------------------------------------------------- */

/* END OF FILE / [COLLJORI] --------------------------------------------------*/
