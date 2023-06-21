#include "Gpio.h"
#include "Bit_Operations.h"
#include "Gpio_Private.h"
#include "Utils.h"

uint32 GPIO_Addresses[2] = {0x40020000, 0x40020400};

void Gpio_ConfigPin(uint8 PortName, uint8 PinNum, uint8 PinMode,uint8 DefaultState) {
	uint8 PortId = PortName - GPIO_A;
	GpioType * gpioReg =  GPIO_Addresses[PortId];

	uint8 OutputState = DefaultState & 0x1;

	uint8 InputState = DefaultState >> 1;

	gpioReg->GPIO_MODER &= ~(0x3 << (2 * PinNum));
	gpioReg->GPIO_MODER |= (PinMode << (2 * PinNum));

	gpioReg->GPIO_OTYPER  &= ~(0x01 << PinNum);
	gpioReg->GPIO_OTYPER |= (OutputState << PinNum);

	gpioReg->GPIO_PUPDR &= ~(0x3 << (2 * PinNum));
	gpioReg->GPIO_PUPDR |= (InputState << (2*PinNum));
}
void GPIO_Init(void){
	Gpio_ConfigPin(GPIO_A, 3,GPIO_INPUT,GPIO_INPUT_PULL_UP);
	Gpio_ConfigPin(GPIO_A, 4, GPIO_INPUT,GPIO_INPUT_PULL_UP);
	Gpio_ConfigPin(GPIO_A, 0, GPIO_OUTPUT,GPIO_PUSH_PULL);//LED_vehicle_PIN
	Gpio_ConfigPin(GPIO_A, 1, GPIO_OUTPUT,GPIO_PUSH_PULL);//LED_HAZARD_PIN
	Gpio_ConfigPin(GPIO_A, 2, GPIO_OUTPUT,GPIO_PUSH_PULL);//LED_AMBIENT_PI
}
void GPIO_Init2(void){
	uint8 PortId = GPIO_A_ARR_INDEX;

	GpioType * gpioReg1 =  GPIO_Addresses[PortId];
	GpioType * gpioReg2 =  GPIO_Addresses[PortId];

	uint8 OutputState = GPIO_PUSH_PULL & 0x1;
	uint8 InputState = GPIO_INPUT_PULL_UP >> 1;
	//for the three led pins
	for (uint8 i =0;i<3;i++){
	gpioReg1->GPIO_MODER &= ~(0x3 << (2 * i));
	gpioReg1->GPIO_MODER |= (GPIO_OUTPUT << (2 * i));

	gpioReg1->GPIO_OTYPER  &= ~(0x01 << i);
	gpioReg1->GPIO_OTYPER |= (OutputState << i);
	}
	//for the two push button pins
	for (uint8 j=3;j<5;j++){
	gpioReg2->GPIO_MODER &= ~(0x3 << (2 * j));
	gpioReg2->GPIO_MODER |= (GPIO_INPUT << (2 * j));

	gpioReg2->GPIO_PUPDR &= ~(0x3 << (2 * j));
	gpioReg2->GPIO_PUPDR |= (InputState << (2*j));
	}
}

void Gpio_WritePin(uint8 PinId, uint8 PinData) {
	uint8 PortId =GPIO_A_ARR_INDEX;
	GpioType * gpioReg =  GPIO_Addresses[PortId];
	gpioReg->GPIO_ODR &= ~(0x01 << PinId);
	gpioReg->GPIO_ODR |= (PinData << PinId);
}

uint8 Gpio_ReadPin(uint8 PinId) {
	uint8 data = 0;
	uint8 PortId = GPIO_A_ARR_INDEX;
	GpioType * gpioReg =  GPIO_Addresses[PortId];
	data = (gpioReg->GPIO_IDR & (1 << PinId)) >> PinId;
	if (data==0){
		return 1;
	}else{
		return 0;
	}
}


