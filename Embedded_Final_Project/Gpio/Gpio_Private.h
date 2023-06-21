/**
 * Gpio_Private.h
 *
 *  Created on: Tue Apr 11 2023
 *  Author    : Abdullah Darwish
 */

#ifndef GPIO_PRIVATE_H
#define GPIO_PRIVATE_H


typedef struct {
	uint32 GPIO_MODER;
	uint32 GPIO_OTYPER;
	uint32 GPIO_OSPEEDR;
	uint32 GPIO_PUPDR;
	uint32 GPIO_IDR;
	uint32 GPIO_ODR;
	uint32 GPIO_BSRR;
	uint32 GPIO_LCKR;
	uint32 GPIO_AFRL;
	uint32 GPIO_AFRH;
} GpioType;


#endif /* GPIO_PRIVATE_H */
