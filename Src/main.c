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

  uint16_t color=ILI9341_COLOR_BLACK, x, y=0;
  uint16_t red=0, green=0, blue=255;

  while(1){

    for (;   red<255;   red++ , blue-- ){
      color=ILI9341_COLOR(red, green, blue);
      PRINTF("r:%d g:%d b:%d (%.4x)\n\r",red,green,blue,color);
      y++;
      y%=ILI9341_HEIGHT;
      for(x=0; x<ILI9341_WIDTH; x++){
        ILI9341_DrawPixel(x, y, color);
      }
    }
    for (; green<255;  green++, red-- ){
      color=ILI9341_COLOR(red, green, blue);
      PRINTF("r:%d g:%d b:%d (%.4x)\n\r",red,green,blue,color);
      y++;
      y%=ILI9341_HEIGHT;
      for(x=0; x<ILI9341_WIDTH; x++){
        ILI9341_DrawPixel(x, y, color);
      }
    }
    for (;   blue<255; blue++,  green-- ){
      color=ILI9341_COLOR(red, green, blue);
      PRINTF("r:%d g:%d b:%d (%.4x)\n\r",red,green,blue,color);
      y++;
      y%=ILI9341_HEIGHT;
      for(x=0; x<ILI9341_WIDTH; x++){
        ILI9341_DrawPixel(x, y, color);
      }
    }
  }
}

/* Private functions ---------------------------------------------------------*/

/* --------------------------------------------------------------------------
 * \brief
 * \param [in]          None
 * \param [out]         None
 * -------------------------------------------------------------------------- */

/* END OF FILE / [COLLJORI] --------------------------------------------------*/
