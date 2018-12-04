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
#define ONE_OFF_TIME  3320
#define ZERO_OFF_TIME 1000

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
static int irBit = 8;
static uint8_t irBytes[BUFFER_SIZE];
static int irLen = 0;
static TIM_HandleTypeDef *tim_handle = 0;
static uint32_t tim_channel = 0;

void samsung_ir_init(TIM_HandleTypeDef *htim, uint32_t channel)
{
	tim_handle = htim;
	tim_channel = channel;
}

int samsung_ir_send(uint8_t *bytes, int len)
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

    if(tim_handle)
    {
    	HAL_TIM_OC_Start_IT(tim_handle, tim_channel);
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

	if(irBytes[irIndex] & (1 << (8 - irBit)))
		irState = DATA_ONE;
	else
		irState = DATA_ZERO;

	irBit--;
	if(irBit == 0)
	{
		irBit = 8;
		irIndex++;
		irLen--;
	}
}

static void setIR()
{
	GPIO_InitTypeDef GPIO_InitStruct;
	GPIO_InitStruct.Pin = IR_CCO_Pin;
	GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
	HAL_GPIO_Init(IR_CCO_GPIO_Port, &GPIO_InitStruct);
}

static void resetIR()
{
	GPIO_InitTypeDef GPIO_InitStruct;
	GPIO_InitStruct.Pin = IR_CCO_Pin;
	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
	HAL_GPIO_Init(IR_CCO_GPIO_Port, &GPIO_InitStruct);
}

void samsung_ir_service(TIM_HandleTypeDef *htim)
{
	switch(irState)
	{
	default:
	case IDLE:
		if((irLen <= 0) && tim_handle)
			HAL_TIM_OC_Stop_IT(tim_handle, tim_channel);

		if(irLen > 0)
		{
			irIndex = 0;
			irBit = 8;
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
