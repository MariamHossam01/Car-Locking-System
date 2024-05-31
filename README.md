# Car door handle control unit 

#### This Repo Consits of the Following:
- **Code Folder:** consits of the drivers and the main code
- **Proteus Folder:** simulation project on Proteus platform
- **Simulation Folder:** video demonstarting the outcome of the simulation

***
### Project Objectives:
The project aims to implement a vehicle door handle control unit that supports an anti-theft locking capability.  

The system consists of the following components: 

- A microcontroller unit. 
- Two push buttons, one to simulate the handle lock/unlock button and the other one to simulate the door lock/unlock.  
- Three LEDs, one to simulate the vehicle lock, one to simulate the hazard lights and one for the vehicle ambient light. 
- All the push buttons are connected as active low. 
- All the LEDs are connected as active high. 

The system shall be implemented to handle the following use cases: 



|Use case |Initial state |inputs |Resultant System state |actions: |
| - | - | - | :- | - |
|Default state |System is powered off. Inputs: |System is powered on, no button is pressed, the switch is in the on state. |Vehicle door is locked. |The three LEDs are off. |
|Door unlock |Vehicle door is locked. |Door handle button is pressed. |Vehicle door is unlocked but it is closed. |<p>Vehicle lock LED is on. </p><p>Hazard LED is blinking one time </p><p>0\.5 second high and 0.5 second low for each blink, </p><p>Ambient light LED is on for 2 seconds then off. </p>|
|Door is open |Vehicle door is unlocked and the door is closed. |Door unlock button is pressed. |Vehicle door is unlocked and the door is open. |Ambient light LED is on. |
|Anti theft vehicle lock. |Vehicle door is unlocked and the door is closed. |No buttons pressed for 10 seconds. |Vehicle door is locked |<p>Vehicle lock LED is off. </p><p>Hazard LED is blinking two times 0.5 second high and 0.5 second low for each blink, Ambient light LED is off. </p>|
|Closing the door |Vehicle door is unlocked and the door is open. |Door lock button is pressed |Vehicle door is unlocked and the door is closed |<p>Vehicle lock LED is off. </p><p>Hazard LED is off. </p><p>Ambient light LED is on for one second and then is off. </p>|
|Locking the door |Vehicle door is unlocked and the door is closed |Door handle lock button is pressed |Vehicle door is locked and the door is closed |<p>Vehicle lock LED is off. </p><p>Hazard LED is blinking two times 0.5 second high and 0.5 second low for each blink, Ambient light LED is off. </p>|

Detailed Deliverables: 

*GPIO driver* that supports the following functions: 

1- void GPIO\_Init(void) 

- A function to initialize the GPIO registers with the needed initial values to support the connected hardware actions. 

2- void GPIO\_WritePinValue(unsigned char PinId, unsigned char PinData) 

- A function to set a value (1 or 0) to a specific pin. 

3- unsigned char GPIO\_ReadPinState(unsigned char PinId) 

- A function to return (1) if a falling edge is detected on the pin and (0) otherwise. 

The GPIO driver shall be used to read the buttons’ state and drive the LEDs’ outputs. 

*GPT driver* that supports the following functions: 

1- void GPT\_Init(void) 

- A function to initialize the GPT registers with the needed initial values to support the needed timing actions. 

2- void GPT\_StartTimer(unsigned long int OverFlowTicks) 

- A function to request the GPT to start and send its number of tickets before timer overflow and stop. 

3- unsigned char GPT\_CheckTimeIsElapsed(void) 

- A function to return (1) if an overflow occurred after the last call of GPT\_StartTimer and 

  (0) if no overflow occurred or GPT\_StartTimer is not called from the last read. 

4- unsigned long int GPT\_GetElapsedTime(void) 

- A function to return number of elapsed ticks from the last call of the GPT\_StartTimer, 0 if it is not called and 0xffffffff if an overflow occurred. 

5- unsigned long int GPT\_GetRemainingTime(void) 

- A function to return number of remaining ticks till the overflow ticks passed to GPT\_StartTimer, 0xffffffff if GPT\_startTime is not called, 0 if an overflow occurred. 

The GPT driver shall be used to handle all the time actions in the project. **(No delay functions will be used, and no hardware wait loops).** 
