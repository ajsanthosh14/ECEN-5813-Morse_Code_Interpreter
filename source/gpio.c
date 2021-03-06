/*
 *  @file		: gpio.c
 *
 *  @description: includes the APIs required for the general purpose input/output
 *
 *  Created on	: December 1, 2021
 *  Author    	: Santhosh, santhosh@colorado.edu
 */

#include "GPIO.h"
#include "systick.h"

//Global variable to be accessed by the main file to know the status of the button
int buttonStatus = 0;
static int i=0;

//Refer the header file for the function documentation

void INIT_GPIO()
{

		SIM->SCGC5 |= SIM_SCGC5_PORTD_MASK;						//set clock to Port D
		PORTD->PCR[6] &= ~PORT_PCR_MUX_MASK;					//Port D pin 6 enable
		PORTD->PCR[6] |= PORT_PCR_MUX(1) | PORT_PCR_PE_MASK 	//Set to GPIO, enable pull up/down,
				| PORT_PCR_PS_MASK | PORT_PCR_IRQC(11);    		// enable pull-up, Interrupt on both edge

		NVIC_SetPriority(PORTD_IRQn,1);							// set priority for interrupt
		PORTD->PCR[6] |=PORT_PCR_ISF_MASK;						//set ISF flag
		NVIC_EnableIRQ(PORTD_IRQn);								//Enable Port D interrupt

}

void PORTD_IRQHandler()
{
		NVIC_DisableIRQ(PORTD_IRQn);					    	//disable Port D interrupt to make function atomic

		if(i){
			buttonStatus =1;									//set global variable
		}

		else
			i++;
		PORTD->PCR[6] |=PORT_PCR_ISF_MASK;						//set ISF flag
		NVIC_EnableIRQ(PORTD_IRQn);								//enable Port D interrupt
}

int button_status()
{
	return buttonStatus;
}

void button_StatusReset(int input)
{
	buttonStatus = input;
}


void disable_gpio(void)
{
	PORTD->PCR[6] &= ~PORT_PCR_ISF_MASK; 						//Disables the GPIO interrupt
}

void enable_gpio(void)
{
	PORTD->PCR[6] |=PORT_PCR_ISF_MASK;							//Enables the GPIO interrupt
}
