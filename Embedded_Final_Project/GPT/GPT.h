/*
 * GPT.h
 *
 *  Created on: May 25, 2023
 *      Author: hp
 */

#ifndef GPT_H_
#define GPT_H_


// A function to initialize the GPT registers with the needed initial values to support the
//needed timing actions.
void GPT_Init(void);

//A function to request the GPT to start and send its number of tickets before timer
//overflow and stop.
void GPT_StartTimer(unsigned long int OverFlowTicks);

//A function to return (1) if an overflow occurred after the last call of GPT_StartTimer and
//(0) if no overflow occurred or GPT_StartTimer is not called from the last read.
unsigned char GPT_CheckTimeIsElapsed(void);

//A function to return number of elapsed ticks from the last call of the GPT_StartTimer, 0 if
//it is not called and 0xffffffff if an overflow occurred.
unsigned long int GPT_GetElapsedTime(void);

//- A function to return number of remaining ticks till the overflow ticks passed to
//GPT_StartTimer, 0xffffffff if GPT_startTime is not called, 0 if an overflow occurred
unsigned long int GPT_GetRemainingTime(void);

void GPT_StartTimer5(unsigned long int OverFlowTicks);
unsigned char GPT_CheckTimeIsElapsed5(void);
void GPT_debouncingTimer(void);
#endif /* GPT_H_ */
