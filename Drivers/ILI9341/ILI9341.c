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
#include "ILI9341.h"
#include "stm32f4xx.h"

#include "spi.h"

#include "util.h"
#include "time.h"
#include "gfxfont.h"
#include "fonts.h"
/* Externs -------------------------------------------------------------------*/
/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
//ILI9341 specific pin definition
#define ILI9341_DC_PIN        GPIO_ODR_OD8
#define ILI9341_DC_PORT       GPIOA

#define ILI9341_RESET_PIN     GPIO_ODR_OD10
#define ILI9341_RESET_PORT    GPIOB

#define ILI9341_CS_PIN        GPIO_ODR_OD6
#define ILI9341_CS_PORT       GPIOB

//ILI9341 SPI definition
#define ILI9341_SPI           SPI1

//ILI9341 specific pin control
#define ILI9341_CS_RESET     CLEAR_BIT(ILI9341_CS_PORT->ODR, ILI9341_CS_PIN)
#define ILI9341_CS_SET       SET_BIT(  ILI9341_CS_PORT->ODR, ILI9341_CS_PIN)

#define ILI9341_RESET_RESET  CLEAR_BIT(ILI9341_RESET_PORT->ODR, ILI9341_RESET_PIN)
#define ILI9341_RESET_SET    SET_BIT(  ILI9341_RESET_PORT->ODR, ILI9341_RESET_PIN)

#define ILI9341_DC_RESET     CLEAR_BIT(ILI9341_DC_PORT->ODR, ILI9341_DC_PIN)
#define ILI9341_DC_SET       SET_BIT(  ILI9341_DC_PORT->ODR, ILI9341_DC_PIN)

//ILI9341 command code

#define ILI9341_CMD_NOP             0x00
#define ILI9341_CMD_SWRESET         0x01
#define ILI9341_CMD_RDDID           0x04
#define ILI9341_CMD_RDDST           0x09

#define ILI9341_CMD_SLPIN           0x10
#define ILI9341_CMD_SLPOUT          0x11
#define ILI9341_CMD_PTLON           0x12
#define ILI9341_CMD_NORON           0x13

#define ILI9341_CMD_RDMODE          0x0A
#define ILI9341_CMD_RDMADCTL        0x0B
#define ILI9341_CMD_RDPIXFMT        0x0C
#define ILI9341_CMD_RDIMGFMT        0x0D
#define ILI9341_CMD_RDSELFDIAG      0x0F

#define ILI9341_CMD_INVOFF          0x20
#define ILI9341_CMD_INVON           0x21
#define ILI9341_CMD_GAMMASET        0x26
#define ILI9341_CMD_DISPOFF         0x28
#define ILI9341_CMD_DISPON          0x29

#define ILI9341_CMD_CASET           0x2A
#define ILI9341_CMD_PASET           0x2B
#define ILI9341_CMD_RAMWR           0x2C
#define ILI9341_CMD_RAMRD           0x2E

#define ILI9341_CMD_PTLAR           0x30
#define ILI9341_CMD_MADCTL          0x36
#define ILI9341_CMD_VSCRSADD        0x37
#define ILI9341_CMD_PIXFMT          0x3A
#define ILI9341_CMD_RAMWR_CONT      0x3C

#define ILI9341_CMD_FRMCTR1         0xB1
#define ILI9341_CMD_FRMCTR2         0xB2
#define ILI9341_CMD_FRMCTR3         0xB3
#define ILI9341_CMD_INVCTR          0xB4
#define ILI9341_CMD_DFUNCTR         0xB6

#define ILI9341_CMD_PWCTR1          0xC0
#define ILI9341_CMD_PWCTR2          0xC1
#define ILI9341_CMD_PWCTR3          0xC2
#define ILI9341_CMD_PWCTR4          0xC3
#define ILI9341_CMD_PWCTR5          0xC4
#define ILI9341_CMD_VMCTR1          0xC5
#define ILI9341_CMD_VMCTR2          0xC7

