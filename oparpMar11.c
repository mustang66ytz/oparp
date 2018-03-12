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

// this function converts the string to the angle value
int angleCalculator(uint8 *angle){
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
    PWM_6_Start();
    PWM_7_Start();
    
    // variables initialization
    uint8 receive;
    uint8 angle[7][10];
    uint8 string[7][10];
    uint8 stringtest[10];
    
    int i=0;
    int j=0;
    int compareVar = 1000;
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
                for(i=0; i<7; i++){
                    sprintf(string[i], "%s", angle[i]);
                    UART_1_PutString(string[i]);
                    UART_1_PutChar('\n');
                    
                    // control the first servo
                    
                    compareVar = angleCalculator(angle[1]);
                    PWM_1_WriteCompare(compareVar);
                        //CyDelay(1000);

                    //compareVar = angleCalculator(angle[2]);
                    PWM_2_WriteCompare(compareVar);
                    PWM_3_WriteCompare(compareVar);
                    PWM_4_WriteCompare(compareVar);
                    PWM_5_WriteCompare(compareVar);
                    PWM_6_WriteCompare(compareVar);
                    PWM_7_WriteCompare(compareVar);
                    
 
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
    
    
//    int i=0;
//    for(;;){
//        receive = UART_1_GetChar();
//        
//        
//        if(receive && receive!='\n'){
//            
//            if(receive == 'B'){
//                i = 0;
//            }
//            else if(receive == 'E'){
//                angle[i] = '\0';
//                sprintf(string, "%s", angle);
//                UART_1_PutString("Finished concatenation, the string is: \n");
//                
//                //for(int j=0; j<i; j++){
//                //    UART_1_PutChar(angle[j]);
//                //}
//                //UART_1_PutChar('\n');
//                
//                UART_1_PutString(string);
//                UART_1_PutChar('\n');
//                
//            }
//            else{
//                angle[i] = receive;
//                //UART_1_PutChar(angle[i]);
//                i++;
//            }
//            
//  
//        }
//    }
//}

//void strInput(uint8 str[], int nchars);
//
//void strInput(uint8 str[], int nchars) {
//    int i = 0;
//    uint8 ch;
//    
//    while((ch = UART_1_GetChar()) != '\n' && ch) {
//        UART_1_PutString("Inside the call function\n");
//        if (i < nchars) {
//            str[i++] = ch;
//       //   sprintf(string, "%d", str);
//            UART_1_PutArray(str, nchars);
//    }
//  }
//  //str[i] = '\0';
//  //UART_1_PutChar('\n');
//}
//
//int main(void) {
//    CyGlobalIntEnable; /* Enable global interrupts. */
//    UART_1_Start();
//    
//    int rowchars = 10;
//    int colchars = 30;
//    uint8 str[rowchars][colchars];
//    //uint8 str1[3] = {'a', 'd', 's'};
//
//    for(;;){
//        //UART_1_PutString("Inside outer loop\n");
//        //UART_1_PutArray(str1,3);
//        for(int i=0; i<rowchars; i++){
//            UART_1_PutString("constucting the string\n");
//            strInput(str[i], colchars);
//        }
//        for(int i=0; i<rowchars; i++){
//            //UART_1_PutString("display the string\n");
//            //UART_1_PutArray(str[i], colchars);
//            //UART_1_PutChar('\n');
//        }
//        
//    }
//    
//    return 0;
//}


//8pm 3/4/2018
//int main(void)
//{
//    CyGlobalIntEnable; /* Enable global interrupts. */
//    UART_1_Start();
//    //UART_1_PutString("Hello World");
//    
//    //char8 rxData1;
//    char string[20];
//    char rxData2;
//    /* The flowing code display the character get from the bus*/
//    for(;;)
//    {
//        //rxData1 = UART_1_GetChar();
//        rxData2 = UART_1_GetChar();
//        //UART_1_PutString("Hello World");
//        //UART_1_PutChar('\n');
//        //CyDelay(50);
//        //sprintf(string, "%d", rxData1);
//        //UART_1_PutString(string);
//        //UART_1_PutString("Not receiving data");        //UART_1_PutChar('\n');
//        if(rxData2){
//            UART_1_PutChar(rxData2);
//            //UART_1_PutString("Received data char: ");
//            //sprintf(string, "%c\n", rxData2);
//            //UART_1_PutString(string);
//            //CyDelay(2000);           
//        }       
//    }
//}

/* [] END OF FILE */
