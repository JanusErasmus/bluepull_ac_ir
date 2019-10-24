/*
 * samsung_ir.c
 *
 *  Created on: 04 Dec 2018
 *      Author: jcera
 */
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "stm32f1xx_hal.h"
#include "main.h"
#include "Utils/utils.h"

#define BUFFER_SIZE   8
#define ON_TIME       1430
#define ONE_OFF_TIME  1000 //3320
#define ZERO_OFF_TIME 3320 //1000

typedef enum
{
	IDLE,
	START,
	DATA_ONE,
	DATA_ZERO,
	STOP
}eIRstate;

static eIRstate irState = IDLE;
static int flag = 1;
static int irIndex = 0;
static int irBit = 0;
static uint8_t irBytes[BUFFER_SIZE];
static int irLen = 0;
static TIM_HandleTypeDef *modulate_tim_handle = 0;
static uint32_t modulate_tim_channel = 0;
static TIM_HandleTypeDef *oc_tim_handle = 0;
static uint32_t oc_tim_channel = 0;

void samsung_ir_init(TIM_HandleTypeDef *modulate_htim, uint32_t modulate_channel, TIM_HandleTypeDef *output_compare_htim, uint32_t output_compare_channel)
{
	modulate_tim_handle = modulate_htim;
	modulate_tim_channel = modulate_channel;
	oc_tim_handle = output_compare_htim;
	oc_tim_channel = output_compare_channel;
}

int samsung_ir_send(const uint8_t *bytes, int len)
{
	//buffer will overflow
	if(len > BUFFER_SIZE)
		return -1;

	//we are still busy
	if(irLen)
		return 0;

	printf("samsung_ir_send %d\n", len);
    diag_dump_buf(bytes, len);
    memcpy(irBytes, bytes, len);
    irLen = len;

    if(modulate_tim_handle)
    {
    	HAL_TIM_OC_Start_IT(modulate_tim_handle, modulate_tim_channel);
    }

    return len;
}

static void setNextState()
{
	if(irLen == 0)
	{
		irState = STOP;
		return;
	}

	if(irBytes[irIndex] & (1 << (7 - irBit)))
		irState = DATA_ONE;
	else
		irState = DATA_ZERO;

	irBit++;
	if(irBit >= 8)
	{
		irBit = 0;
		irIndex++;
		irLen--;
	}
}

static void setIR()
{
	HAL_TIM_OC_Start(oc_tim_handle, oc_tim_channel);
}

static void resetIR()
{
	HAL_TIM_OC_Stop(oc_tim_handle, oc_tim_channel);
}

const char *getStateName(uint8_t state)
{
	switch(state)
	{
	default:
	case IDLE:
		return "I";
	case START:
		return "S";
	case DATA_ONE:
		return "1";
	case DATA_ZERO:
		return "0";
	case STOP:
		return "P";
	}
}

void samsung_ir_service(TIM_HandleTypeDef *htim)
{
//	if(flag)
//		printf("%s\n", getStateName(irState));

	switch(irState)
	{
	default:
	case IDLE:
		if((irLen <= 0) && modulate_tim_handle)
			HAL_TIM_OC_Stop_IT(modulate_tim_handle, modulate_tim_channel);

		if(irLen > 0)
		{
			irIndex = 0;
			irBit = 0;
			irState = START;
		}
		break;
	case START:
		if(flag)
		{
			flag = 0;
			setIR();
			htim->Instance->ARR = 7130; //3ms on
		}
		else
		{
			flag = 1;
			resetIR();
			htim->Instance->ARR = 20500; //8.8ms off

			setNextState();
		}
		break;
	case DATA_ONE:
		if(flag)
		{
			flag = 0;
			setIR();
			htim->Instance->ARR = ON_TIME; //0.6ms on
		}
		else
		{
			flag = 1;
			resetIR();
			htim->Instance->ARR = ONE_OFF_TIME;  //1.4ms off

			setNextState();
		}
		break;
	case DATA_ZERO:
		if(flag)
		{
			flag = 0;
			setIR();
			htim->Instance->ARR = ON_TIME; //0.6ms on
		}
		else
		{
			flag = 1;
			resetIR();
			htim->Instance->ARR = ZERO_OFF_TIME;  //0.4ms off

			setNextState();
		}
		break;
	case STOP:
		if(flag)
		{
			flag = 0;
			setIR();
			htim->Instance->ARR = ON_TIME; //0.6ms on
		}
		else
		{
			flag = 1;
			resetIR();
			irState = IDLE;
			htim->Instance->ARR = 952;  //0.4ms off

		}
		break;
	}
}