#define ILI9341_CMD_POWERA 	        0xCB
#define ILI9341_CMD_POWERB 	        0xCF

#define ILI9341_CMD_RDID1           0xDA
#define ILI9341_CMD_RDID2           0xDB
#define ILI9341_CMD_RDID3           0xDC
#define ILI9341_CMD_RDID4           0xDD

#define ILI9341_CMD_GMCTRP1         0xE0
#define ILI9341_CMD_GMCTRN1         0xE1
#define ILI9341_CMD_DTCA				    0xE8
#define ILI9341_CMD_DTCB				    0xEA
#define ILI9341_CMD_POWER_SEQ			  0xED

#define ILI9341_CMD_3GAMMA_EN			  0xF2
#define ILI9341_CMD_PRC					    0xF7

/* Private variable ----------------------------------------------------------*/
const GFXfont* gfxFont = &FreeSans9pt7b;
ILI9341_Orientation_T orientation = PORTRAIT;

/* Private function prototypes -----------------------------------------------*/
void ILI9341_InitLCD(void);
void ILI9341_SendCommand(uint8_t data);
void ILI9341_SendData(uint8_t data);
void ILI9341_SetCursorPosition(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2);
/* Private variables ---------------------------------------------------------*/
/* Exported functions --------------------------------------------------------*/

//########################CONTROL
/* --------------------------------------------------------------------------
 * \brief
 * \param [in]          None
 * \param [out]         None
 * -------------------------------------------------------------------------- */
void ILI9431_Init(void){
  __attribute__((unused)) __IO uint32_t tmpreg = 0x00U;
  //////////////////////ENABLE CLOCK///////////////
  /* Activate clock for GPIOA on AHB1 */
  SET_BIT(RCC->AHB1ENR, RCC_AHB1ENR_GPIOAEN);
  /* Activate clock for GPIOB on AHB1 */
  SET_BIT(RCC->AHB1ENR, RCC_AHB1ENR_GPIOBEN);
  /* Delay after an RCC peripheral clock enabling */
  tmpreg = READ_BIT(RCC->AHB1ENR, RCC_AHB1ENR_GPIOBEN | RCC_AHB1ENR_GPIOAEN);
  //////////////////////CONF OUTPUT///////////////
  // GPIOB 6(CS) in output mode
  MODIFY_REG(GPIOB->MODER, GPIO_MODER_MODE6_Msk,
                           GPIO_MODER_MODE6_0);
  // GPIOA 8(DC) in output mode
  MODIFY_REG(GPIOA->MODER, GPIO_MODER_MODE8_Msk,
                           GPIO_MODER_MODE8_0);
  // GPIOA 9(RESET) in output mode
  MODIFY_REG(GPIOB->MODER, GPIO_MODER_MODE10_Msk,
                           GPIO_MODER_MODE10_0);
  //////////////////////CONF SPI/...//////////////
  // Init the SPI for communication with ILI9341
  Spi_Init();
  // Initially chip in commande mode
  ILI9341_DC_SET;
  // Initially chip select to high level
  ILI9341_CS_SET;
  // start the init procedure of the ILI9341
  ILI9341_InitLCD();
}


/* --------------------------------------------------------------------------
 * \brief
 * \param [in]          None
 * \param [out]         None
 * -------------------------------------------------------------------------- */
void ILI9341_DisplayOn(void) {
	ILI9341_SendCommand(ILI9341_CMD_DISPON);
}


/* --------------------------------------------------------------------------
 * \brief
 * \param [in]          None
 * \param [out]         None
 * -------------------------------------------------------------------------- */
void ILI9341_DisplayOff(void) {
	ILI9341_SendCommand(ILI9341_CMD_DISPOFF);
}


/* --------------------------------------------------------------------------
 * \brief
 * \param [in]          None
 * \param [out]         None
 * -------------------------------------------------------------------------- */
