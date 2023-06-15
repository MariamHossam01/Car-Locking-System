#include "Std_Types.h"
#include "Bit_Operations.h"
#include "GPT.h"
#include "GPT_Private.h"
#include "RCC.h"
#include "stm32f401xe.h"
#include "stm32f4xx.h"

// Initialize the GPT registers with the needed initial values to support the needed timing actions.
void GPT_Init(void)
{
    // Enable the clock for TIM2
    RCC->APB1ENR |= RCC_APB1ENR_TIM2EN;
//	Rcc_Init();    					// Initialize the clock
//	Rcc_Enable(RCC_TIM2);
    // Configure TIM2
    TIM2->PSC = 16000 - 1; // 1 tick is 1 millisecond
//    TIM2->CR1 &= ~TIM_CR1_DIR; // count up
    CLEAR_BIT(TIM2->CR1,4);
    TIM2->ARR = 0xFFFFFFFF; // max value
//    TIM2->CR1 &= ~TIM_CR1_CKD; // clock division = 1
    CLEAR_BIT(TIM2->CR1,8);
    CLEAR_BIT(TIM2->CR1,9);
}

// Request the GPT to start and send its number of tickets before timer overflow and stop.
void GPT_StartTimer(unsigned long int OverFlowTicks)
{
    // Set the timer period
    TIM2->ARR = OverFlowTicks;

    // Start the timer
    TIM2->CR1 |= TIM_CR1_CEN;
}

// Return (1) if an overflow occurred after the last call of GPT_StartTimer and (0) if no overflow occurred or GPT_StartTimer is not called from the last read.
unsigned char GPT_CheckTimeIsElapsed(void)
{
    // Check if an overflow occurred
    if ((TIM2->SR & TIM_SR_UIF) != 0)
    {
        // Clear the overflow flag
        TIM2->SR &= ~TIM_SR_UIF;
        return 1;
    }
    else
    {
        return 0;
    }
}

// Return number of elapsed ticks from the last call of the GPT_StartTimer, 0 if it is not called and 0xffffffff if an overflow occurred.
unsigned long int GPT_GetElapsedTime(void)
{
    // Get the elapsed time
    uint32_t elapsedTime = TIM2->CNT;

    // Check if an overflow occurred
    if ((TIM2->SR & TIM_SR_UIF) != 0)
    {
        return 0xFFFFFFFF; // overflow occurred
    }
    else
    {
        return elapsedTime;
    }
}

// Return number of remaining ticks till the overflow ticks passed to GPT_StartTimer, 0xffffffff if GPT_startTime is not called, 0 if an overflow occurred
unsigned long int GPT_GetRemainingTime(void)
{
    // Check if the timer is running
    if ((TIM2->CR1 & TIM_CR1_CEN) != 0)
    {
        // Get the remaining time
        uint32_t remainingTime = TIM2->ARR - TIM2->CNT;
        return remainingTime;
    }
    else
    {
        return 0xFFFFFFFF; // GPT_StartTimer is not called or an overflow occurred
    }
}

/*
 * GPT.c
 *
 *  Created on: May 25, 2023
 *      Author: hp
 */

//#include "Std_Types.h"
//#include "Bit_Operations.h"
//#include "GPT.h"
//#include "GPT_Private.h"
//#include "RCC.h"
//
//
//
//
//void GPT_Init(void)
//{
//    // Enable the clock for TIM2
//	Rcc_Enable(RCC_TIM2);
//
//
//    // Configure TIM2
//    TIM2->PSC = 16000 - 1; // 1 tick is 1 millisecond
//
//    CLEAR_BIT(TIM2->CR1,4);  // count up
//
//    TIM2->ARR = 0xFFFFFFFF; // max value
////	TIM2->CNT=0;
//
//    // clock division = 1
//    CLEAR_BIT(TIM2->CR1,8);
//    CLEAR_BIT(TIM2->CR1,9);
//}
//
//
//void GPT_StartTimer(unsigned long int OverFlowTicks)
//{
//
//	// Set the timer period
//    TIM2->ARR = OverFlowTicks;
//
//    // Start the timer
//    TIM2->CR1 |= 0x01;
//
//
//}
//
//
//unsigned char GPT_CheckTimeIsElapsed(void)
//{
//    // Check if an overflow occurred
//    if ((TIM2->SR & 0x01) != 0)
//    {
//        // Clear the overflow flag
//        TIM2->SR &= ~0x01;
//        return 1;
//    }
//    else
//    {
//        return 0;
//    }
//}
//
//
//unsigned long int GPT_GetElapsedTime(void)
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
//unsigned long int GPT_GetRemainingTime(void)
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

