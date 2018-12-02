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

#include "spi.h"
/* Externs -------------------------------------------------------------------*/
/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/


/* --------------------------------------------------------------------------
 * \brief  Check SPI busy status
 * \param [in]          None
 * \param [out]         None
 * -------------------------------------------------------------------------- */
#define SPI_IS_BUSY(SPIx) (((SPIx)->SR & (SPI_SR_TXE | SPI_SR_RXNE)) == 0 ||\
                           ((SPIx)->SR & SPI_SR_BSY))


/* --------------------------------------------------------------------------
 * \brief  SPI wait till end of the actual transfert
 * \param [in]          None
 * \param [out]         None
 * -------------------------------------------------------------------------- */
#define SPI_WAIT(SPIx)            while (SPI_IS_BUSY(SPIx))


/* Private variable ----------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Exported functions --------------------------------------------------------*/


/* --------------------------------------------------------------------------
 * \brief init SPI1
 * \param [in]          None
 * \param [out]         None
 * -------------------------------------------------------------------------- */
void Spi_Init(void){
  __attribute__((unused)) __IO uint32_t tmpreg = 0x00U;
  //////////////////////ENABLE CLOCK///////////////
  /* Activate clock for GPIOA on AHB1 */
  SET_BIT(RCC->AHB1ENR, RCC_AHB1ENR_GPIOAEN);
  /* Delay after an RCC peripheral clock enabling */
  tmpreg = READ_BIT(RCC->AHB1ENR, RCC_AHB1ENR_GPIOAEN);
  /* Activate clock for SPI1 on APB1 */
  SET_BIT(RCC->APB2ENR, RCC_APB2ENR_SPI1EN);
  /* Delay after an RCC peripheral clock enabling */
  tmpreg = READ_BIT(RCC->APB2ENR, RCC_APB2ENR_SPI1EN);
  //////////////////////CONF OUTPUT///////////////
  // GPIOA 5(SCK), 6(MISO) and 7(MOSI) in AF mode
  MODIFY_REG(GPIOA->MODER,
    GPIO_MODER_MODE7_Msk | GPIO_MODER_MODE6_Msk | GPIO_MODER_MODE5_Msk,
    GPIO_MODER_MODE7_1   | GPIO_MODER_MODE6_1   | GPIO_MODER_MODE5_1);
  // GPIOA 5, 6 and 7 AF mode 5 for SPI1
  MODIFY_REG(GPIOA->AFR[0],
    GPIO_AFRL_AFSEL7_Msk | GPIO_AFRL_AFSEL6_Msk | GPIO_AFRL_AFSEL5_Msk,
    GPIO_AFRL_AFSEL7_2   | GPIO_AFRL_AFSEL6_2   | GPIO_AFRL_AFSEL5_2 |
    GPIO_AFRL_AFSEL7_0   | GPIO_AFRL_AFSEL6_0   | GPIO_AFRL_AFSEL5_0);
  // GPIOA 5, 6 and 7 in full speed
  SET_BIT(GPIOA->OSPEEDR,GPIO_OSPEEDR_OSPEED7_0 | GPIO_OSPEEDR_OSPEED7_1 );
  SET_BIT(GPIOA->OSPEEDR,GPIO_OSPEEDR_OSPEED6_0 | GPIO_OSPEEDR_OSPEED6_1 );
  SET_BIT(GPIOA->OSPEEDR,GPIO_OSPEEDR_OSPEED5_0 | GPIO_OSPEEDR_OSPEED5_1 );
  //////////////////////SPI CR1 ///////////////
  // Select the desired baud rate using the SP1_CR register.
  // SCK at Fpclk/2 = 90/16 = 45MHz
  MODIFY_REG(SPI1->CR1, SPI_CR1_BR_Msk, 0x0);
  // CPOL = 0 : CK to 0 when idle
  // CPHA = 0 : The first clock transition is the first data capture edge
  CLEAR_BIT(SPI1->CR1, SPI_CR1_CPOL | SPI_CR1_CPHA);
// Configure the SSM bit : software management
  SET_BIT(SPI1->CR1, SPI_CR1_SSM);
  SET_BIT(SPI1->CR1, SPI_CR1_SSI);
  SET_BIT(SPI1->CR2, SPI_CR2_SSOE);
  // Configure the MSTR bit : master mode
  SET_BIT(SPI1->CR1, SPI_CR1_MSTR);
  // ENAble error interrupt
  SET_BIT(SPI1->CR2, SPI_CR2_ERRIE );
  NVIC_SetPriority (SPI1_IRQn, (1UL << __NVIC_PRIO_BITS) - 2UL); /* set Priority for Systick Interrupt */
  NVIC_EnableIRQ(SPI1_IRQn);
  // Finally, enable SPI
  SET_BIT(SPI1->CR1, SPI_CR1_SPE);
}


/* --------------------------------------------------------------------------
 * \brief  Sends single byte over SPI
 * \param  *SPIx: Pointer to SPIx peripheral you will use, where x is between 1 to 6
 * \param  data: 8-bit data size to send over SPI
 * \retval Received byte from slave device
 * -------------------------------------------------------------------------- */
__inline__ uint8_t Spi_Send(SPI_TypeDef* SPIx, uint8_t data) {

	/* Wait for previous transmissions to complete if DMA TX enabled for SPI */
	SPI_WAIT(SPIx);

	/* Fill output buffer with data */
	SPIx->DR = data;

	/* Wait for transmission to complete */
	SPI_WAIT(SPIx);

	/* Return data from buffer */
	return SPIx->DR;
}


/* Private functions ---------------------------------------------------------*/

/* --------------------------------------------------------------------------
 * \brief
 * \param [in]          None
 * \param [out]         None
 * -------------------------------------------------------------------------- */

/* END OF FILE / [COLLJORI] --------------------------------------------------*/
