#include "Gpio.h"
#include "Rcc.h"
#include "Std_Types.h"
#include"GPT.h"

#define GPT_TICKS_SECOND     (1000)/2
#define GPT_TICKS_500_MS     (GPT_TICKS_SECOND / 2)
#define GPT_TICKS_TWO_SECOND (GPT_TICKS_SECOND*2)
#define DEBOUNCE_TIME_MS      50

#define BUTTON_PRESSED           0
#define BUTTON_NOT_PRESSED       1

#define LED_ON                   1
#define LED_OFF                  0

#define LOCKED                   0
#define UNLOCKED_CLOSED          1
#define UNLOCKED_OPEN            2

#define VEHICLE_LED_PIN          0
#define HAZARD_LED_PIN           1
#define AMBIENT_LED_PIN          2


uint8 door_state,hazard_led_state,ambient_led_state,vehicle_lock;
uint8 anti_theft_flag=0;
static unsigned long int remainingTime =0;

void default_state(void)
{
    door_state 		= LOCKED;

    Gpio_WritePin(VEHICLE_LED_PIN, LED_OFF);
    Gpio_WritePin(HAZARD_LED_PIN, LED_OFF);
    Gpio_WritePin(AMBIENT_LED_PIN, LED_OFF);
}

void door_unlocked(void)
{

    door_state = UNLOCKED_CLOSED;

    Gpio_WritePin(VEHICLE_LED_PIN, LED_ON);   //it should be on
    Gpio_WritePin(HAZARD_LED_PIN, LED_ON);
    Gpio_WritePin(AMBIENT_LED_PIN, LED_ON);

    //--------------------------------------------- question 3
    GPT_StartTimer(1);
    while (!GPT_CheckTimeIsElapsed());

    GPT_StartTimer(GPT_TICKS_500_MS);
    while (!GPT_CheckTimeIsElapsed());
    Gpio_WritePin(HAZARD_LED_PIN, LED_OFF);

    GPT_StartTimer((1500)/2);
    while (!GPT_CheckTimeIsElapsed());
    Gpio_WritePin(AMBIENT_LED_PIN, LED_OFF);

}

void door_open(void)
{
    Gpio_WritePin(VEHICLE_LED_PIN, LED_OFF);
    Gpio_WritePin(HAZARD_LED_PIN,  LED_OFF);
    Gpio_WritePin(AMBIENT_LED_PIN, LED_OFF);

    door_state = UNLOCKED_OPEN;

    Gpio_WritePin(AMBIENT_LED_PIN, LED_ON);
}
void anti_theft_lock(void)
{
    Gpio_WritePin(VEHICLE_LED_PIN, LED_OFF);
    Gpio_WritePin(HAZARD_LED_PIN,  LED_OFF);
    Gpio_WritePin(AMBIENT_LED_PIN, LED_OFF);

	door_state = LOCKED;

    for (uint8 i=0;i<3;i++){
        Gpio_WritePin(HAZARD_LED_PIN, LED_ON);
        GPT_StartTimer(GPT_TICKS_500_MS);
        while (!GPT_CheckTimeIsElapsed());
        Gpio_WritePin(HAZARD_LED_PIN, LED_OFF);
        GPT_StartTimer(GPT_TICKS_500_MS);
        while (!GPT_CheckTimeIsElapsed());
    }

}

void closing_door(void)
{
    Gpio_WritePin(VEHICLE_LED_PIN, LED_OFF);
    Gpio_WritePin(HAZARD_LED_PIN,  LED_OFF);
    Gpio_WritePin(AMBIENT_LED_PIN, LED_OFF);

    door_state = UNLOCKED_CLOSED;

    Gpio_WritePin(AMBIENT_LED_PIN, LED_ON);
    GPT_StartTimer(GPT_TICKS_SECOND);
    while (!GPT_CheckTimeIsElapsed());
    Gpio_WritePin(AMBIENT_LED_PIN, LED_OFF);
}

void locking_door(void)
{
    Gpio_WritePin(VEHICLE_LED_PIN, LED_OFF);
    Gpio_WritePin(HAZARD_LED_PIN,  LED_OFF);
    Gpio_WritePin(AMBIENT_LED_PIN, LED_OFF);

	door_state = LOCKED;


    for (uint8 i=0;i<3;i++){
        Gpio_WritePin(HAZARD_LED_PIN, LED_ON);
        GPT_StartTimer(GPT_TICKS_500_MS);
        while (!GPT_CheckTimeIsElapsed());
        Gpio_WritePin(HAZARD_LED_PIN, LED_OFF);
        GPT_StartTimer(GPT_TICKS_500_MS);
        while (!GPT_CheckTimeIsElapsed());
    }



}



int main() {
	Rcc_Init();
	Rcc_Enable(RCC_GPIOA);
	GPIO_Init();
	Gpio_WritePin(0, LOW);
	Gpio_WritePin(1, LOW);
	Gpio_WritePin(2, LOW);
	GPT_Init();


    uint8 handle_button_state = BUTTON_NOT_PRESSED;
    uint8 door_button_state   = BUTTON_NOT_PRESSED;
    uint8 handle_button_prev_state=1,door_button_prev_state=1;

    default_state();

  while (1)
  {
	  if (GPT_GetElapsedTime()== 0xffffffff && anti_theft_flag)
	  {
		  anti_theft_lock();
		  anti_theft_flag=0;
	  }
//--------------------------------------------- question 1
    handle_button_state = Gpio_ReadPin(3);
    door_button_state   = Gpio_ReadPin(4);
//    remainingTime=GPT_GetRemainingTime();
//    GPT_StartTimer(DEBOUNCE_TIME_MS);
//    while (!GPT_CheckTimeIsElapsed());
//    if((remainingTime>=DEBOUNCE_TIME_MS))
//    {
//        GPT_StartTimer(remainingTime-DEBOUNCE_TIME_MS);
//    }

    handle_button_state = Gpio_ReadPin(3);
    door_button_state   = Gpio_ReadPin(4);


	if(door_state==LOCKED)
	{
            if((handle_button_state==BUTTON_PRESSED)&&(handle_button_prev_state==1))
            {//01 -------> door unlock
                door_unlocked();
//                handle_button_prev_state=0;
//                 start counting 10 seconds
				GPT_StartTimer(5000);
				anti_theft_flag=1;
            }

    }
    else if(door_state==UNLOCKED_CLOSED)
    {

    	if((door_button_state==BUTTON_PRESSED)&&(door_button_prev_state==1))
            { //02 -------> door is open
            	anti_theft_flag=0;
                door_open();
//                door_button_prev_state=0;
            }
        	else if((handle_button_state==BUTTON_PRESSED)&&(handle_button_prev_state==1))
        	{ //05 -------> Locking the door
            	anti_theft_flag=0;
//                locking_door();
            	//--------------------------------------------- question 4
            	anti_theft_lock();
//                handle_button_prev_state=0;

            }
    }
    else if(door_state==UNLOCKED_OPEN){
            if((door_button_state==BUTTON_PRESSED)&&(door_button_prev_state==1))
            {//04 -------> Closing the door
                closing_door();
//                door_button_prev_state=0;
                // start counting 10 seconds
				GPT_StartTimer(5000);
				anti_theft_flag=1;
            }
    }

	//--------------------------------------------- question 2
    handle_button_prev_state=handle_button_state;
    door_button_prev_state=door_button_state;
  }
  return 0;
}
