/******************************************************************************
* Project Name		: CE95277 ADC and UART
* File Name			: main.c
* Version 			: **
* Device Used		: CY8C5888LTI-LP097
* Software Used		: PSoC Creator 3.1 SP2
* Compiler    		: ARM GCC 4.8.4, ARM RVDS Generic, ARM MDK Generic
* Related Hardware	: CY8CKIT059 PSoC 5 LP Prototyping Kit 
* Owner				: KLMZ
*
********************************************************************************
* Copyright (2015), Cypress Semiconductor Corporation. All Rights Reserved.
********************************************************************************
* This software is owned by Cypress Semiconductor Corporation (Cypress)
* and is protected by and subject to worldwide patent protection (United
* States and foreign), United States copyright laws and international treaty
* provisions. Cypress hereby grants to licensee a personal, non-exclusive,
* non-transferable license to copy, use, modify, create derivative works of,
* and compile the Cypress Source Code and derivative works for the sole
* purpose of creating custom software in support of licensee product to be
* used only in conjunction with a Cypress integrated circuit as specified in
* the applicable agreement. Any reproduction, modification, translation,
* compilation, or representation of this software except as specified above 
* is prohibited without the express written permission of Cypress.
*
* Disclaimer: CYPRESS MAKES NO WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, WITH 
* REGARD TO THIS MATERIAL, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED 
* WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
* Cypress reserves the right to make changes without further notice to the 
* materials described herein. Cypress does not assume any liability arising out 
* of the application or use of any product or circuit described herein. Cypress 
* does not authorize its products for use as critical components in life-support 
* systems where a malfunction or failure may reasonably be expected to result in 
* significant injury to the user. The inclusion of Cypress' product in a life-
* support systems application implies that the manufacturer assumes all risk of 
* such use and in doing so indemnifies Cypress against all charges. 
*
* Use of this Software may be limited by and subject to the applicable Cypress
* software license agreement. 
*******************************************************************************/

#include <device.h>
#include "stdio.h"

/* Project Defines */
#define FALSE  0
#define TRUE   1
#define TRANSMIT_BUFFER_SIZE  16


