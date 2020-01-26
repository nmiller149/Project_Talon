/* ========================================
Header file for HC12 Tranceiver Functions.
Last edited 10/23/2017
 * ======================================== */


#ifndef _HC12_TRANSMIT_H
#define _HC12_TRANSMIT_H

//Define some global variables used for setting the mode

// Send only sends and receive only receives
#define Direct_Communication  0

 //Hard send that expects an acknowledge to be received betweenevery packet of data
 #define Acknowledge_Mode 1
    //   -That acknowledge could be a byte that says the parity was good or just an "OK"
    //   -Could be an echo of that data

#define Bipolar_Mode  2   // Set a hard send and receive (polarity) and flip that with a packet or byte when needed
      //  - could just be a function inside of Direct Mode

#define Fair_Shake 3  // Uniform, Parallel, Proportionate mode: both ready for send or receive
        // - Not for high speed communication
        // -used to share info as it comes
        // -Use of addresses

#define Multitask_Mode 4 // See if there is a way to both send and receive at the same time where the reveive has no
  //affect on the send and vice versa.

uint8 Signal_Mode;




int HC_Send(uint32 Data);
char HC_Receive();

void HC_Signal_Loss();
// A loss of communication function that sets default data config and high power everything and a specific channel to go to




#endif // _HC12_CONFIGURATION_H Define
/* [] END OF FILE */
