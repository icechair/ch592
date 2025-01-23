/********************************** (C) COPYRIGHT *******************************
* File Name          : BAT.h
* Author             : WCH
* Version            : V1.0
* Date               : 2021/01/27
* Description        :
*******************************************************************************/



/******************************************************************************/
#ifndef __BAT_H
#define __BAT_H

#ifdef __cplusplus
extern "C"
{
#endif


/**************************************************************************************************
 *                                              MACROS
 **************************************************************************************************/
#define BATT_ADC_LEVEL_2V5       2755       //2.5V
#define BATT_ADC_LEVEL_2V4       2706       //2.4V
#define BATT_ADC_LEVEL_2V3       2658       //2.3V
#define BATT_ADC_LEVEL_1V        2024       //1V

/**************************************************************************************************
 *                                             GLOBAL VARIABLES
 **************************************************************************************************/
//extern uint8 BatVolStartFlg; //电池电压获取开始

/*********************************************************************
 * FUNCTIONS
 */

/*
 * Initilize BAT Service
 */
void HAL_BatVolInit(void);

/*
 *  Get the average value of ADC
 */
uint16_t ADC_GetAverage(uint8_t times);

/*
 * Check whether the battery voltage is too low
 */
uint8_t HAL_BatLowVolCheck(void);


/**************************************************************************************************
**************************************************************************************************/

#ifdef __cplusplus
}
#endif

#endif