void ILI9341_SetOrientation(ILI9341_Orientation_T new_orientation) {
  orientation = new_orientation;
}


//########################DRAW

/* --------------------------------------------------------------------------
 *  \brief   Print on ILI9341 screen
 *   \param    x   Bottom left corner x coordinate
 *   \param    y   Bottom left corner y coordinate
 *   \param    String string of char to print
 *   \param    color 16-bit 5-6-5 Color to draw chraracter with
 *   \param    size  Font magnification level, 1 is 'original' size
 * -------------------------------------------------------------------------- */
void ILI9341_Print(uint16_t x, uint16_t y, char* string, uint32_t color, uint8_t size) {
  char c;
  for(int i=0 ; (c = string[i]) !='\0' ; i++){
    c -=                  gfxFont->first;
    GFXglyph *glyph   = &(gfxFont->glyph[(uint32_t)c]);
    uint8_t  *bitmap  =   gfxFont->bitmap;

    uint16_t bitmapOffset = glyph->bitmapOffset;
    uint8_t  width        = glyph->width,
             height       = glyph->height,
             xAdvance     = glyph->xAdvance;
    int8_t   xOffset      = glyph->xOffset,
             yOffset      = glyph->yOffset;
    uint8_t  xx, yy, bits = 0,
                      bit = 0;
    int16_t     xOffset16 = 0,
                yOffset16 = 0;
    // avoid overflow when size magnification of char
    if(size > 1) {
      xOffset16 = xOffset;
      yOffset16 = yOffset;
    }
    // NOTE: THERE IS NO 'BACKGROUND' COLOR OPTION ON CUSTOM FONTS.
    // THIS IS ON PURPOSE AND BY DESIGN.  The background color feature
    // has typically been used with the 'classic' font to overwrite old
    // screen contents with new data.  This ONLY works because the
    // characters are a uniform size; it's not a sensible thing to do with
    // proportionally-spaced fonts with glyphs of varying sizes (and that
    // may overlap).
    for(yy=0; yy<height; yy++) {
      for(xx=0; xx<width; xx++) {
        if(!(bit++ & 7)) {
          bits = bitmap[bitmapOffset++];
        }
        if(bits & 0x80) {
          if(size == 1) {
            ILI9341_DrawPixel(x+xOffset+xx, y+yOffset+yy, color);
          } else {
            ILI9341_DrawFillRectangle(x+(xOffset16+xx)*size, y+(yOffset16+yy)*size,
            x+(xOffset16+xx+1)*size, y+(yOffset16+yy+1)*size, color);
          }
        }
        bits <<= 1;
      }
    }
    DBG_PRINTF("width:%d height:%d\n\r",width, height);
    x+=(xAdvance)*size;
  }
}


/* --------------------------------------------------------------------------
 * \brief
 * \param [in]          None
 * \param [out]         None
 * -------------------------------------------------------------------------- */
void ILI9341_DrawPixel(uint16_t x, uint16_t y, uint32_t color) {
  int16_t temp;

  switch(orientation) {
    case PORTRAIT:
    break;
    case LANDSCAPE:
    temp = x;
    x = ILI9341_WIDTH  - 1 - y;
    y = temp;
    break;
    case PORTRAIT_REVERSE:
    x = ILI9341_WIDTH  - 1 - x;
    y = ILI9341_HEIGHT - 1 - y;
    break;
    case LANDSCAPE_REVERSE:
    temp = x;
    x = y;
    y = ILI9341_HEIGHT - 1 - temp;
    break;
  }

	ILI9341_SetCursorPosition(x, y, x , y);

	ILI9341_SendCommand(ILI9341_CMD_RAMWR);
	ILI9341_SendData(color >> 8);
	ILI9341_SendData(color & 0xFF);
}


/* --------------------------------------------------------------------------
 * \brief
 * \param [in]          None
 * \param [out]         None
 * -------------------------------------------------------------------------- */
