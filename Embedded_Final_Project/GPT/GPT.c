

#include "Std_Types.h"
#include "Bit_Operations.h"
#include "GPT.h"
#include "GPT_Private.h"
#include "RCC.h"

//static uint8 flag=0;

/*----- A function to initialize the GPT registers with the needed initial values to support the
------- needed timing actions*/
void GPT_Init(void)
{
    // Enable the clock for TIM2
	Rcc_Enable(RCC_TIM2);

    TIM2->PSC = 16000 - 1;   // 1 Count is 1 millisecond
    CLEAR_BIT(TIM2->CR1,4);  // Direction -->count up
    TIM2->ARR = 0xFFFFFFFF;  // max value for ARR
    TIM2->CNT = 0x00000000;  // Initialize counter with zero
    TIM2->SR &= ~0x01;	     // clear in SR bit of Update interrupt flag
    // clock division : ratio between timer clock frequency and sampling clock-->1
    CLEAR_BIT(TIM2->CR1,8);
    CLEAR_BIT(TIM2->CR1,9);
    //Update generation bit in  event generation register
    SET_BIT(TIM2->EGR,0);

    // Enable the clock for TIM5
    Rcc_Enable(RCC_TIM5);
    TIM5->PSC = 16000 - 1;   // 1 tick is 1 millisecond
    CLEAR_BIT(TIM5->CR1,4);  // count up
    TIM5->ARR = 0xFFFFFFFF;  // max value
	TIM5->CNT = 0x00000000;
	TIM5->SR &= ~0x01;
	CLEAR_BIT(TIM5->CR1,8);
	CLEAR_BIT(TIM5->CR1,9);
	SET_BIT(TIM5->EGR,0);



}

/*-----A function to request the GPT to start and send its number of tickets before timer
------ overflow and stop.*/
void GPT_StartTimer(uint32 OverFlowTicks)
{
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

/*----- A function to return (1) if an overflow occurred after the last call of GPT_StartTimer and
------ (0) if no overflow occurred or GPT_StartTimer is not called from the last read.*/

uint8 GPT_CheckTimeIsElapsed(void)
{
    // Check if an overflow occurred
    if ((TIM2->SR & 0x01) != 0)
    {
        return 1;
    }
    else
    {
        return 0;
    }
}

/*-----A function to return number of elapsed ticks from the last call of the GPT_StartTimer, 0 if
------ it is not called and 0xffffffff if an overflow occurred. */
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
    	return 0;//it is not called
    }
    else
    {
        return elapsedTime;
    }
}

/*----- A function to return number of remaining ticks till the overflow ticks passed to
------ GPT_StartTimer, 0xffffffff if GPT_startTime is not called, 0 if an overflow occurred*/

uint32 GPT_GetRemainingTime(void)
{

    if ((TIM2->CR1 & 0x01) != 0)
    {
        // Get the remaining time
        uint32 remainingTime = TIM2->ARR - TIM2->CNT;
        return remainingTime;
    }
    else
    {
        return 0xFFFFFFFF; // GPT_StartTimer is not called
    }

}

/*
 * function that start timer 5 and wait for 0.1sec to solve debouncing effect
 */
void GPT_debouncingTimer(void)
{
		//disable timer
		CLEAR_BIT(TIM5->CR1,0);
		//counter=0
		TIM5->CNT = 0;
		//CLR SR
	    TIM5->SR &= ~0x01;
	    // Set the timer period
	    TIM5->ARR = 100;// --->0.1 sec

	    // Start the timer
	    TIM5->CR1 |= 0x01;

	// Check if an overflow occurred

	    while((TIM5->SR & 0x01)==0){}

	    TIM5->SR &= ~0x01;
}




