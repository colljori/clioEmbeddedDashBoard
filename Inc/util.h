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

#ifndef __UTIL_H
#define __UTIL_H

/* Exported defines ----------------------------------------------------------*/
#define DBG_PRINTF(...) DbgPrint(__FILE__,__LINE__,__PRETTY_FUNCTION__,__VA_ARGS__)
#define PRINTF(...) LogPrint(__VA_ARGS__)
/* Exported types ------------------------------------------------------------*/
/* Exported functions --------------------------------------------------------*/
void VcomInit(void);

void DbgPrint(const char * file_name,
              int          line_number,
              const char * function_name,
              const char * fmt, ...);

void LogPrint(const char * fmt, ...);

void LedInit(void);
void ToggleLed2(void);
void ResetLed2(void);
void SetLed2(void);


#endif /* __UTIL_H */

/* END OF FILE / [COLLJORI] --------------------------------------------------*/
