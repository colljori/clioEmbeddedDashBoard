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

#ifndef __GFXFONT_H__
#define __GFXFONT_H__

/* Exported defines ----------------------------------------------------------*/
/* Exported types ------------------------------------------------------------*/


// Font data stored PER GLYPH
typedef struct {
	uint16_t bitmapOffset;     // Pointer into GFXfont->bitmap
	uint8_t  width;            // Bitmap dimensions in pixels
	uint8_t  height;           // Bitmap dimensions in pixels
	uint8_t  xAdvance;         // Distance to advance cursor (x axis)
	int8_t   xOffset;          // X dist from cursor pos to UL corner
	int8_t   yOffset;          // Y dist from cursor pos to UL corner
} GFXglyph;

// Data stored for FONT AS A WHOLE
typedef struct {
	uint8_t  *bitmap;      // Glyph bitmaps, concatenated
	GFXglyph *glyph;       // Glyph array
	uint8_t   first;       // ASCII extents (first char)
	uint8_t   last;        // ASCII extents (last char)
	uint8_t   yAdvance;    // Newline distance (y axis)
} GFXfont;



/* Exported functions --------------------------------------------------------*/


#endif /* __GFXFONT_H__ */

/* END OF FILE / [COLLJORI] --------------------------------------------------*/
