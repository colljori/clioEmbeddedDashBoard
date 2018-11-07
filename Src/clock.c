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
/* Externs -------------------------------------------------------------------*/
/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private variable ----------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Exported functions --------------------------------------------------------*/

/* --------------------------------------------------------------------------
 * \brief achieve system clock configuration (168MHz)
 * \param [in]          None
 * \param [out]         None
 * -------------------------------------------------------------------------- */
void SystemClockConfig(void){
  /* when VOS[1:0] = 0x11, the maximum value of f(HCLK) is 168 MHz. */
  MODIFY_REG(PWR->CR, PWR_CR_VOS_Msk, PWR_CR_VOS_1 | PWR_CR_VOS_0);
  /* Program the new number of wait states to the LATENCY bits in the FLASH_ACR register */
  WRITE_REG(FLASH->ACR, FLASH_ACR_LATENCY_5WS   /* 6 CPU cycle wait */
                      | FLASH_ACR_PRFTEN        /* enable prefetch */
                      | FLASH_ACR_ICEN          /* instruction cache enable */
                      | FLASH_ACR_DCEN);        /* data cache enable */
  /* Check that the new number of wait states is taken into account to access the Flash
  memory by reading the FLASH_ACR register */
  while((READ_REG(FLASH->ACR) & FLASH_ACR_LATENCY_Msk) != FLASH_ACR_LATENCY_5WS);
  /* Set AHB prescaller to 1 : 168MHz */
  MODIFY_REG(RCC->CFGR,RCC_CFGR_HPRE_Msk, RCC_CFGR_HPRE_DIV1);
  /* Set APB2 prescaller to 2 to not get over 90MHz on this bus (so 84MHz) */
  MODIFY_REG(RCC->CFGR,RCC_CFGR_PPRE2_Msk, RCC_CFGR_PPRE2_DIV2);
  /* Set APB1 prescaller to 4 to not get over 45MHz on this bus (so 42MHz) */
  MODIFY_REG(RCC->CFGR,RCC_CFGR_PPRE1_Msk, RCC_CFGR_PPRE1_DIV4);
  /* PLL configuration for full speed without overrun, 168MHz
     PLL output frequency = input frequency * N / (P*M) */
  MODIFY_REG(RCC->PLLCFGR,RCC_PLLCFGR_PLLM_Msk, 8     << RCC_PLLCFGR_PLLM_Pos);
  MODIFY_REG(RCC->PLLCFGR,RCC_PLLCFGR_PLLN_Msk, 168   << RCC_PLLCFGR_PLLN_Pos);
  MODIFY_REG(RCC->PLLCFGR,RCC_PLLCFGR_PLLP_Msk, 0b00  << RCC_PLLCFGR_PLLP_Pos); /* P => 2 */
  /* Enable PLL */
  SET_BIT(RCC->CR, RCC_CR_PLLON);
  /* Wait for locked PLL */
  while(READ_BIT(RCC->CR, RCC_CR_PLLRDY) != RCC_CR_PLLRDY);
  /* select system clock */
  CLEAR_BIT(RCC->CFGR, RCC_CFGR_SW);
  SET_BIT(RCC->CFGR, RCC_CFGR_SW_PLL);
  /* Wait for SYSCLK to be PPL */
  while(READ_BIT(RCC->CFGR, RCC_CFGR_SW_PLL) != RCC_CFGR_SW_PLL);
  SystemCoreClockUpdate();
  /* set systick and it interruption, load value on 24bit */
  SysTick_Config(SystemCoreClock/20);
}

/* Private functions ---------------------------------------------------------*/

/* --------------------------------------------------------------------------
 * \brief
 * \param [in]          None
 * \param [out]         None
 * -------------------------------------------------------------------------- */

/* END OF FILE / [COLLJORI] --------------------------------------------------*/
