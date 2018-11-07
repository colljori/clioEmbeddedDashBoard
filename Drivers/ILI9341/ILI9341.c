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
#include "ILI9341.h"
#include "stm32f4xx.h"
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
void ILI9431Init(void){
  __attribute__((unused)) __IO uint32_t tmpreg = 0x00U;
  //////////////////////ENABLE CLOCK///////////////
  /* Activate clock for GPIOA on AHB1 */
  SET_BIT(RCC->AHB1ENR, RCC_AHB1ENR_GPIOAEN);
  /* Activate clock for USART2 on APB1 */
  SET_BIT(RCC->APB2ENR, RCC_APB2ENR_SPI1EN);
  /* Delay after an RCC peripheral clock enabling */
  tmpreg = READ_BIT(RCC->APB2ENR, RCC_APB2ENR_SPI1EN);
  //////////////////////CONF OUTPUT///////////////
  // GPIOA 5, 6 and 7 in AF mode
  MODIFY_REG(GPIOA->MODER,
    GPIO_MODER_MODE7_Msk | GPIO_MODER_MODE6_Msk | GPIO_MODER_MODE5_Msk,
    GPIO_MODER_MODE6_1   | GPIO_MODER_MODE6_1   | GPIO_MODER_MODE5_1);
  // GPIOA 5, 6 and 7 AF mode 5 for SPI1
  MODIFY_REG(GPIOA->AFR[0],
    GPIO_AFRL_AFSEL7_Msk | GPIO_AFRL_AFSEL6_Msk | GPIO_AFRL_AFSEL5_Msk,
    GPIO_AFRL_AFSEL7_2 | GPIO_AFRL_AFSEL7_0 |
    GPIO_AFRL_AFSEL6_2 | GPIO_AFRL_AFSEL6_0 |
    GPIO_AFRL_AFSEL5_2 | GPIO_AFRL_AFSEL5_0);
  // GPIOA 5, 6 and 7 in full speed
  SET_BIT(GPIOA->OSPEEDR,GPIO_OSPEEDR_OSPEED7_0 | GPIO_OSPEEDR_OSPEED7_1 );
  SET_BIT(GPIOA->OSPEEDR,GPIO_OSPEEDR_OSPEED6_0 | GPIO_OSPEEDR_OSPEED6_1 );
  SET_BIT(GPIOA->OSPEEDR,GPIO_OSPEEDR_OSPEED5_0 | GPIO_OSPEEDR_OSPEED5_1 );
  //////////////////////SPI CR1 ///////////////
  // Select the desired baud rate using the SP1_CR register.
  // SCK at Fpclk/16 = 90/16 = 5,625MHz FOR TEST PUSPROSE, TEST WITH 24MHz OR EVEN 48MHz.
  MODIFY_REG(SPI1->CR1, SPI_CR1_BR_Msk, SPI_CR1_BR_1 | SPI_CR1_BR_0);
  // CPOL = 0 : CK to 0 when idle
  // CPHA = 0 : The first clock transition is the first data capture edge
  CLEAR_BIT(SPI1->CR1, SPI_CR1_CPOL | SPI_CR1_CPHA);
  // Configure the MSTR bit : master mode
  SET_BIT(SPI1->CR1, SPI_CR1_MSTR);
}

/* Private functions ---------------------------------------------------------*/

/* --------------------------------------------------------------------------
 * \brief
 * \param [in]          None
 * \param [out]         None
 * -------------------------------------------------------------------------- */

/* END OF FILE / [COLLJORI] --------------------------------------------------*/
