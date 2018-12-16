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
#include <string.h>
#include <stdarg.h>
#include "stm32f4xx.h"
#include "serial.h"
#include "util.h"
/* Externs -------------------------------------------------------------------*/
/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
#define PRINT_MAX_SIZE 200
#define Fir_SAMPLE_NUM         69
#define Fir_COEF_PRECISION     16

/* Private variable ----------------------------------------------------------*/
char printBuffer[PRINT_MAX_SIZE];

/* Private function prototypes -----------------------------------------------*/
void VcomSend(char* buffer);

/* Private variables ---------------------------------------------------------*/
// coeficient of the finite impulse response filter
int32_t Fir_low_pass_coeficients[Fir_SAMPLE_NUM]={
  0,
  0,
  0,
  0,
  0,
  0,
  -1,
  0,
  1,
  4,
  2,
  -6,
  -14,
  -8,
  19,
  44,
  23,
  -54,
  -113,
  -49,
  135,
  253,
  90,
  -305,
  -518,
  -141,
  652,
  1015,
  194,
  -1422,
  -2139,
  -234,
  4276,
  9096,
  11172,
  9096,
  4276,
  -234,
  -2139,
  -1422,
  194,
  1015,
  652,
  -141,
  -518,
  -305,
  90,
  253,
  135,
  -49,
  -113,
  -54,
  23,
  44,
  19,
  -8,
  -14,
  -6,
  2,
  4,
  1,
  0,
  -1,
  0,
  0,
  0,
  0,
  0,
  0
};
// buffer to hold previous sample for the fir filter
uint32_t samples_buffer[Fir_SAMPLE_NUM];

/* Exported functions --------------------------------------------------------*/

/* --------------------------------------------------------------------------
 * \brief finit impulse response filter implementation. add a new sample to
 *        the filter and calculate the output. sample freq:2000Hz
 * \param [in]          None
 * \param [out]         None
 * -------------------------------------------------------------------------- */
uint32_t Fir_AddValue(uint32_t new_value){
  static int32_t Fir_index=0;
  int32_t previous_sample_index = 0;
  uint64_t Fir_result=0;
  //add the new value into the samples buffer
  Fir_index = (Fir_index >= Fir_SAMPLE_NUM) ? 0 : Fir_index + 1;
  samples_buffer[Fir_index]=new_value;
  //calculate the output of the filter
  previous_sample_index = Fir_index;
  for(int i = 0 ; i < Fir_SAMPLE_NUM ; i++){
    previous_sample_index  = (previous_sample_index <= 0) ?
      Fir_SAMPLE_NUM-1 : previous_sample_index-1 ;
    Fir_result += (int64_t)samples_buffer[previous_sample_index] * Fir_low_pass_coeficients[i];
  }
  //go back to a gain of 1
  Fir_result >>= Fir_COEF_PRECISION-1;
  return (uint32_t) Fir_result;
}


/* --------------------------------------------------------------------------
 * \brief
 * \param [in]          None
 * \param [out]         None
 * -------------------------------------------------------------------------- */
void LedInit(void){
  __attribute__((unused)) __IO uint32_t tmpreg = 0x00U;
  /* Activate clock for GPIOA on AHB1 */
  SET_BIT(RCC->AHB1ENR, RCC_AHB1ENR_GPIOAEN);
  /* Delay after an RCC peripheral clock enabling */
  tmpreg = READ_BIT(RCC->AHB1ENR, RCC_AHB1ENR_GPIOAEN);
  // GPIOA 5 in output mode: LED
  SET_BIT(GPIOA->MODER,GPIO_MODER_MODE5_0);
}


/* --------------------------------------------------------------------------
 * \brief
 * \param [in]          None
 * \param [out]         None
 * -------------------------------------------------------------------------- */
void ToggleLed2(void){
  GPIOA->ODR ^= GPIO_ODR_OD5;
}


/* --------------------------------------------------------------------------
 * \brief
 * \param [in]          None
 * \param [out]         None
 * -------------------------------------------------------------------------- */
void ResetLed2(void){
  CLEAR_BIT(GPIOA->ODR, GPIO_ODR_OD5);
}


/* --------------------------------------------------------------------------
 * \brief
 * \param [in]          None
 * \param [out]         None
 * -------------------------------------------------------------------------- */
void SetLed2(void){
  SET_BIT(GPIOA->ODR, GPIO_ODR_OD5);
}


/* --------------------------------------------------------------------------
 * \brief
 * \param [in]          None
 * \param [out]         None
 * -------------------------------------------------------------------------- */
void VcomInit(void){
  Usart2_Init();
}


/* --------------------------------------------------------------------------
 * \brief
 * \param [in]          None
 * \param [out]         None
 * -------------------------------------------------------------------------- */
void LogPrint( const char * fmt, ...){
  va_list argp;
  va_start(argp,fmt);

  vsprintf(printBuffer, fmt, argp);

  VcomSend(printBuffer);
  va_end(argp);
}


/* --------------------------------------------------------------------------
 * \brief
 * \param [in]          None
 * \param [out]         None
 * -------------------------------------------------------------------------- */
void DbgPrint( const char * file_name,
               int          line_number,
               const char * function_name,
               const char * fmt, ...){
  va_list argp;
  va_start(argp,fmt);

  sprintf(printBuffer,"%s:%d (%s) ~ ",file_name,line_number,function_name);
  vsprintf(printBuffer + strlen(printBuffer), fmt, argp);

  VcomSend(printBuffer);
  va_end(argp);
}


/* --------------------------------------------------------------------------
 * \brief
 * \param [in]          None
 * \param [out]         None
 * -------------------------------------------------------------------------- */
void VcomSend(char * buffer){
  Usart_Send(USART2, buffer);
}


/* Private functions ---------------------------------------------------------*/

/* --------------------------------------------------------------------------
 * \brief
 * \param [in]          None
 * \param [out]         None
 * -------------------------------------------------------------------------- */

/* END OF FILE / [COLLJORI] --------------------------------------------------*/
