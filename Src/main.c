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
/* System */
#include <stdlib.h>
#include "stm32f4xx.h"
/* External peripherals */
#include "ILI9341.h"
/* Utilities */
#include "time.h"
#include "it.h"
#include "clock.h"
#include "util.h"

/* Externs -------------------------------------------------------------------*/
/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private variable ----------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* --------------------------------------------------------------------------
* \brief main function of the project
*
*  it's been call after startup by startup_stm32f446xx
* -------------------------------------------------------------------------- */
int main(void)
{
  SystemClockConfig();
  VcomInit();

  //Clear screen and put cursor to home position
  PRINTF("'\033[2J");
  PRINTF("\033[H\n");

  PRINTF("{{}}#######################{{}}\n\r");
  PRINTF("    CLIO EXTENDED DASHBOARD\n\r");
  PRINTF("{{}}#######################{{}}\n\r\n");

  PRINTF("~ Boot success\n\r");
  PRINTF("~ System clock started (%ldHz)\n\r",SystemCoreClock);

  PRINTF("~ Start ILI9342 initialization...\n\r",SystemCoreClock);
  ILI9431_Init();
  PRINTF("~ Done\n\r");

  ILI9341_FillScreen(ILI9341_COLOR_BLACK);
  ILI9341_DrawFillRectangle(35, 35, 65, 90, ILI9341_COLOR_ORANGE);

  ILI9341_drawChar(12, 12, 'J', ILI9341_COLOR_WHITE);
  ILI9341_drawChar(22, 22, 'o', ILI9341_COLOR_WHITE);
  ILI9341_drawChar(32, 32, 'r', ILI9341_COLOR_WHITE);
  ILI9341_drawChar(42, 42, 'i', ILI9341_COLOR_WHITE);
  ILI9341_drawChar(52, 52, 's', ILI9341_COLOR_WHITE);

  while(1){
  }
}

/* Private functions ---------------------------------------------------------*/

/* --------------------------------------------------------------------------
 * \brief
 * \param [in]          None
 * \param [out]         None
 * -------------------------------------------------------------------------- */

/* END OF FILE / [COLLJORI] --------------------------------------------------*/
