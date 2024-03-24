/*
 * main.c
 *
 *  Created on: 17 Dec 2023
 *      Author: Selim
 */
#include "../LIB/BIT_MATH.h"
#include "../LIB/stdtypes.h"
#include "../MCAL/DIO_V1/DIO_int.h"
#include "../RTOS_Layer/RTOS_Interface.h"

#define LEDS_OFF      (u8) 0
#define LEDS_HANDLER  (u8) 1
#define READ_BUTTON   (u8) 2
#define HAZARD        (u8) 3
#define LEFT_FLASHER  (u8) 4
#define RIGHT_FLASHER (u8) 5


typedef enum
{
	States_e_OFF,
	States_e_LEFT,
	States_e_RIGHT,
	States_e_HAZARD,
	Max_States
}States_e;


typedef struct
{
	u8 States_Stack[Max_States];
	u8 Top;
}Stack_t;


u8 CurrentState = States_e_OFF;
u8 LastState = States_e_OFF;

u8 LeftFlasherState = 1;
u8 RightFlasherState = 1;
u8 HazardState = 1;

u8 LeftFlaserCounter = 0;
u8 RightFlaserCounter = 0;
u8 HazardCounter = 0;

static void Stack_init(Stack_t *Stack)
{
	Stack->Top = -1;
}

static u8 Stack_isEmpty(Stack_t *Stack)
{
	return Stack->Top == -1;
}

static u8 Stack_isFull(Stack_t *Stack)
{
	return Stack->Top == Max_States - 1;
}

static u8 Stack_push(Stack_t *Stack, u8 Local_u8StateValue)
{
	if(Stack_isFull(Stack))
	{
		CurrentState = LastState;
		return CurrentState;
	}
	return Stack->States_Stack[++Stack->Top] = Local_u8StateValue;

}

static u8 Stack_pop(Stack_t *Stack)
{
	if(Stack_isEmpty(Stack))
	{
		CurrentState = LastState;
		return CurrentState;
	}
	return Stack->States_Stack[Stack->Top--];

}

//static u8 Stack_peek(Stack_t *Stack)
//{
//	if(Stack_isEmpty(Stack))
//	{
//		CurrentState = States_e_OFF;
//		return CurrentState;
//	}
//	return  Stack->States_Stack[Stack->Top];
//}

void LeftFlasher_voidTask(void);
void RightFlasher_voidTask(void);
void Hazard_voidTask(void);
void LedsOff_voidTask(void);
void ReadButton_voidTask(void);
void LedsHandler_voidTask(void);


void create_task(void)
{
	CreateTask_FuncArgus ReadButton_Task;
	ReadButton_Task.Copy_u8Priority = READ_BUTTON;
	ReadButton_Task.Copy_u8State = TASK_RESUMED;
	ReadButton_Task.Copy_u16Periodicity = 100;
	ReadButton_Task.Copy_u16FirstDelay = 0 ;
	ReadButton_Task.Copy_pvTaskFunc = &ReadButton_voidTask;
	RTOS_u8CreateTask(&ReadButton_Task);

	CreateTask_FuncArgus LedsHandler_Task;
	LedsHandler_Task.Copy_u8Priority = LEDS_HANDLER;
	LedsHandler_Task.Copy_u8State = TASK_SUSPENDED;
	LedsHandler_Task.Copy_u16Periodicity = 100;
	LedsHandler_Task.Copy_u16FirstDelay = 0;
	LedsHandler_Task.Copy_pvTaskFunc = &LedsHandler_voidTask;
	RTOS_u8CreateTask(&LedsHandler_Task);

	CreateTask_FuncArgus LedsOff_Task;
	LedsOff_Task.Copy_u8Priority = LEDS_OFF;
	LedsOff_Task.Copy_u8State = TASK_RESUMED;
	LedsOff_Task.Copy_u16Periodicity = 200;
	LedsOff_Task.Copy_u16FirstDelay = 0;
	LedsOff_Task.Copy_pvTaskFunc = &LedsOff_voidTask;
	RTOS_u8CreateTask(&LedsOff_Task);

	CreateTask_FuncArgus Hazard_Task;
	Hazard_Task.Copy_u8Priority = HAZARD;
	Hazard_Task.Copy_u8State = TASK_SUSPENDED;
	Hazard_Task.Copy_u16Periodicity = 500;
	Hazard_Task.Copy_u16FirstDelay = 300;
	Hazard_Task.Copy_pvTaskFunc = &Hazard_voidTask;
	RTOS_u8CreateTask(&Hazard_Task);

	CreateTask_FuncArgus LeftFlasher_Task;
	LeftFlasher_Task.Copy_u8Priority = LEFT_FLASHER;
	LeftFlasher_Task.Copy_u8State = TASK_SUSPENDED;
	LeftFlasher_Task.Copy_u16Periodicity = 500;
	LeftFlasher_Task.Copy_u16FirstDelay = 100;
	LeftFlasher_Task.Copy_pvTaskFunc = &LeftFlasher_voidTask;
	RTOS_u8CreateTask(&LeftFlasher_Task);

	CreateTask_FuncArgus RightFlasher_Task;
	RightFlasher_Task.Copy_u8Priority = RIGHT_FLASHER;
	RightFlasher_Task.Copy_u8State = TASK_SUSPENDED;
	RightFlasher_Task.Copy_u16Periodicity = 500;
	RightFlasher_Task.Copy_u16FirstDelay = 200;
	RightFlasher_Task.Copy_pvTaskFunc = &RightFlasher_voidTask;
	RTOS_u8CreateTask(&RightFlasher_Task);

}