/*******************************************************************************
* Function Name: main
********************************************************************************
*
* Summary:
*  main() performs following functions:
*  1: Starts the ADC and UART components.
*  2: Checks for ADC end of conversion.  Stores latest result in output
*     if conversion complete.
*  3: Checks for UART input.
*     On 'C' or 'c' received: transmits the last sample via the UART.
*     On 'S' or 's' received: continuously transmits samples as they are completed.
*     On 'X' or 'x' received: stops continuously transmitting samples.
*     On 'E' or 'e' received: transmits a dummy byte of data.
*
* Parameters:
*  None.
*
* Return:
*  None.
*
*******************************************************************************/
void main()
{
    /* Variable to store ADC result */
    uint32 Output;
    /* Variable to store UART received character */
    uint8 Ch;
    /* Variable used to send emulated data */
    uint8 EmulatedData;
    /* Flags used to store transmit data commands */
    uint8 ContinuouslySendData;
    uint8 SendSingleByte;
    uint8 SendEmulatedData;
    /* Transmit Buffer */
    char TransmitBuffer1[TRANSMIT_BUFFER_SIZE];
    char TransmitBuffer2[TRANSMIT_BUFFER_SIZE];
    /* Start the components */
    ADC_DelSig_1_Start();
    UART_1_Start();
    PWM_Start();
    
    /* Initialize Variables */
    ContinuouslySendData = FALSE;
    SendSingleByte = FALSE;
    SendEmulatedData = FALSE;
    EmulatedData = 0;
    
    /* Start the ADC conversion */
    ADC_DelSig_1_StartConvert();
    
    /* Send message to verify COM port is connected properly */
    UART_1_PutString("COM Port Open");
    
    // Set the reference angle
    int reference_angle = 120;
    int angle_to_volt = 10;
    int reference_volt = reference_angle*angle_to_volt;
    
    //initialize the error, proportional gain
    int error = 0;
    int Pcontrol = 0;
    //Kp=2.1: oscillation
    //Kp<2: steady state error -33mV
    double Kp = 0.2;
    
    //initialize the integral error, integral gain
    int Ierror = error;
    int Icontrol = 0;
    double Ki = 0.001;
    
    //initialize the delta error, derivative gain
    int last_error = 0;
    int derror = 0;
    double Kd = 0.2;
    int Dcontrol = 0;
    
    for(;;)
    {        
        /* Non-blocking call to get the latest data recieved  */
        Ch = UART_1_GetChar();
        
        /* Set flags based on UART command */
        switch(Ch)
        {
            case 0:
                /* No new data was recieved */
                break;
            case 'C':
            case 'c':
                SendSingleByte = TRUE;
                break;
            case 'S':
            case 's':
                ContinuouslySendData = TRUE;
                break;
            case 'X':
            case 'x':
                ContinuouslySendData = FALSE;
                break;
            case 'E':
            case 'e':
                SendEmulatedData = TRUE;
                break;
            default:
                /* Place error handling code here */
                break;    
        }
        
        /* Check to see if an ADC conversion has completed */
        if(ADC_DelSig_1_IsEndConversion(ADC_DelSig_1_RETURN_STATUS))
        {
            Output = ADC_DelSig_1_CountsTo_mVolts(ADC_DelSig_1_GetResult16());
            
            // calculate PID error
            last_error = error;
            error = reference_volt - Output;
            Ierror = Ierror + error;
            derror = error - last_error;
            // generate PID control commands
            Pcontrol = Kp*error + Output;
            Icontrol = Ki*Ierror;
            Dcontrol = Kd*derror;
         
            PWM_WriteCompare(Pcontrol+Icontrol+Dcontrol);
            CyDelay(1);
            
            
            
            if(SendSingleByte || ContinuouslySendData)
            {
                /* Format ADC result for transmition */
                //sprintf(TransmitBuffer1, "Integral control: %d mV\r\n", Icontrol);
                //sprintf(TransmitBuffer1, "Integral error: %d mV\r\n", Ierror);
                sprintf(TransmitBuffer1, "error: %d mV\r\n", error);
                //sprintf(TransmitBuffer1, "control: %d mV\r\n", Pcontrol);
                //sprintf(TransmitBuffer, "Sample: %d mV\r\n", Output);
                /* Send out the data */
                UART_1_PutString(TransmitBuffer1);
                //UART_1_PutString(TransmitBuffer2);
                /* Reset the send once flag */
                SendSingleByte = FALSE;
            }
            else if(SendEmulatedData)
            {
                /* Format ADC result for transmition */
                sprintf(TransmitBuffer1, "Emulated Data: %x \r\n", EmulatedData);
                /* Send out the data */
                UART_1_PutString(TransmitBuffer1);
                EmulatedData++;
                /* Reset the send once flag */
                SendEmulatedData = FALSE;   
            }
        }
    }
}
        
        
  //      if(ADC_DelSig_1_IsEndConversion(ADC_DelSig_1_RETURN_STATUS))
   //     {
            /* Use the GetResult16 API to get an 8 bit unsigned result in
             * single ended mode.  The API CountsTo_mVolts is then used
             * to convert the ADC counts into mV before transmitting via
             * the UART.  See the datasheet API description for more
             * details */
  //          Output = ADC_DelSig_1_CountsTo_mVolts(ADC_DelSig_1_GetResult16());
  //          PWM_WriteCompare(Output+100);
  //          CyDelay(1000);
            /* Send data based on last UART command */
  //          if(SendSingleByte || ContinuouslySendData)
  //          {
                /* Format ADC result for transmition */
  //              sprintf(TransmitBuffer, "Sample: %d mV\r\n", Output);
  //              /* Send out the data */
  //              UART_1_PutString(TransmitBuffer);
                /* Reset the send once flag */
  //              SendSingleByte = FALSE;
  //          }
  //          else if(SendEmulatedData)
  //          {
                /* Format ADC result for transmition */
  //              sprintf(TransmitBuffer, "Emulated Data: %x \r\n", EmulatedData);
                /* Send out the data */
  //              UART_1_PutString(TransmitBuffer);
  //              EmulatedData++;
                /* Reset the send once flag */
  //              SendEmulatedData = FALSE;   
  //          }
  //      }
  //  }
//}


/* [] END OF FILE */
