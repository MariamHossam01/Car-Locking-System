#ifndef GPT_PRIVATE_H_
#define GPT_PRIVATE_H_


#include "Std_Types.h"

#define TIM2 ((GPT_Type *)0x40000C00)
#define TIM3 ((GPT_Type *)0x40000800)
#define TIM4 ((GPT_Type *)0x40000400)
#define TIM5 ((GPT_Type *)0x40000000)
#define NVIC ((NVICType *)0xE000E100 )

typedef struct {
	uint32 CR1;
	uint32 CR2;
	uint32 SMCR;
	uint32 DIER;
	uint32 SR;
	uint32 EGR;
	uint64 CCMR1;
	uint64 CCMR2;
	uint32 CCER;
	uint32 CNT;
	uint32 PSC;
	uint32 ARR;
	uint32 RES;
	uint32 CCR1;
	uint32 CCR2;
	uint32 CCR3;
	uint32 CCR4;
	uint32 RES_1;
	uint32 DCR;
	uint32 DMAR;
	uint32 TIM2_OR;
	uint32 TIM5_OR;

} GPT_Type;

typedef struct {
	uint32 ISER0;
	uint32 ISER1;
	uint32 ISER2;
	uint32 ICER0;
	uint32 ICER1;
	uint32 ICER2;
	uint32 ISPR0;
	uint32 ISPR1;
	uint32 ISPR2;
	uint32 ICPR0;
	uint32 ICPR1;
	uint32 ICPR2;
	uint32 IABR0;
	uint32 IABR1;
	uint32 IABR2;
	uint32 IPR0;
	uint32 IPR1;
	uint32 IPR2;
	uint32 IPR3;
	uint32 IPR4;
	uint32 IPR5;
	uint32 IPR6;
	uint32 IPR7;
	uint32 IPR8;
	uint32 IPR9;
	uint32 IPR10;
	uint32 IPR11;
	uint32 IPR12;
	uint32 IPR13;
	uint32 IPR14;
	uint32 IPR15;
	uint32 IPR16;
	uint32 IPR17;
	uint32 IPR18;
	uint32 IPR19;
	uint32 IPR20;
} NVICType;




#endif /* GPT_PRIVATE_H_ */