void main(void)
{

	DIO_u8setPinDirection(PORT_A,PIN_0,INPUT);
	DIO_u8setPinDirection(PORT_A,PIN_1,INPUT);
	DIO_u8setPinDirection(PORT_A,PIN_2,INPUT);
	DIO_u8setPinDirection(PORT_A,PIN_3,OUTPUT);
	DIO_u8setPinDirection(PORT_A,PIN_4,OUTPUT);

	DIO_u8setPinValue(PORT_A,PIN_0,HIGH);
	DIO_u8setPinValue(PORT_A,PIN_1,HIGH);
	DIO_u8setPinValue(PORT_A,PIN_2,HIGH);



	create_task();
	RTOS_voidStart();
	while(1)
	{

	}
}

void LeftFlasher_voidTask(void)
{
	static u8 Local_u8LedState = 0;
	TOG_BIT(Local_u8LedState,0);
	DIO_u8setPinValue(PORT_A,PIN_4,Local_u8LedState);

}

void RightFlasher_voidTask(void)
{
	static u8 Local_u8LedState = 0;
	TOG_BIT(Local_u8LedState,0);
	DIO_u8setPinValue(PORT_A,PIN_3,Local_u8LedState);
}

void Hazard_voidTask(void)
{
	static u8 Local_u8LedState = 0;
	TOG_BIT(Local_u8LedState,0);
	DIO_u8setPinValue(PORT_A,PIN_3,Local_u8LedState);
	DIO_u8setPinValue(PORT_A,PIN_4,Local_u8LedState);
}

void LedsOff_voidTask(void)
{
	DIO_u8setPinValue(PORT_A,PIN_3,LOW);
	DIO_u8setPinValue(PORT_A,PIN_4,LOW);
}


void ReadButton_voidTask(void)
{

	Stack_t Local_statesStack;

	Stack_init(&Local_statesStack);
	RTOS_voidSuspendTask(LEDS_HANDLER);

	DIO_u8getPinValue(PORT_A,PIN_0,&LeftFlasherState);
	DIO_u8getPinValue(PORT_A,PIN_1,&RightFlasherState);
	DIO_u8getPinValue(PORT_A,PIN_2,&HazardState);

    CurrentState = Stack_push(&Local_statesStack,States_e_OFF);

	if(LeftFlasherState == 0)
	{
		LeftFlaserCounter++;
		switch(LeftFlaserCounter)
		{
		case 1: CurrentState = Stack_push(&Local_statesStack,States_e_LEFT); RTOS_voidResumeTask(LEDS_HANDLER); break;
		case 2: LastState = CurrentState; CurrentState = Stack_pop(&Local_statesStack);
		        RTOS_voidResumeTask(LEDS_HANDLER); LeftFlaserCounter = 0; break;
		default: RTOS_voidSuspendTask(LEDS_HANDLER);
		}
	}

	if(RightFlasherState == 0)
	{
		RightFlaserCounter++;
		switch(RightFlaserCounter)
		{
		case 1: CurrentState = Stack_push(&Local_statesStack,States_e_RIGHT); RTOS_voidResumeTask(LEDS_HANDLER); break;
		case 2: LastState = CurrentState; CurrentState = Stack_pop(&Local_statesStack);
		        RTOS_voidResumeTask(LEDS_HANDLER); RightFlaserCounter = 0; break;
		default: RTOS_voidSuspendTask(LEDS_HANDLER);
		}
	}

	if(HazardState == 0)
	{
		HazardCounter++;
		switch(HazardCounter)
		{
		case 1: CurrentState = Stack_push(&Local_statesStack,States_e_HAZARD); RTOS_voidResumeTask(LEDS_HANDLER); break;
		case 2: LastState = CurrentState; CurrentState = Stack_pop(&Local_statesStack);
		        RTOS_voidResumeTask(LEDS_HANDLER); HazardCounter = 0; break;
		default: RTOS_voidSuspendTask(LEDS_HANDLER);
		}
	}

}

void LedsHandler_voidTask(void)
{
	switch(CurrentState)
	{
	case States_e_OFF:
		RTOS_voidResumeTask(LEDS_OFF);
		RTOS_voidSuspendTask(LEFT_FLASHER);
		RTOS_voidSuspendTask(RIGHT_FLASHER);
		RTOS_voidSuspendTask(HAZARD);
		break;

	case States_e_LEFT:
		RTOS_voidSuspendTask(LEDS_OFF);
		RTOS_voidResumeTask(LEFT_FLASHER);
		RTOS_voidSuspendTask(RIGHT_FLASHER);
		RTOS_voidSuspendTask(HAZARD);
		break;

	case States_e_RIGHT:
		RTOS_voidSuspendTask(LEDS_OFF);
		RTOS_voidSuspendTask(LEFT_FLASHER);
		RTOS_voidResumeTask(RIGHT_FLASHER);
		RTOS_voidSuspendTask(HAZARD);
		break;

	case States_e_HAZARD:
		RTOS_voidSuspendTask(LEDS_OFF);
		RTOS_voidSuspendTask(LEFT_FLASHER);
		RTOS_voidSuspendTask(RIGHT_FLASHER);
		RTOS_voidResumeTask(HAZARD);
		break;
	}

}
