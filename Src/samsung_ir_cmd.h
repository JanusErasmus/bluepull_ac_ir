/*
 * samsung_ir_cmd.h
 *
 *  Created on: 04 Dec 2018
 *      Author: jcera
 */

#ifndef SRC_SAMSUNG_IR_CMD_H_
#define SRC_SAMSUNG_IR_CMD_H_
#include "stm32f1xx_hal.h"

#ifdef __cplusplus
extern "C" {
#endif

void samsung_ir_setFanHigh();
void samsung_ir_setFanMed();
void samsung_ir_setFanLow();


#ifdef __cplusplus
}
#endif
#endif /* SRC_SAMSUNG_IR_CMD_H_ */
