/*
 * samsung_ir.h
 *
 *  Created on: 04 Dec 2018
 *      Author: jcera
 */

#ifndef SRC_SAMSUNG_IR_H_
#define SRC_SAMSUNG_IR_H_
#include "stm32f1xx_hal.h"

#ifdef __cplusplus
extern "C" {
#endif

void samsung_ir_init(TIM_HandleTypeDef *htim);
int samsung_ir_send(uint8_t *bytes, int len);
void samsung_ir_service(TIM_HandleTypeDef *htim);

#ifdef __cplusplus
}
#endif
#endif /* SRC_SAMSUNG_IR_H_ */
