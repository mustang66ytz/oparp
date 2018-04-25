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

int compareVar1 = 1000;
int previousVar1 = 1000;
int compareVar2 = 1000;
int previousVar2 = 1000; 
int compareVar3 = 1000;
int previousVar3 = 1000;

CY_ISR(ISR_Timer_Interrupt){
 
    if(compareVar1>previousVar1){
        for(int i=previousVar1; i<compareVar1; i+=1){
            PWM_1_WriteCompare(i);
            CyDelay(4);
        }
    }
    if(compareVar2>previousVar2){
        for(int i=previousVar2; i<compareVar2; i+=1){
            PWM_2_WriteCompare(i);
            CyDelay(4);
        }
    }
    if(compareVar2<=previousVar2){
        for(int i=previousVar2; i>compareVar2; i-=1){
            PWM_2_WriteCompare(i);
            CyDelay(4);
        }
    }
    if(compareVar1<=previousVar1){
        for(int i=previousVar1; i>compareVar1; i-=1){
            PWM_1_WriteCompare(i);
            CyDelay(4);
        }
    }
    
    if(compareVar3>previousVar3){
        for(int i=previousVar3; i<compareVar3; i+=1){
            PWM_3_WriteCompare(i);
            PWM_4_WriteCompare(i+50);
            CyDelay(4);
        }
    }
    if(compareVar3<=previousVar3){
        for(int i=previousVar3; i>compareVar3; i-=1){
            PWM_3_WriteCompare(i);
            PWM_4_WriteCompare(i+50);
            CyDelay(4);
        }
    }
 
    previousVar1 = compareVar1;
    previousVar2 = compareVar2;
    previousVar3 = compareVar3;  
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
                    compareVar1 = angleCalculator(angle[1]);
                    //PWM_1_WriteCompare(compareVar);
                    
                    // control the second servo
                    compareVar2 = angleCalculator(angle[2]);
                    //PWM_2_WriteCompare(compareVar1);
                    // control the third and fourth servo
                    compareVar3 = angleCalculator(angle[3]);
                    //PWM_3_WriteCompare(compareVar1);
                    //PWM_4_WriteCompare(compareVar1);
                    PWM_5_WriteCompare(compareVar1);
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