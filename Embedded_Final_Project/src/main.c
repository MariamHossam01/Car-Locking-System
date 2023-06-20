#include "Gpio.h"
#include "Rcc.h"
#include "Std_Types.h"
#include"GPT.h"

#define GPT_TICKS_SECOND     (1000)
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


uint8 vehicle_state=UNLOCKED_CLOSED,hazard_led_state,ambient_led_state;
uint8 anti_theft_flag=0, action_done_flag=0, prev_action=0;
static unsigned long int remainingTime =0;

void default_state(void)
{
	vehicle_state= LOCKED;

    Gpio_WritePin(VEHICLE_LED_PIN, LED_OFF);
    Gpio_WritePin(HAZARD_LED_PIN, LED_OFF);
    Gpio_WritePin(AMBIENT_LED_PIN, LED_OFF);
}

void door_unlocked(void)
{


    if(GPT_GetElapsedTime()<500)
    {
    	Gpio_WritePin(VEHICLE_LED_PIN, LED_ON);   //it should be on
		Gpio_WritePin(HAZARD_LED_PIN, LED_ON);
		Gpio_WritePin(AMBIENT_LED_PIN, LED_ON);
    }
    else if(GPT_GetElapsedTime()<1999)
    {
    	Gpio_WritePin(HAZARD_LED_PIN, LED_OFF);
    }
    else
    {
    	Gpio_WritePin(AMBIENT_LED_PIN, LED_OFF);
    	action_done_flag=0;
    	vehicle_state= UNLOCKED_CLOSED;
    	//door locked close ---> antitheft
    	GPT_StartTimer(10000);
    	anti_theft_flag=1;

    }

}

void door_open(void)
{
    vehicle_state= UNLOCKED_OPEN;

    Gpio_WritePin(VEHICLE_LED_PIN, LED_OFF);
    Gpio_WritePin(AMBIENT_LED_PIN, LED_ON);

}
void anti_theft_lock(void)
{

    Gpio_WritePin(VEHICLE_LED_PIN, LED_OFF);


//    for (uint8 i=0;i<2;i++){
//        Gpio_WritePin(HAZARD_LED_PIN, LED_ON);
//        GPT_StartTimer(GPT_TICKS_500_MS);
//        while (!GPT_CheckTimeIsElapsed());
//        Gpio_WritePin(HAZARD_LED_PIN, LED_OFF);
//        GPT_StartTimer(GPT_TICKS_500_MS);
//        while (!GPT_CheckTimeIsElapsed());
//    }
    if(GPT_GetElapsedTime()<500)
    {
    	Gpio_WritePin(HAZARD_LED_PIN, LED_ON);
    }
    else if(GPT_GetElapsedTime()<1000)
    {
    	Gpio_WritePin(HAZARD_LED_PIN, LED_OFF);
    }
    else if(GPT_GetElapsedTime()<1500)
    {
    	Gpio_WritePin(HAZARD_LED_PIN, LED_ON);
    }
    else if(GPT_GetElapsedTime()<2000)
    {
    	Gpio_WritePin(HAZARD_LED_PIN, LED_OFF);
    	vehicle_state= LOCKED;
    	action_done_flag=0;
	    anti_theft_flag=0;


    }

}

void closing_door(void)
{


//    Gpio_WritePin(AMBIENT_LED_PIN, LED_OFF);
//
//    Gpio_WritePin(AMBIENT_LED_PIN, LED_ON);
//    GPT_StartTimer(GPT_TICKS_SECOND);
//    while (!GPT_CheckTimeIsElapsed());
//    Gpio_WritePin(AMBIENT_LED_PIN, LED_OFF);

    if(GPT_GetElapsedTime()<1000)
    {
    	Gpio_WritePin(AMBIENT_LED_PIN, LED_ON);
    }
    else
    {
    	Gpio_WritePin(AMBIENT_LED_PIN, LED_OFF);
    	action_done_flag=0;
    	vehicle_state= UNLOCKED_CLOSED;

    	GPT_StartTimer(10000);
		anti_theft_flag=1;
    }
}

