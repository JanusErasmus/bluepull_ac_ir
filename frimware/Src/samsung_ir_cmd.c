/*
 * samsung_ir_cmd.c
 *
 *  Created on: 04 Dec 2018
 *      Author: jcera
 */

#include "samsung_ir.h"

static const uint8_t fanLow[]  = {0x7F, 0xB0, 0x80, 0x7F, 0xFE, 0x53, 0xF0};
static const uint8_t fanMed[]  = {0x7F, 0xB0, 0x80, 0x7F, 0xFE, 0x63, 0xF0};
static const uint8_t fanHigh[] = {0x7F, 0xB8, 0x80, 0x7F, 0xFE, 0x23, 0xF0};
static const uint8_t ac24[]    = {0x7F, 0xB7, 0x00, 0x7F, 0xFE, 0x77, 0xF0};
static const uint8_t ac25[]    = {0x7F, 0xBF, 0x00, 0x7F, 0xF6, 0x77, 0xF0};
static const uint8_t ac26[]    = {0x7F, 0xBF, 0x00, 0x7F, 0xFA, 0x77, 0xF0};
static const uint8_t off[]     = {0x7F, 0xBB, 0x00, 0x7F, 0xFD, 0x67, 0xFC};


void samsung_ir_setFan(uint8_t speed)
{
	switch(speed)
	{
	default:
	case 0:
		samsung_ir_send(off, 7);
		return;
	case 1:
		samsung_ir_send(fanLow, 7);
		return;
	case 2:
		samsung_ir_send(fanMed, 7);
		return;
	case 3:
		samsung_ir_send(fanHigh, 7);
		return;
	}
}

void samsung_ir_setAC(uint8_t temp)
{
	switch(temp)
	{
	default:
	case 0:
		samsung_ir_send(off, 7);
		break;
	case 24:
		samsung_ir_send(ac24, 7);
		return;
	case 25:
		samsung_ir_send(ac25, 7);
		return;
	case 26:
		samsung_ir_send(ac26, 7);
		return;
	}
}
