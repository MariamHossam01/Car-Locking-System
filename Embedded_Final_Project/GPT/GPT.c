/*
 * GPT.c
 *
 *  Created on: May 25, 2023
 *      Author: hp
 */

#include "Std_Types.h"
#include "Bit_Operations.h"
#include "GPT.h"
#include "GPT_Private.h"
#include "RCC.h"


//#include "stm32f4xx.h"

void GPT_Init(void)
{
    // Enable the clock for TIM2
	Rcc_Enable(RCC_TIM2);


    // Configure TIM2
    TIM2->PSC = 16000 - 1; // 1 tick is 1 millisecond

    CLEAR_BIT(TIM2->CR1,4);  // count up

    TIM2->ARR = 0xFFFFFFFF; // max value
    TIM2->CNT = 0x00000000;
    TIM2->SR &= ~0x01;

    // clock division = 1
    CLEAR_BIT(TIM2->CR1,8);
    CLEAR_BIT(TIM2->CR1,9);
}


void GPT_StartTimer(unsigned long int OverFlowTicks)
{
    // Set the timer period
    TIM2->ARR = OverFlowTicks;

    // Start the timer
    TIM2->CR1 |= 0x01;


}


unsigned char GPT_CheckTimeIsElapsed(void)
{
    // Check if an overflow occurred
    if ((TIM2->SR & 0x01) != 0)
    {
        // Clear the overflow flag
        TIM2->SR &= ~0x01;
        return 1;
    }
    else
    {
        return 0;
    }
}


unsigned long int GPT_GetElapsedTime(void)
{
    // Get the elapsed time
    uint32 elapsedTime = TIM2->CNT;

    // Check if an overflow occurred
    if ((TIM2->SR & 0x01) != 0)
    {
        return 0xFFFFFFFF; // overflow occurred
    }
    else
    {
        return elapsedTime;
    }
}


unsigned long int GPT_GetRemainingTime(void)
{
    // Check if the timer is running
    if ((TIM2->CR1 & 0x01) != 0)
    {
        // Get the remaining time
        uint32 remainingTime = TIM2->ARR - TIM2->CNT;
        return remainingTime;
    }
    else
    {
        return 0xFFFFFFFF; // GPT_StartTimer is not called or an overflow occurred
    }
}
