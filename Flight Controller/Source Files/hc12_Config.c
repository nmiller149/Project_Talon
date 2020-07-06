/* ========================================
Last edited 01/12/2020
 * "hc12_Config.c"
 *
 * Author:  Nathan Miller
 *
 * Version: 1.4
 *
 * Description:
 * SOURCE CODE - Library for HC-12 Configuration Functions and Routines. Handles 
 *               communication directly to the module.
 *
 * Reasons for Revision:
 *    - HC_Test and configuration functions were interfering with the workings of the
 *      ISR built communication protocol. Any communication through HC12 is done in
 *      hc12_comm and communication done to the HC12 module is dealt with here. When 
 *      communicating to HC12, the ISR is disabled and re-enabled after configuration.
 *
 *    - Set_Pin changed to HC12_SET
 *
 *    - sendString_Clear() remoced and sendString removed from header and
 *      replaced as a local variable so that clear is not necessary
 *
 *    - HC_readConfig fixed. Also, CommandMode_Enable/Disable added UART_Clearbuffers 
 *      so to fix command acknowledge read in problems
 *
 *    - UART_BaudRate() Simplified
 *    - [1.4] Removed initiliazation of HC12_Comm () functionality. These files are to be independent.
 * 
 * TODOS:
 *    - Optimize and clean up code
 *      *turn readConfig into setConfig where the string is passed to it to send (unless that is a pain in the ass with pointers)
 *      *look into condensing the block of code at the end of the functions which returns true 
 *        **see if strcpy can be cutout with strcmp
 *      *make each return actually return true/false properly
 *      
 *    - Clean up comments and prettify the code
 *
 *    - Change FindBaud() to have a baudrate argument be passed to start with (then rotate through ea possibility)
 *
 ========================================== */

/* ========================================
Source file for HC12 Tranceiver Configuration Functions.

The purpose of this file is to have simple and easy to use functions, for the use of the
HC-12 radio transceiver module, that only require minimum reference to the header file 
rather than digging through data sheets.

This file takes care of configuring the communication between UART to external module as well as 
configuration of the different modes for the tranceiver module. Any functions used for sending data
between modules or finding outside modules must see "hc12_comm" .c and .h files.

Note: This library is still under development and is subject to change.

Property of Nathan M Miller.
 * ======================================== */


#include "hc12_Config.h"

//Internal Constants
#define NULL_STRING_10 "\0\0\0\0\0\0\0\0\0\0"

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
// DEFINITIONS                                                                //
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////


//uint32 Data;



//-------------------------------------------------------------------------------------------------------------------
//Internal Functions

char* HC_readConfig()
{   
    //If problems occur, it is likely due to the rx buffer being only 4 bytes. Will need to reconfigure
    //reading during cross communication and communicating directly to module. Two methods will need to merge
    char temp;
    uint16 startTime, period, index = 0;
    int16 durationTime;
    static char configString[10];
    strncpy(configString,"\0\0\0\0\0\0\0\0\0\0",10);
    
    period=Timer_1_ReadPeriod();
    while(index < 10)
    {
        /*while( UART_HC12_GetRxBufferSize() ==0u && timer <= 200000)
        {
            timer++;
        }
        if (timer >= 200000 )
            break;
        */
        durationTime=0;
        startTime = Timer_1_ReadCounter();
        while( UART_HC12_GetRxBufferSize() ==0u && (durationTime <= 1000) )
        {
            durationTime = startTime - Timer_1_ReadCounter();
            if(durationTime <1) 
                durationTime+=period;
        }
        if (durationTime >= 1000 )
            break;//return NULL;

        temp = UART_HC12_GetChar();
        //strcat(configString,&temp);
               
        if (temp != '\0')
        {
            configString[index] = temp;
            index++; 
        }
    }
    Timer_1_WriteCounter(0xFFFF);
    return configString;        
}




void Module_Communication_Error(){
    //For development and debug only
    //Once in this state, it will be held here until reset or debug
    
    uint8 count = 0;
    while(count <= 15){
        LED_Write(1u);
        CyDelay(200);
        LED_Write(0u);
        CyDelay(200);     
    }

}






void UART_Baud(uint32 BaudRate){
  //With external clock going to UART called "UART_HC12_CLOCK" set at 24MHz
  //Equation to find clock divider :
       //   (Clock_Freq / Bits / BaudRate) = clock divider
       // EX:  24,000,000/8/9600 = 312.5

    while(UART_HC12_GetTxBufferSize() >0){}
    UART_HC12_CLOCK_SetDivider(BCLK__BUS_CLK__HZ/(BaudRate * 8)); 
}


