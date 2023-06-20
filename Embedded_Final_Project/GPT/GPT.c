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

static uint8 flag=0;
void GPT_Init(void)
{
    // Enable the clock for TIM2,TIM5
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
    SET_BIT(TIM2->EGR,0);



    Rcc_Enable(RCC_TIM5);
    TIM5->PSC = 16000 - 1; // 1 tick is 1 millisecond
    CLEAR_BIT(TIM5->CR1,4);  // count up
    TIM5->ARR = 0xFFFFFFFF; // max value
	TIM5->CNT = 0x00000000;
	TIM5->SR &= ~0x01;
	CLEAR_BIT(TIM5->CR1,8);
	CLEAR_BIT(TIM5->CR1,9);
	SET_BIT(TIM5->EGR,0);



}


void GPT_StartTimer(uint32 OverFlowTicks)
{
	flag=1;
	//disable timer
	CLEAR_BIT(TIM2->CR1,0);
	//counter=0
	TIM2->CNT = 0;
	//CLR SR
    TIM2->SR &= ~0x01;
    // Set the timer period
    TIM2->ARR = OverFlowTicks;

    // Start the timer
    TIM2->CR1 |= 0x01;


}


uint8 GPT_CheckTimeIsElapsed(void)
{
    // Check if an overflow occurred
    if ((TIM2->SR & 0x01) != 0)
    {
        // Clear the overflow flag
//        TIM2->SR &= ~0x01;
        return 1;
    }
    else
    {
        return 0;
    }
}


//uint32 GPT_GetElapsedTime(void)
//{
//    // Get the elapsed time
//    uint32 elapsedTime = TIM2->CNT;
//
//    // Check if an overflow occurred
//    if ((TIM2->SR & 0x01) != 0)
//    {
//        return 0xFFFFFFFF; // overflow occurred
//    }
//    else
//    {
//        return elapsedTime;
//    }
//}
//
//
//uint32 GPT_GetRemainingTime(void)
//{
//    // Check if the timer is running
//    if ((TIM2->CR1 & 0x01) != 0)
//    {
//        // Get the remaining time
//        uint32 remainingTime = TIM2->ARR - TIM2->CNT;
//        return remainingTime;
//    }
//    else
//    {
//        return 0xFFFFFFFF; // GPT_StartTimer is not called or an overflow occurred
//    }
//}




void GPT_debouncingTimer(void)
{
		//disable timer
		CLEAR_BIT(TIM5->CR1,0);
		//counter=0
		TIM5->CNT = 0;
		//CLR SR
	    TIM5->SR &= ~0x01;
	    // Set the timer period
	    TIM5->ARR = 100;

	    // Start the timer
	    TIM5->CR1 |= 0x01;

	// Check if an overflow occurred

	    while((TIM5->SR & 0x01)==0){}

	    TIM5->SR &= ~0x01;
}

uint32 GPT_GetElapsedTime(void)
{
    // Get the elapsed time
    uint32 elapsedTime = TIM2->CNT;

    // Check if an overflow occurred
    if ((TIM2->SR & 0x01) != 0)
    {
        return 0xFFFFFFFF; // overflow occurred
    }
    else if((!READ_BIT(TIM2->CR1,0)))
    {
    	return 0;
    }
    else
    {
        return elapsedTime;
    }
}


uint32 GPT_GetRemainingTime(void)
{
    // Check if the timer is running
    if ((TIM2->CR1 & 0x01) != 0)
    {
        // Get the remaining time
        uint32 remainingTime = TIM2->ARR - TIM2->CNT;
        return remainingTime;
    }
    else if ((!READ_BIT(TIM2->CR1,0)))
    {
        return 0xFFFFFFFF; // GPT_StartTimer is not called or an overflow occurred
    }
    else
    {
    	return 0;
    }
}