void ILI9341_DrawFillRectangle(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, uint32_t color) {
  uint32_t nb_pixel = ((x2-x1)+1)*((y2-y1)+1)-1;
  DBG_PRINTF("nb_pixel:%d\n\r",nb_pixel);
	ILI9341_SetCursorPosition(x1, y1, x2, y2);
	ILI9341_SendCommand(ILI9341_CMD_RAMWR);
  for(int i=0 ; i <nb_pixel; i++){
  ILI9341_SendData(color >> 8);
    ILI9341_SendData(color & 0xFF);
  }
}


/* --------------------------------------------------------------------------
 * \brief
 * \param [in]          None
 * \param [out]         None
 * -------------------------------------------------------------------------- */
void ILI9341_FillScreen(uint32_t color) {
  ILI9341_DrawFillRectangle(0, 0, ILI9341_WIDTH, ILI9341_HEIGHT, color);
}


/* Private functions ---------------------------------------------------------*/


/* --------------------------------------------------------------------------
 * \brief
 * \param [in]          None
 * \param [out]         None
 * -------------------------------------------------------------------------- */
void ILI9341_SetCursorPosition(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2) {
	ILI9341_SendCommand(ILI9341_CMD_CASET);
	ILI9341_SendData(x1 >> 8);
	ILI9341_SendData(x1 & 0xFF);
	ILI9341_SendData(x2 >> 8);
	ILI9341_SendData(x2 & 0xFF);

	ILI9341_SendCommand(ILI9341_CMD_PASET);
	ILI9341_SendData(y1 >> 8);
	ILI9341_SendData(y1 & 0xFF);
	ILI9341_SendData(y2 >> 8);
	ILI9341_SendData(y2 & 0xFF);
}


/* --------------------------------------------------------------------------
 * \brief
 * \param [in]          None
 * \param [out]         None
 * -------------------------------------------------------------------------- */
void ILI9341_SendCommand(uint8_t data) {
	ILI9341_DC_RESET;
	ILI9341_CS_RESET;
	Spi_Send(ILI9341_SPI, data);
	ILI9341_CS_SET;
}


/* --------------------------------------------------------------------------
 * \brief
 * \param [in]          None
 * \param [out]         None
 * -------------------------------------------------------------------------- */
void ILI9341_SendData(uint8_t data) {
	ILI9341_DC_SET;
	ILI9341_CS_RESET;
	Spi_Send(ILI9341_SPI, data);
	ILI9341_CS_SET;
}


/* --------------------------------------------------------------------------
 * \brief
 * \param [in]          None
 * \param [out]         None
 * -------------------------------------------------------------------------- */