void HC_CommandMode_Enable()
{
    isr_HC12_Rx_Disable();
    UART_HC12_ClearTxBuffer();
    UART_HC12_ClearRxBuffer();   
    HC12_SET_Write(0);
    CyDelay(Mode_Delay);     
}

void HC_CommandMode_Disable()
{    
    CyDelay(Mode_Delay*2);
    HC12_SET_Write(1);
    CyDelay(Mode_Delay);
    UART_HC12_ClearTxBuffer();
    UART_HC12_ClearRxBuffer();
    isr_HC12_Rx_Enable();    
}

//---------------------------------------------------------------------------------------------
//External Functions


//uint8 Set_Config(char String[]){
    //Pass a string to this function to send to the HC12 module for configuration
    //Check that the configuration sent without error
    //Only checks the communication between the UART and the HC12 module
    //Return 1 if no error occured
    

	/* last edit: replaced String with sendString because the global array works, while the one being
	passed to the function does not. Cannot pass arrays to functions. To save time and effort this 
	funtion	is being removed from all other funtions and being commented out. Once a funtion is needed/
	wanted to check for a good communication instance. Note: still stuck in sending "AT+C" before 
	being stuck in some sort of loop. This was solved earlier, outside of using this function (for
	debuging), by changing "UART_HC12_PutString" to "-PutArray". This did not solve the problem in
	this case.
	
	NOTE: This function is being removed to a .txt file and placed inside the latest HC_12 development
	psoc file as of 11/20/2017. The rest of the content will be inside of that file.
	
	The file using this function will also be archived
	*/





void HC_Start(){
    //Starts the UART used for communicating with the HC-12 module
    //Put this in the main function and only run it once
    
    Timer_1_Start();    
    UART_HC12_Start();
    HC12_SET_Write(1u);
    CyDelay(500);
    UART_HC12_ClearTxBuffer();
    UART_HC12_ClearRxBuffer();
    
    return;
}

    






uint8 HC_Test(){
    //Use to test for good communication with the HC-12 module.
    //RETURN 1 if good test or 0 if not communicating
    
    //uint8 OK=0;

    HC_CommandMode_Enable();  //When sending configuration commands to the module, the set pin must be low

    UART_HC12_ClearTxBuffer();
    UART_HC12_ClearRxBuffer();
    UART_HC12_PutString("AT");
    
    char* temp = HC_readConfig();
    
    HC_CommandMode_Disable();

    //char testString[10] = "";
    //strncpy(testString,temp,10);  
    if (strncmp(temp, "OK",2) == 0)
    {
    return 1;
    }
    else return 0;
    }












void HC_Baud(uint32 BaudRate){   // 1200, 2400, 4800, 9600, 19200, 38400, 57600, 115200
    //uint8  OK=0;
    char sendString[10]= NULL_STRING_10;

    HC_CommandMode_Enable();  //Pin must be low for configuration mode

    switch (BaudRate){
        case 1200:
           strcpy(sendString,"AT+B1200\0");
           break;

        case 2400:
           strcpy(sendString,"AT+B2400\0");
           break;

        case 4800:
           strcpy(sendString,"AT+B4800\0");
           break;

        case 9600:
           strcpy(sendString,"AT+B9600\0");
           break;

        case 19200:
           strcpy(sendString,"AT+B19200\0");
           break;

        case 38400:
           strcpy(sendString,"AT+B38400\0");
           break;

        case 57600:
           strcpy(sendString,"AT+B57600\0");
           break;

        case 115200:
           strcpy(sendString,"AT+B115200\0");
           break;

        default:
           Module_Communication_Error();

    }//end switch

    UART_HC12_PutString(sendString);
	char* temp = HC_readConfig();
    
    HC_CommandMode_Disable();

    char testString[10] = "";
    strncpy(testString,temp,10);       

    
    UART_Baud(BaudRate);

    return; //return !strcmp(testString, "OK+");
}







