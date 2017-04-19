/*
 * Timer.c
 *
 *  Created on: 10 ��� 2015
 *      Author: stelkork
 */
#include "Timer.h"
#include "Interrupts.h"
#include "UART.h"
#include "Defs.h"
#include "crc.h"
#include <stdlib.h>



void Timer_Init(XTmrCtr *TmrCtrInstancePtr, u8 TmrCtrNumber, u16 DeviceId,XTmrCtr *TmrCtrInstancePtr_1, u8 TmrCtrNumber_1, u16 DeviceId_1)
{

	/*
	 * Initialize the timer counter so that it's ready to use,
	 * specify the device ID that is generated in xparameters.h
	 */
	XTmrCtr_Initialize(TmrCtrInstancePtr, DeviceId);

	/*
	 * Setup the handler for the timer counter that will be called from the
	 * interrupt context when the timer expires, specify a pointer to the
	 * timer counter driver instance as the callback reference so the handler
	 * is able to access the instance data
	 */
	XTmrCtr_SetHandler(TmrCtrInstancePtr, TimerCounterHandler, TmrCtrInstancePtr);//alagh

	/*
	 * Enable the interrupt of the timer counter so interrupts will occur
	 * and use auto reload mode such that the timer counter will reload
	 * itself automatically and continue repeatedly, without this option
	 * it would expire once only
	 */
	XTmrCtr_SetOptions(TmrCtrInstancePtr, TmrCtrNumber,	XTC_INT_MODE_OPTION | XTC_AUTO_RELOAD_OPTION);

	/*
	 * Set a reset value for the timer counter such that it will expire
	 * eariler than letting it roll over from 0, the reset value is loaded
	 * into the timer counter when it is started
	 */
	XTmrCtr_SetResetValue(TmrCtrInstancePtr, TmrCtrNumber, RESET_VALUE);

	/*
	 * Start the timer counter such that it's incrementing by default,
	 * then wait for it to timeout a number of times
	 */
	XTmrCtr_Start(TmrCtrInstancePtr, TmrCtrNumber);


	/*************************************************************************/
	/*=======================================================================*/
	/*************************************************************************/
	/*
	 * Initialize the timer counter so that it's ready to use,
	 * specify the device ID that is generated in xparameters.h
	 */
	XTmrCtr_Initialize(TmrCtrInstancePtr_1, DeviceId_1);

	/*
	 * Setup the handler for the timer counter that will be called from the
	 * interrupt context when the timer expires, specify a pointer to the
	 * timer counter driver instance as the callback reference so the handler
	 * is able to access the instance data
	 */
	XTmrCtr_SetHandler(TmrCtrInstancePtr_1, TimerHandler, TmrCtrInstancePtr_1);

	/*
	 * Enable the interrupt of the timer counter so interrupts will occur
	 * and use auto reload mode such that the timer counter will reload
	 * itself automatically and continue repeatedly, without this option
	 * it would expire once only
	 */
	XTmrCtr_SetOptions(TmrCtrInstancePtr_1, TmrCtrNumber_1,	XTC_INT_MODE_OPTION | XTC_AUTO_RELOAD_OPTION);

	/*
	 * Set a reset value for the timer counter such that it will expire
	 * eariler than letting it roll over from 0, the reset value is loaded
	 * into the timer counter when it is started
	 */
	XTmrCtr_SetResetValue(TmrCtrInstancePtr_1, TmrCtrNumber_1, RESET_VALUE_1);

	/*
	 * Start the timer counter such that it's incrementing by default,
	 * then wait for it to timeout a number of times
	 */
	XTmrCtr_Start(TmrCtrInstancePtr_1, TmrCtrNumber_1);

}

/*****************************************************************************/
/**
* This function is the handler which performs processing for the transmition to the channel.
* The timer check flags and make the properly actions, send XON/XOFF, send data, or send nothing
*
******************************************************************************/

void TimerCounterHandler(void *CallBackRef, u8 TmrCtrNumber)
{
	u8 TC_data;
	u8 temp,temp2;
	if (flag_enable_TC==1)
	{
		TC_data = TB[Rd_TB];

		switch(flag_rc)		//	 an flag_rc=0 steilei xoff alliws xon
		{
		case(0):
			temp = DEL;
			XUartLite_Send(&UART_Inst_Ptr_2, &temp, 1);
			temp2=XOFF ^ 32;
			XUartLite_Send(&UART_Inst_Ptr_2, &temp2, 1);
			break;
		case(1):
			temp = DEL;
			XUartLite_Send(&UART_Inst_Ptr_2, &temp, 1);
			temp2=XON ^ 32;
			XUartLite_Send(&UART_Inst_Ptr_2, &temp2, 1);
			flag_rc=2;
			break;
		case(2):
			if (flag_xon_xoff==0)//mhn steileis
			{
				xil_printf("den stelnw reeeeeeeeee\n\r");
			}
			else
			{
				if(TC_data==DEL)
				{
					temp=DEL;
					XUartLite_Send(&UART_Inst_Ptr_2, &temp, 1);
					temp2=DEL ^ 32;
					XUartLite_Send(&UART_Inst_Ptr_2, &temp2, 1);

				}
				else if(Rd_TB==75)
				{
					temp=crc_final ;
					xil_printf("crc = %X\r\n",temp);
					XUartLite_Send(&UART_Inst_Ptr_2, &temp, 1);
				}
				else
				{
					XUartLite_Send(&UART_Inst_Ptr_2, &TC_data, 1);
				}
				Rd_TB = (Rd_TB + 1) % BuffSize;
			}
			break;
		case(3):
		{
			temp=DEL;
			XUartLite_Send(&UART_Inst_Ptr_2, &temp, 1);
			temp = 0xFF ^32; //send to transmitter board the ack xor 32
			XUartLite_Send(&UART_Inst_Ptr_2, &temp, 1);
			flag_enable_TC=0;
			break;
		}
		case(4):
		{
			temp=DEL;
			XUartLite_Send(&UART_Inst_Ptr_2, &temp, 1);
			temp = 0x00 ^32;	// send back to transmiter baord the nack xor 32
			XUartLite_Send(&UART_Inst_Ptr_2, &temp, 1);
			flag_enable_TC=0;
			break;
		}

		default:
			xil_printf("LA8OOOOOOOOOOOOOOOOOOOOOOOOOOOS");
		}
		num_of_reads++;
	}

}


/***************************************************************************/
/**
* This function is a handler for receiver's timer for data readen from Receiver's cyclic buffer
* This function check if Matlab's CTS is ON/OFF and keep sending data or not, respectivly
*
*/
void TimerHandler(void *CallBackRef, u8 TmrCtrNumber_1)
{
	unsigned char *cts_1 = (unsigned char*) UART1_CTS_ADDR;
	u8 RU_data;	
	if(flag_enable_RC==1)
	{
		if(*cts_1 == 0x00)
		{
			RU_data = RB[Rd_RB];
			XUartLite_Send(&UART_Inst_Ptr_1, &RU_data, BUF_SIZE);
			Rd_RB = (Rd_RB + 1) % BuffSize;
			num_of_data++;
		}
		else
			xil_printf("CTS is OFF TMR KATW\n\r");
	}
}
