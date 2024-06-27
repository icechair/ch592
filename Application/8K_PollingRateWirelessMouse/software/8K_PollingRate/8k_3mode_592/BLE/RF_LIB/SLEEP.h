/********************************** (C) COPYRIGHT *******************************
* File Name          : rf.h
* Author             : WCH
* Version            : V1.0
* Date               : 2022/03/10
* Description        :
* Copyright (c) 2023 Nanjing Qinheng Microelectronics Co., Ltd.
* SPDX-License-Identifier: Apache-2.0
*******************************************************************************/
#include <RF.h>
#include "wchrf.h"
#include "CH59x_common.h"

#ifndef SLEEP_H_
#define SLEEP_H_

#ifdef __cplusplus
extern "C" {
#endif


/*********************************************************************
 * GLOBAL VARIABLES
 */

/*********************************************************************
 * FUNCTIONS
 */

/**
 * @brief   ����˯�߻��ѵķ�ʽ   - RTC���ѣ�����ģʽ
 */
extern void RF_SleepInit(void);

/**
 * @brief   ����˯��
 *
 * @param   time    - ���ѵ�ʱ��㣨RTC����ֵ��
 *
 * @return  state.
 */
extern void RF_LowPower(uint32_t time);

/*********************************************************************
*********************************************************************/

#ifdef __cplusplus
}
#endif


#endif /* SLEEP_H_ */
