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

void samsung_ir_init(TIM_HandleTypeDef *modulate_htim, uint32_t modulate_channel, TIM_HandleTypeDef *output_compare_htim, uint32_t output_compare_channel);
int samsung_ir_send(const uint8_t *bytes, int len);
void samsung_ir_service(TIM_HandleTypeDef *htim);

#ifdef __cplusplus
}
#endif
#endif /* SRC_SAMSUNG_IR_H_ */
