/**
 * Gpio.h
 *
 *  Created on: Tue Apr 11 2023
 *  Author    : Abdullah Darwish
 */

#ifndef GPIO_H
#define GPIO_H
#include "Std_Types.h"
#include "Bit_Operations.h"

/*if we uses the second initial */
#define GPIO_A_ARR_INDEX	0
#define GPIO_B_ARR_INDEX	1
#define GPIO_C_ARR_INDEX	2
#define GPIO_D_ARR_INDEX	3


#define GPIO_A 'A'
#define GPIO_B 'B'
#define GPIO_C 'C'
#define GPIO_D 'D'


#define GPIO_INPUT  0x00
#define GPIO_OUTPUT 0x01
#define GPIO_AF     0x02
#define GPIO_ANALOG 0x03


#define GPIO_PUSH_PULL  0x00
#define GPIO_OPEN_DRAIN 0x01


#define LOW    0x00
#define HIGH   0x01

/*DefaultState Input*/
#define GPIO_NPUSH_NPULL  0x00
#define GPIO_INPUT_PULL_UP 0x01
#define GPIO_INPUT_PULL_DOWN 0x02

#define GPIO_NO_PULL   (0x00 << 1)
#define GPIO_PULL_UP   (0x01 << 1)
#define GPIO_PULL_DOWN (0x02 << 1)

void Gpio_ConfigPin(uint8 PortName, uint8 PinNum, uint8 PinMode, uint8 DefaultState);
void GPIO_Init(void);
void GPIO_Init2(void);
void Gpio_WritePin(uint8 PinId, uint8 PinData);
uint8 Gpio_ReadPin(uint8 PinId) ;

#endif /* GPIO_H */