void HC_Channel(uint8 Channel){   //001 thru 127

    char sendString[10]= NULL_STRING_10;
    
    HC_CommandMode_Enable();
    //ToDo: simplify library with the following function
    //sprintf(sendString,"AT+C00%u", Channel);

    //Convert the channel number to characters and prepare sendString to be sent to the UART
    if (Channel < 10 && Channel >0){
		strcpy(sendString,"AT+C00");
        sendString[6] = Channel + '0';
		sendString[7] = '\0';
	    sendString[8] = '\0';
		sendString[9] = '\0';

    }

    //Same thing but for channels from 10 to 99
    else if (Channel <100){
		strcpy(sendString,"AT+C0");
        sendString[5]= (Channel/10) + '0';  //converts the channel integer value to the ascii character val
        sendString[6]= (Channel%10) + '0';
		sendString[7]= '\0';
		sendString[8] = '\0';
    }

    //and one more time but for channels from 100 to 127
    else if (Channel <127){    //Same thing as else if statement above just get subtract 100 and put it in the initial copy
        Channel -=100;
		strcpy(sendString,"AT+C1");
        sendString[5] = (Channel/10) + '0';
        sendString[6] = (Channel%10) + '0';
		sendString[7] = '\0';
    }

    
	//UART_HC12_PutArray(sendString,strlen(sendString));
	UART_HC12_PutString(sendString);
    char* temp = HC_readConfig();
    
    HC_CommandMode_Disable();

    char testString[10] = "";
    strncpy(testString,temp,10);       

    return; //return !strcmp(testString, "OK+");
}







void HC_Function(uint8 FUx){   //FU1 --> FU4
    //FU3 is default


    HC_CommandMode_Enable();

    switch (FUx){
        case 1:  //FU1 : Power Saving Mode (can use any of 8 baud rates)
           UART_HC12_PutString("AT+FU1\0");
           break;

        case 2:  //FU2 : Extreme Power Save Mode (can only use 1200, 2400, & 4800 baud. Limited 
                 //to 1 second between packets)
           UART_HC12_PutString("AT+FU2\0");
           break;

        case 3:  //FU3 : Normal Operation. Can switch to any power or baud rate
           UART_HC12_PutString("AT+FU3\0");
           HC_Baud(9600);
           //UART_Baud(9600);
           break;

        case 4:  //FU4 : Long Distance (1.8km) Limited to 1200bps (500bps in air) Packet size of 
                 // 60 bytes or less and atleast 2 seconds between packets
           UART_HC12_PutString("AT+FU4\0");
           UART_Baud(1200); 
           break;
        
        default:    //This is bad and should never ever happen
           Module_Communication_Error();
    }


    char* temp = HC_readConfig();
    
    HC_CommandMode_Disable();

    char testString[10] = "";
    strncpy(testString,temp,10);       

    return; //return !strcmp(testString, "OK+");

}






void HC_Power(uint8 Power){  //1 to 8
    //Change the output power of the module, 1 being lowest & 8 being the Highest
    //ONLY WORKS FOR FU3?, POSSIBLY TAILOR THE CODE TO CALL FU3 FUNCTION OR THROW ERROR CODE, IF THE MODULE IS NOT IN FU3

    //char sendString[10]= NULL_STRING_10;

    HC_CommandMode_Enable();
    
    switch (Power){
        case 1:  //Power = Lowest
           UART_HC12_PutString("AT+P1\0");
           break;

        case 2:
           UART_HC12_PutString("AT+P2\0");
           break;

        case 3:
           UART_HC12_PutString("AT+P3\0");
           break;

        case 4:
           UART_HC12_PutString("AT+P4\0");
           break;

        case 5:  //Power = Mediumest
           UART_HC12_PutString("AT+P5\0");
           break;

        case 6:
           UART_HC12_PutString("AT+P6\0");
           break;

        case 7:
           UART_HC12_PutString("AT+P7\0");
           break;

        case 8:  //Power = Highest
           UART_HC12_PutString("AT+P8\0");
           break;

        default:
           UART_HC12_PutString("AT+P8\0");
    }
    
    char* temp = HC_readConfig();
    
    HC_CommandMode_Disable();

    char testString[10] = "";
    strncpy(testString,temp,10);       

    return; //return !strcmp(testString, "OK+");
}







