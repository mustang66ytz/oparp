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

// variable declarations and initialization
int compareVar1 = 1000;
int previousVar1 = 1000;
int compareVar2 = 1000;
int previousVar2 = 1000; 
int compareVar3 = 1000;
int previousVar3 = 1000;
int compareVar4 = 1000;
int previousVar4 = 1000;
int compareVar5 = 1000;
int previousVar5 = 1000;

int i = 0;
int j = 0;

int tempRead = 0;
int distance = 0;

// arrays to store the incoming angle instructions
uint8 receive;
uint8 angle[6][10];
uint8 string[6][10];
uint8 stringtest[10];
char distString[16];

// this function converts the angle string to PWM instruction value
int angleCalculator(uint8 *angle){
    //return 408+10*(atoi((const char*)angle));
    return 408+10.1*(atoi((const char*)angle));

}    
    
// UART interrupt function
CY_ISR(ISR_UART_Interrupt){
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
            for(i=0; i<6; i++){
                sprintf(string[i], "%s", angle[i]);
                UART_1_PutString(string[i]);
                UART_1_PutChar('\n');
                
                sprintf(distString, "%d", distance);
                UART_1_PutString("hello");
                
                // assign the PWM values to all five joints
                compareVar1 = angleCalculator(angle[1]);
                compareVar2 = angleCalculator(angle[2]);
                compareVar3 = angleCalculator(angle[3]);
                compareVar4 = angleCalculator(angle[4]);
                compareVar5 = angleCalculator(angle[5]);
               
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

// watchdog interrupt to reset the psoc 
CY_ISR(ISR_WDT_Interrupt){
    CySoftwareReset();
}

// ultrasonic interrupt
//CY_ISR(ISR_Sensor_Interrupt){
    // at 1MHz Timer clock each count is 1 micro Second
    //Timer_2_SetInterruptMode(Timer_2_STATUS_CAPTURE_INT_MASK);
    //tempRead = Timer_2_ReadCounter();    
    
    // from HC 04 Ultra Sonic module's datasheet, each 58 uS = 1 cm
    //distance =i/58; 
//}

// timer interrupt for servo control, linear interpolation between the adjacent PWM signal instructions to
// smooth the servo motion
CY_ISR(ISR_Timer_Interrupt){
 
    if(compareVar1>previousVar1){
        previousVar1+=5;
        PWM_1_WriteCompare(previousVar1+65);
        //CyDelay(4);
    }
    
    if(compareVar2>previousVar2){
       previousVar2+=5;
       PWM_2_WriteCompare(previousVar2);
       //CyDelay(4);
    }
    
    if(compareVar2<=previousVar2){
        previousVar2-=5;
        PWM_2_WriteCompare(previousVar2);
        //CyDelay(4);
    }
    
    if(compareVar1<=previousVar1){
        previousVar1-=5;
        PWM_1_WriteCompare(previousVar1+65);
        //CyDelay(4);
    }
    
    if(compareVar3>previousVar3){
        previousVar3+=5;
        PWM_3_WriteCompare(previousVar3+250);
        PWM_4_WriteCompare(previousVar3+110);
        //CyDelay(4);
    }
    
    if(compareVar3<=previousVar3){
        previousVar3-=5;
        // software calibration for PWM3 and PWM4
        PWM_3_WriteCompare(previousVar3+250);
        PWM_4_WriteCompare(previousVar3+110);
        //CyDelay(4);
    } 
    
    if(compareVar4>previousVar4){
       previousVar4+=5;
       PWM_5_WriteCompare(previousVar4);
       //CyDelay(4);
    }
    
    if(compareVar4<=previousVar4){
        previousVar4-=5;
        PWM_5_WriteCompare(previousVar4);
        //CyDelay(4);
    }
    
    if(compareVar5>previousVar5){
       previousVar5+=5;
       PWM_6_WriteCompare(previousVar5);
       //CyDelay(4);
    }
    
    if(compareVar5<=previousVar5){
        previousVar5-=5;
        PWM_6_WriteCompare(previousVar5);
        //CyDelay(4);
    }
    Timer_1_ReadStatusRegister();

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
    PWM_6_Start();
   
    // start the firmware components
    Timer_1_Start();
    Timer_2_Start();
    Timer_Interrupt_StartEx(ISR_Timer_Interrupt);
    UART_Interrupt_StartEx(ISR_UART_Interrupt);
    WDT_Interrupt_StartEx(ISR_WDT_Interrupt);
    //Sensor_Interrupt_StartEx(ISR_Sensor_Interrupt);
    
    CyWdtStart(CYWDT_1024_TICKS, CYWDT_LPMODE_NOCHANGE); 
    // main for loop
    for(;;)

    {
        CyDelay(200);
        CyWdtClear(); // clear watchdog
        //Timer_2_EnableTrigger();
        //CyDelayUs(10);
        //Timer_2_DisableTrigger();
    }
}