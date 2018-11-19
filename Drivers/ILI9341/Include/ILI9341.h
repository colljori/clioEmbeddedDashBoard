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

#ifndef __ILI9341_H__
#define __ILI9341_H__


/* Exported defines ----------------------------------------------------------*/

/* LCD settings */
#define ILI9341_WIDTH        240
#define ILI9341_HEIGHT       320
#define ILI9341_PIXEL        76800

/* Colors */
#define ILI9341_COLOR_WHITE			0xFFFF
#define ILI9341_COLOR_BLACK			0x0000
#define ILI9341_COLOR_RED       0xF800
#define ILI9341_COLOR_GREEN			0x07E0
#define ILI9341_COLOR_GREEN2		0xB723
#define ILI9341_COLOR_BLUE			0x001F
#define ILI9341_COLOR_BLUE2			0x051D
#define ILI9341_COLOR_YELLOW		0xFFE0
#define ILI9341_COLOR_ORANGE		0xFBE4
#define ILI9341_COLOR_CYAN			0x07FF
#define ILI9341_COLOR_MAGENTA		0xA254
#define ILI9341_COLOR_GRAY			0x7BEF
#define ILI9341_COLOR_BROWN			0xBBCA

#define ILI9341_COLOR_BLUE_POS	0
#define ILI9341_COLOR_GREEN_POS	6
#define ILI9341_COLOR_RED_POS  	12

#define ILI9341_COLOR_BLUE_MSK	ILI9341_COLOR_BLUE
#define ILI9341_COLOR_GREEN_MSK	ILI9341_COLOR_GREEN
#define ILI9341_COLOR_RED_MSK  	ILI9341_COLOR_RED

#define ILI9341_COLOR(_r,_g,_b) ((((_r)&0xF8)<<8)|(((_g)&0xFC)<<3)|((_b)>>3))

/* Exported types ------------------------------------------------------------*/
/* Exported functions --------------------------------------------------------*/
void ILI9431_Init(void);
void ILI9341_DisplayOn(void);
void ILI9341_DisplayOff(void);
void ILI9341_DrawPixel(uint16_t x, uint16_t y, uint32_t color);
void ILI9341_DrawFillRectangle(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, uint32_t color);


#endif /* __ILI9341_H__ */

/* END OF FILE / [COLLJORI] --------------------------------------------------*/