void ILI9341_InitLCD(void) {
	/* Force reset */
	ILI9341_RESET_RESET ;
	Delay(20);
  ILI9341_RESET_SET;

	/* Delay for RST response */
	Delay(2);

	/* Software reset */
	ILI9341_SendCommand(ILI9341_CMD_SWRESET);
	Delay(5);
  /* Init procedure of ILI9341 */
	ILI9341_SendCommand(ILI9341_CMD_POWERA);
	ILI9341_SendData(0x39);
	ILI9341_SendData(0x2C);
	ILI9341_SendData(0x00);
	ILI9341_SendData(0x34);
	ILI9341_SendData(0x02);
	ILI9341_SendCommand(ILI9341_CMD_POWERB);
	ILI9341_SendData(0x00);
	ILI9341_SendData(0xC1);
	ILI9341_SendData(0x30);
	ILI9341_SendCommand(ILI9341_CMD_DTCA);
	ILI9341_SendData(0x85);
	ILI9341_SendData(0x00);
	ILI9341_SendData(0x78);
	ILI9341_SendCommand(ILI9341_CMD_DTCB);
	ILI9341_SendData(0x00);
	ILI9341_SendData(0x00);
	ILI9341_SendCommand(ILI9341_CMD_POWER_SEQ);
	ILI9341_SendData(0x64);
	ILI9341_SendData(0x03);
	ILI9341_SendData(0x12);
	ILI9341_SendData(0x81);
	ILI9341_SendCommand(ILI9341_CMD_PRC);
	ILI9341_SendData(0x20);
	ILI9341_SendCommand(ILI9341_CMD_PWCTR1);
	ILI9341_SendData(0x23);
	ILI9341_SendCommand(ILI9341_CMD_PWCTR2);
	ILI9341_SendData(0x10);
	ILI9341_SendCommand(ILI9341_CMD_VMCTR1);
	ILI9341_SendData(0x3E);
	ILI9341_SendData(0x28);
	ILI9341_SendCommand(ILI9341_CMD_VMCTR2);
	ILI9341_SendData(0x86);
	ILI9341_SendCommand(ILI9341_CMD_MADCTL);
	ILI9341_SendData(0x48);
	ILI9341_SendCommand(ILI9341_CMD_PIXFMT);
	ILI9341_SendData(0x55);
	ILI9341_SendCommand(ILI9341_CMD_FRMCTR1);
	ILI9341_SendData(0x00);
	ILI9341_SendData(0x18);
	ILI9341_SendCommand(ILI9341_CMD_DFUNCTR);
	ILI9341_SendData(0x08);
	ILI9341_SendData(0x82);
	ILI9341_SendData(0x27);
	ILI9341_SendCommand(ILI9341_CMD_3GAMMA_EN);
	ILI9341_SendData(0x00);
	ILI9341_SendCommand(ILI9341_CMD_CASET);
	ILI9341_SendData(0x00);
	ILI9341_SendData(0x00);
	ILI9341_SendData(0x00);
	ILI9341_SendData(0xEF);
	ILI9341_SendCommand(ILI9341_CMD_PASET);
	ILI9341_SendData(0x00);
	ILI9341_SendData(0x00);
	ILI9341_SendData(0x01);
	ILI9341_SendData(0x3F);
	ILI9341_SendCommand(ILI9341_CMD_GAMMASET);
	ILI9341_SendData(0x01);
	ILI9341_SendCommand(ILI9341_CMD_GMCTRP1);
	ILI9341_SendData(0x0F);
	ILI9341_SendData(0x31);
	ILI9341_SendData(0x2B);
	ILI9341_SendData(0x0C);
	ILI9341_SendData(0x0E);
	ILI9341_SendData(0x08);
	ILI9341_SendData(0x4E);
	ILI9341_SendData(0xF1);
	ILI9341_SendData(0x37);
	ILI9341_SendData(0x07);
	ILI9341_SendData(0x10);
	ILI9341_SendData(0x03);
	ILI9341_SendData(0x0E);
	ILI9341_SendData(0x09);
	ILI9341_SendData(0x00);
	ILI9341_SendCommand(ILI9341_CMD_GMCTRN1);
	ILI9341_SendData(0x00);
	ILI9341_SendData(0x0E);
	ILI9341_SendData(0x14);
	ILI9341_SendData(0x03);
	ILI9341_SendData(0x11);
	ILI9341_SendData(0x07);
	ILI9341_SendData(0x31);
	ILI9341_SendData(0xC1);
	ILI9341_SendData(0x48);
	ILI9341_SendData(0x08);
	ILI9341_SendData(0x0F);
	ILI9341_SendData(0x0C);
	ILI9341_SendData(0x31);
	ILI9341_SendData(0x36);
	ILI9341_SendData(0x0F);
	ILI9341_SendCommand(ILI9341_CMD_SLPOUT);

	Delay(100);

	ILI9341_SendCommand(ILI9341_CMD_DISPON);
	ILI9341_SendCommand(ILI9341_CMD_RAMWR);
}

/* --------------------------------------------------------------------------
 * \brief
 * \param [in]          None
 * \param [out]         None
 * -------------------------------------------------------------------------- */

/* END OF FILE / [COLLJORI] --------------------------------------------------*/