void locking_door(void)
{

    Gpio_WritePin(VEHICLE_LED_PIN, LED_OFF);

    if(GPT_GetElapsedTime()<499)
    {
    	Gpio_WritePin(HAZARD_LED_PIN, LED_ON);
    }
    else if(GPT_GetElapsedTime()<999)
    {
    	Gpio_WritePin(HAZARD_LED_PIN, LED_OFF);
    }
    else if(GPT_GetElapsedTime()<1499)
    {
    	Gpio_WritePin(HAZARD_LED_PIN, LED_ON);
    }
    else if(GPT_GetElapsedTime()>1500)
    {
    	Gpio_WritePin(HAZARD_LED_PIN, LED_OFF);
    	vehicle_state= LOCKED;
    	action_done_flag=0;
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
    uint8 handle_button_state_2 = BUTTON_NOT_PRESSED;
    uint8 door_button_state_2   = BUTTON_NOT_PRESSED;

    default_state();

  while (1)
  {
	  if ((GPT_GetElapsedTime()== 0xffffffff && anti_theft_flag)||(action_done_flag==3))
	  {
		  anti_theft_lock();
		  if(GPT_CheckTimeIsElapsed())  //return 1 if timer isn't started
			{
				GPT_StartTimer(2010);
				action_done_flag=3;
			}
	  }
//--------------------------------------------- question 1
    handle_button_state = Gpio_ReadPin(3);
    door_button_state   = Gpio_ReadPin(4);
    GPT_debouncingTimer();
    handle_button_state_2 = Gpio_ReadPin(3);
    door_button_state_2   = Gpio_ReadPin(4);

    if((handle_button_state==handle_button_state_2)&&(door_button_state==door_button_state_2))
    {
    	switch(vehicle_state)
    	{
    		case LOCKED:
    		{
    			if(((handle_button_state==BUTTON_PRESSED)&&(handle_button_prev_state==1))||(action_done_flag==1))
				{//01 -------> door unlock


					if((GPT_CheckTimeIsElapsed()&&(action_done_flag==0))||(action_done_flag!=1))  //return 1 if timer isn't started
					{
						action_done_flag=1;
						GPT_StartTimer(2000);
					}
					door_unlocked();

				}
    			break;
    		}
    		case UNLOCKED_CLOSED:
    		{

    			if(((door_button_state==BUTTON_PRESSED)&&(door_button_prev_state==1)))
    				{ //02 -------> door is open
    					anti_theft_flag=0;
    					door_open();
    				}

				else if(((handle_button_state==BUTTON_PRESSED)&&(handle_button_prev_state==1))||(action_done_flag==5))
				{ //05 -------> Locking the door
					if((GPT_CheckTimeIsElapsed()&&(action_done_flag==0))||(action_done_flag!=5))  //return 1 if timer isn't started
					{
						action_done_flag=5;
						GPT_StartTimer(2000);
					}
					locking_door();
					anti_theft_flag=0;
				}
    			break;
    		}
    		case UNLOCKED_OPEN:
    		{
    			if(((door_button_state==BUTTON_PRESSED)&&(door_button_prev_state==1))||(action_done_flag==4))
				{//04 -------> Closing the door

					if((GPT_CheckTimeIsElapsed()&&(action_done_flag==0))||(action_done_flag!=4))  //return 1 if timer isn't started
					{
						action_done_flag=4;
						GPT_StartTimer(1010);
					}
					closing_door();
				}
    			break;
    		}

    	}

//		if(vehicle_state==LOCKED)
//		{
//				if(((handle_button_state==BUTTON_PRESSED)&&(handle_button_prev_state==1))||(action_done_flag==1))
//				{//01 -------> door unlock
//					if(GPT_CheckTimeIsElapsed()&&(action_done_flag==0))  //return 1 if timer isn't started
//					{
//						GPT_StartTimer(2000);
//						action_done_flag=1;
//					}
//					door_unlocked();
//
//				}
//
//		}
//		else if(vehicle_state==UNLOCKED_CLOSED)
//		{
//
//			if(((door_button_state==BUTTON_PRESSED)&&(door_button_prev_state==1)))
//				{ //02 -------> door is open
//					anti_theft_flag=0;
//					door_open();
//				}
//
//				else if(((handle_button_state==BUTTON_PRESSED)&&(handle_button_prev_state==1))||(action_done_flag==5))
//				{ //05 -------> Locking the door
//					if(GPT_CheckTimeIsElapsed()&&(action_done_flag==0))  //return 1 if timer isn't started
//					{
//						GPT_StartTimer(2000);
//						action_done_flag=5;
//					}
//					locking_door();
//					anti_theft_flag=0;
//
//				}
//		}
//		else if(vehicle_state==UNLOCKED_OPEN){
//				if(((door_button_state==BUTTON_PRESSED)&&(door_button_prev_state==1))||(action_done_flag==4))
//				{//04 -------> Closing the door
//					if(GPT_CheckTimeIsElapsed()&&(action_done_flag==0))  //return 1 if timer isn't started
//					{
//						GPT_StartTimer(1010);
//						action_done_flag=4;
//					}
//					closing_door();
//	//                door_button_prev_state=0;
//					// start counting 10 seconds
//
//				}
//		}


    }
	//--------------------------------------------- question 2
    handle_button_prev_state=handle_button_state;
    door_button_prev_state=door_button_state;
  }
  return 0;
}
