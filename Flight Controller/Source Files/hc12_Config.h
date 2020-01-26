/* ========================================
Last edited 06/07/2018
 * "hc12_Config.h"
 *
 * Author:  Nathan Miller
 *
 * Version: 1.3
 *
 * Description:
 * HEADER FILE - Library for HC-12 Configuration Functions and Routines. Handles 
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
 ========================================== */
/* ========================================
Header file for HC12 Tranceiver Configuration Functions.

This file takes care of configuring the communication between UART to external module as well as 
configuration of the different modes for the tranceiver module. Any functions used for sending data
between modules or finding outside modules must see "hc12_comm" .c and .h files.

Note: This library is still under development and is subject to change.

Property of Nathan M Miller.

Last edited 11/20/2017
 * ======================================== */

/*      void HC_Start();

        void HC_Test();                //Sends "AT+" to module and should receive "OK" back from it

        void HC_Baud(uint32 BaudRate); // 1200, 2400, 4800, 9600, 19200, 38400, 57600, 115200

        void HC_Channel(uint8 Channel);// 0 -125 (Pick 6)

        void HC_Function(uint8 FUx);    //FU1 : Power Saving Mode (can use any of 8 baud rates)
                                        //FU2 : Extreme Power Save Mode (can only use 1200, 2400, & 4800 baud. Limited to 1 second between packets)
                                        //FU3 : Normal Operation. Can switch to any power or baud rate            
                                        //FU4 : Long Distance (1.8km) Limited to 1200bps (500bps in air) Packet size of 
                                             // 60 bytes or less and atleast 2 seconds between packets
                                        
        void HC_Power(uint8 Power);     //1 (min) - 8 (max) - should only be used with FU3

        void HC_SetData(uint32 d, char p, uint8 s);
                                        // d idk if this can even go over 8 actually but i think the uart caps at 9 anyways
                                        // p = 'E' is even, 'O' is odd, 'N' is none
                                        // s = 1 means one stop bit, 2 means two stop bits, and 3 means 1.5 stop bits.

        void HC_Sleep();                //enable/disable sleep mode

        void HC_Default();              //
  */

//#include <stdint.h>   //WTF IS THIS FOR??

#ifndef _HC12_CONFIG_H
#define _HC12_CONFIG_H

#include <project.h>
//#include "hc12_Comm.h"

    
#define Mode_Delay  100        //The amount of time (ms) between swithcing the module into command mode and writing to it
#define Num_Test_Cycles 4     //The number of cycles that the Set Configuration command should be checking the Rx for correct communication before throwing the data away

    
//GlobalVariables.

//--- Configuration ---//
void HC_Start();
uint8 HC_Test();
void HC_Baud(uint32 BaudRate);
void HC_Channel(uint8 Channel);
void HC_Function(uint8 FUx);
void HC_Power(uint8 Power);
void HC_SetData(uint32 d, char p, uint8 s);
void HC_Sleep();
void HC_Default();

void HC_FindBaud();
char HC_SCheck(char parameter);  //Single Check
char HC_FCheck();   //Can int work here?   returns a string     //Full Check

uint32 Data;


#endif // _HC12_CONFIGURATION_H Define
/* [] END OF FILE */
