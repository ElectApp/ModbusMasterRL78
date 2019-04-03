/***********************************************************************************************************************
* DISCLAIMER
* This software is supplied by Renesas Electronics Corporation and is only intended for use with Renesas products.
* No other uses are authorized. This software is owned by Renesas Electronics Corporation and is protected under all
* applicable laws, including copyright laws. 
* THIS SOFTWARE IS PROVIDED "AS IS" AND RENESAS MAKES NO WARRANTIES REGARDING THIS SOFTWARE, WHETHER EXPRESS, IMPLIED
* OR STATUTORY, INCLUDING BUT NOT LIMITED TO WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
* NON-INFRINGEMENT.  ALL SUCH WARRANTIES ARE EXPRESSLY DISCLAIMED.TO THE MAXIMUM EXTENT PERMITTED NOT PROHIBITED BY
* LAW, NEITHER RENESAS ELECTRONICS CORPORATION NOR ANY OF ITS AFFILIATED COMPANIES SHALL BE LIABLE FOR ANY DIRECT,
* INDIRECT, SPECIAL, INCIDENTAL OR CONSEQUENTIAL DAMAGES FOR ANY REASON RELATED TO THIS SOFTWARE, EVEN IF RENESAS OR
* ITS AFFILIATES HAVE BEEN ADVISED OF THE POSSIBILITY OF SUCH DAMAGES.
* Renesas reserves the right, without notice, to make changes to this software and to discontinue the availability 
* of this software. By using this software, you agree to the additional terms and conditions found by accessing the 
* following link:
* http://www.renesas.com/disclaimer
*
* Copyright (C) 2016, 2018 Renesas Electronics Corporation. All rights reserved.
***********************************************************************************************************************/

/***********************************************************************************************************************
* File Name    : r_cg_it_user.c
* Version      : Code Generator for RL78/G11 V1.02.02.04 [24 May 2018]
* Device(s)    : R5F1056A
* Tool-Chain   : CCRL
* Description  : This file implements device driver for IT module.
* Creation Date: 2/4/2562
***********************************************************************************************************************/

/***********************************************************************************************************************
Includes
***********************************************************************************************************************/
#include "r_cg_macrodriver.h"
#include "r_cg_it.h"
/* Start user code for include. Do not edit comment generated here */
#include "u_modbus.h"

/* End user code. Do not edit comment generated here */
#include "r_cg_userdefine.h"

/***********************************************************************************************************************
Pragma directive
***********************************************************************************************************************/
#pragma interrupt r_it_interrupt(vect=INTIT)
/* Start user code for pragma. Do not edit comment generated here */
/* End user code. Do not edit comment generated here */

/***********************************************************************************************************************
Global variables and functions
***********************************************************************************************************************/
/* Start user code for global. Do not edit comment generated here */

unsigned long counter_1ms, checkCount;
unsigned short random[5] = { 123, 456, 789, 101, 112 };
unsigned char index;

unsigned short readLength;

/* End user code. Do not edit comment generated here */

/***********************************************************************************************************************
* Function Name: r_it_interrupt
* Description  : None
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/
static void __near r_it_interrupt(void)
{
    /* Start user code. Do not edit comment generated here */

	//if(counter_1ms > 2147483646){ counter_1ms = 0; }
	counter_1ms++;

	switch(counter_1ms){
		case 1000:
			readLength = 40;
			ReadHoldingRegisters(0, readLength); //break;
		//case 1510: WriteSingleRegister(5, getModbusStatus()); break;
		//case 1520: WriteSingleRegister(7, getResponseBuffer(0)+getResponseBuffer(1));
			//WriteMultipleRegisters(10, u8ModbusADUSize, u8ModbusADU);
			counter_1ms = 0; break;
		default:
			CheckModbusStatus();
	}


    /* End user code. Do not edit comment generated here */
}

/* Start user code for adding. Do not edit comment generated here */


void ModbusResponseCallback(uint8_t exception, uint8_t u8MBFunction){

	if(exception == ResponseSuccess){
		if(u8MBFunction == fReadHoldingRegisters) {
			//WriteSingleRegister(7, getResponseBuffer(0)+getResponseBuffer(1));

			uint16_t value[100]; uint16_t i;
			for(i=0; i<100; i++){

				if(i==readLength){
					WriteMultipleRegisters(readLength, readLength, value);
					break;
				}else{
					value[i] = getResponseBuffer(i);
				}
			}

		}
	}else{
		WriteSingleRegister(9, exception);
	}

	setModbusStatus(Ready); //Update Status

}


/* End user code. Do not edit comment generated here */