void HC_SetData(uint32 d, char p, uint8 s){  // d = # Data Bits    p = parity bit (E=even, O=odd, N=none) //s= stop bits (1=1bit 2=2bits 3=1.5bits)
    //LOOK INTO DATA BIT PARAMETERS/LIMITS
    //NEED TO CONFIGURE UART BLOCK AS WELL TO MAKE THIS FUNCTION WORK
    /*NOT WORKING PROPERLY AS OF 11/19
    
    */
   
    char sendString[10]= NULL_STRING_10;
    char  charD2='0',charD1,charS;


    //Set module ready for command mode
    HC_CommandMode_Enable();


    //convert d to a character (or 2)
    if (d<10 && d >0){  //d>8???
        charD1 = d + '0';
    }
    else if (d <=64){
        charD2 = (d /10) + '0';
        charD1 = (d%10) + '0';
    }
    //if not a valid number, set to default.
    else
       charD1 ='8';



    //convert s to a character
    if (s >0 && s<=3){
        charS = s + '0';
    }
    else
      charS = '1';   //if s is not a valid number, then set it to the default


    //Prepare the sendString
    strcpy(sendString,"AT+U");

	uint8 i = 4;
    if (d>=10){
       //strcat(sendString,&charD2);  //only include the second digit if d is >=10
	   sendString[i]=charD2;
	   i++;
	}
	
	sendString[i+1] = charD1;
	sendString[i+2] = p;
	sendString[i+3] = charS;
    
	UART_HC12_PutString(sendString);    
    char* temp = HC_readConfig();
    
    HC_CommandMode_Disable();

    char testString[10] = "";
    strncpy(testString,temp,10);       

    return; //return !strcmp(testString, "OK+");
}







void HC_Sleep(){
    //DISABLE AND ENDABLE THE UART MODULE AS WELL
    
    HC_CommandMode_Enable();

    UART_HC12_PutString("AT+SLEEP");
    char* temp = HC_readConfig();
    
    HC_CommandMode_Disable();

    char testString[10] = "";
    strncpy(testString,temp,10);       

    return; //return !strcmp(testString, "OK+SLEEP");
}






void HC_Default(){
    //8 Bits, No parity, 1 stop bit, FU3, Pw = 8, baud = 9600

    HC_CommandMode_Enable();
    UART_HC12_PutString("AT+DEFAULT");
    char* temp = HC_readConfig();
    
    HC_CommandMode_Disable();

    char testString[10] = "";
    strncpy(testString,temp,10);       

    UART_Baud(9600);
    
    return; //return !strcmp(testString, "OK+SLEEP");  
}


void HC_FindBaud(){
    //Finds what baud rate the module is set at by running through each possibility and
    //sending a test command and looking for an "OK" string
    
    /*Instead of solving the desired way, this is a (hopefully) temporary fix. This method
    involves running through each HC_Baud possibility separetely and even landing on a desired
    baud rate. Eventually the module will catch one and will land will be able to follow the 
    psoc into the next tested baud rate until landing on the desired?*/
    
    uint8 i;
    uint32 BaudRate = 1200;
    
    for(i=0;i<8;++i){
        
        UART_Baud(BaudRate);   // 1200, 2400, 4800, 9600, 19200, 38400, 57600, 115200
        if (HC_Test()) break;
        CyDelay(200);
        if (BaudRate != 38400){
            BaudRate = BaudRate * 2;
        }
        else{
            BaudRate = BaudRate + (BaudRate/2);
        }
    }


}







/*
char HC_SCheck(char parameter){  //Single Check module parameter   B, C, F, P
    HC_CommandMode_Enable();
    CyDelay(Mode_Delay);

    switch (parameter){
        case 'B':  //Check the BaudRate
           strcpy(sendString,"AT+RB");
           break;

        case 'C':  //Channel
           strcpy(sendString,"AT+RC");
           break;

        case 'F':  //Function
           strcpy(sendString,"AT+RF");
           break;

        case 'P':  //Power
           strcpy(sendString,"AT+RP");
           break;

        default:
           strcpy(sendString,"AT");
    }

    UART_Tx_PutString(sendString);

    HC_CommandMode_Disable();





}


char HC_FCheck(){   //Can int work here?   returns a string     //Full Check of module parameters
    HC_CommandMode_Enable();
    CyDelay(Mode_Delay);
    UART_Tx_PutString("AT+Rx");

    HC12_SET_Write(1u);

    return;  //WORK ON RECEIVING END TO RETURN A USEFUL ANYHING
}
*/


//2)
//Write a function that Sends a check, reads the check, then determines if the receive should
//save that data, and if not then the transfer should RESEND

//1)
//Write a function that whenever the settings are being changed on the Transfer side, it will tell the
//receive side and it will follow suite
