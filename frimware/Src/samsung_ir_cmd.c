/*
 * samsung_ir_cmd.c
 *
 *  Created on: 04 Dec 2018
 *      Author: jcera
 */

#include "samsung_ir.h"

//  uint8_t cmd[] = {0x01, 0x02, 0xFF, 0x01, 0x80, 0x19, 0xF0}; //ac25auto
uint8_t fanLow[]  = {0x01, 0xE2, 0xFE, 0x01, 0x90, 0x35, 0xF0}; //fanLow
uint8_t fanMed[]  = {0x01, 0xE2, 0xFE, 0x01, 0x90, 0x39, 0xF0}; //fanMed
uint8_t fanHigh[] = {0x01, 0xD2, 0xFE, 0x01, 0x90, 0x3B, 0xF0}; //fanHigh


void samsung_ir_setFanHigh()
{
	samsung_ir_send(fanHigh, 7);
}

void samsung_ir_setFanMed()
{
	samsung_ir_send(fanMed, 7);
}

void samsung_ir_setFanLow()
{
	samsung_ir_send(fanLow, 7);
}
