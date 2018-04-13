/* ========================================

 *

 * Copyright YOUR COMPANY, THE YEAR

 * All Rights Reserved

 * UNPUBLISHED, LICENSED SOFTWARE.

 *

 * CONFIDENTIAL AND PROPRIETARY INFORMATION

 * WHICH IS THE PROPERTY OF your company.

 *

 * ========================================

*/

#include "project.h"

#include "string.h"

#include <stdio.h>

#include <stdlib.h>

int compareVar = 1000;
int previousVar = 1000;

    
CY_ISR(ISR_Timer_Interrupt){
 
    if(compareVar>previousVar){
        for(int i=previousVar; i<compareVar; i+=1){
            PWM_1_WriteCompare(i);
            CyDelay(4);
        }
    }
    else{
        for(int i=previousVar; i>compareVar; i-=1){
            PWM_1_WriteCompare(i);
            CyDelay(4);
        }
    }
    previousVar = compareVar;
    //PWM_1_WriteCompare(compareVar);
    

    Timer_1_ReadStatusRegister();

}

// this function converts the string to the angle value

int angleCalculator(uint8 *angle){

    //return 408+10*(atoi((const char*)angle));
    return 408+10*(atoi((const char*)angle));

}



// This program construct 7 strings to collect info about seven motor angles

int main(void){

    CyGlobalIntEnable; /* Enable global interrupts. */

    // start the uart and pwm module

    UART_1_Start();

    PWM_1_Start();

    PWM_2_Start();

    PWM_3_Start();

    PWM_4_Start();

    PWM_5_Start();

    Timer_1_Start();

    Timer_Interrupt_StartEx(ISR_Timer_Interrupt);
    

    // variables initialization

    uint8 receive;

    uint8 angle[5][10];

    uint8 string[5][10];

    uint8 stringtest[10];

    

    int i=0;

    int j=0;

    // main for loop

    for(;;)

    {

        // get a char

        receive = UART_1_GetChar();

    

        if(receive){

            //initialize the i and j if read 'B'

            if(receive == 'B'){

                UART_1_GetChar();

                i = 0;

                j = 0;

            }    

            

            // print out the angles collected if read 'E'

            else if (receive == 'E'){ 

                for(i=0; i<5; i++){

                    sprintf(string[i], "%s", angle[i]);

                    UART_1_PutString(string[i]);

                    UART_1_PutChar('\n');

                    // control the first servo

                    compareVar = angleCalculator(angle[1]);

                    //PWM_1_WriteCompare(compareVar);

                    //compareVar = angleCalculator(angle[2]);

                    PWM_2_WriteCompare(compareVar);

                    PWM_3_WriteCompare(compareVar);

                    PWM_4_WriteCompare(compareVar);

                    PWM_5_WriteCompare(compareVar);

                    

 

                } 

                UART_1_GetChar();

                UART_1_ClearRxBuffer();

            }

            

            else if(receive == '\n'){

                angle[i][j] = '\0';

                i++;

                j = 0;

            }

            

            else{

                angle[i][j] = receive;

                j++;

            }

        }

    }

}