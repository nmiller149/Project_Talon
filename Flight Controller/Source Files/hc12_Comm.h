/* ========================================
Last edited 01/12/2020
 * "hc12_Comm.h"
 *
 * Author:  Nathan Miller
 *
 * Version: 0.3
 *
 * Description:
 * HEADER FILE - Contains all files for communicating between two HC12 devices. This version of the HC12 Communication
 *               layer is focuesd on combining all communication of and to the HC12 from all other files. 
 *
 *       TODOS:   Version 0.4 will also contain a routine for sending data directly upon receiving the data to prevent 
 *                packets from clashing (receive and send at the same time). This will require a master (either plane or 
 *                more likely the base station) to produce each message at a specific rate (5-10Hz) and the slave to send
 *                back at that rate while keeping a rhythm incase a message is missed. Master might continuously resend 
 *                last message or could send an acknowledge/synce message to fill required rate.
 *               
 * Reasons for Revision:
 *    - Previous Version 1.0 did not contain anything useful. This file is completely different minus the name.
 *    - [0.3] Includes an initialization function
 *    -
 *
**/
 /* ======================================== */


#ifndef _HC12_COMM_H
#define _HC12_COMM_H
    
#include <project.h>
//#include "hc12_Config.h"

//Constants
#define COMMAND_START_BYTE 0xFFE3 //ÿä  //0xFFC7
    
    
//---------- REMOVE -------------
enum HC12_ReceiveState
{
    Idle,
    Receiving,
    Collect,
    Sending,
    Error
}HC12_ReceiveState;

int HC_Send(uint32 Data);
void HC_Signal_Loss(); // A loss of communication function that sets default data config and high power everything and a specific channel to go to
//--------------------------------

CY_ISR_PROTO(FCisrHandler);

//Functions
void HC_Comm_Init();

//Variables
uint8 FC_RxString[2];
_Bool rxCommandFlag;


#endif
/* [] END OF FILE */
