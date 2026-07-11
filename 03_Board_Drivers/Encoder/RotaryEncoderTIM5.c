/**
 * ###########################################################################
 * @file     RotaryEncoderTIM5.c
 * @brief    New Vario
 * @author   Max Baetz / Horst Rupp
 * ############################################################################
 */
//
//  ****************************************************************************
//
// Includes
//
#include "General_Includes.h"
#include "RotaryEncoderTIM5.h"
#include "stm32f4xx_hal.h"
#include "stm32f4xx_hal_tim.h"
#include "stm32f4xx.h"

/* Timer handler declaration */
extern TIM_HandleTypeDef    htim5;

/* Timer Output Compare Configuration Structure declaration */
//TIM_OC_InitTypeDef sConfig;

/* Timer Encoder Configuration Structure declaration */
//TIM_Encoder_InitTypeDef sEncoderConfig;


//  ****************************************************************************
//
void RotaryEncoderInitTIM5 ( void )
{
	/* Start the encoder interface */
	HAL_TIM_Encoder_Start(&htim5, TIM_CHANNEL_ALL);
}

//	===================================================================================

uint32_t GetRotaryEncoderCountTIM5 ( void )
{
  return __HAL_TIM_GET_COUNTER ( &htim5 );
}

//	===================================================================================

void  SetRotaryEncoderCountTIM5 ( uint32_t count)
{
  __HAL_TIM_SET_COUNTER ( &htim5, count );
}
//
// =============================================================================
//  The End
// =============================================================================
