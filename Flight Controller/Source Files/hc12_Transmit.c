/* ========================================
Header file for HC12 Tranceiver Functions.
Last edited 10/23/2017
 * ======================================== */


 #ifndef HC12_TRANSMIT_H
 #define HC12_TRANSMIT_H
 #include "hc12_Transmit.h"
 #endif


////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
// DEFINITIONS                                                                //
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////


int Data;
char sendString[10];




/*SEND AND RECEIVE NEED DIFFERENT MODES.
 1) Direct/Hard communication where send only sends and receive only receives

 2) Acknowledge mode: Hard send that expects an acknowledge to be received between
 every packet of data.
        -That acknowledge could be a byte that says the parity was good or just an "OK"
        -Could be an echo of that data

3) Bipolar Mode: Set a hard send and receive (polarity) and flip that with a packet or byte when needed
         - could just be a function inside of Direct Mode

4) Fiar-Shake, Uniform, Parallel, Proportionate mode: both ready for send or receive
         - Not for high speed communication
         -used to share info as it comes
         -Use of addresses

5) Multitask mode: -See if there is a way to both send and receive at the same time where the reveive has no
affect on the send and vice versa.

6) Make a loss of communication function that sets default data config and high power everything and a specific channel to go to


--MAKE A NEW HEADER AND SOURCE FILES FOR SEND AND RECEIVE
*/


//Send and receive functions





//IMPORTANTE :: !! :: IMPORTANT
//========= SUPER IMPORTANT ===========
// ADD FUNCTIONS TO EACH CONFIGURATION BLOCK THE TELLS THE RECEIVE END WHAT TO DO
//    -- Make these functions high priority and go through multiple error checks
//    -- especially for channel change.






//Functions built into the UART block that may be of use in the development of this library

//uint8 UART_GetChar(void)
//void UART_WriteTxData(uint8 txDataByte)
//void UART_SetRxAddress1(uint8 address)
//uint8 UART_ReadTxStatus(void)
//void UART_PutString(const char8 string[])
//void UART_PutArray(const uint8 string[], uint8/uint16 byteCount)
//void UART_Sleep(void)
//void UART_Wakeup(void)




//-------------------------------------------------------------------------------------------------------------------
//Internal Functions


void Module_Communication_Error(){
    //For development and debug only
    //Once in this state, it will be held here until reset or debug

    while(1){
        LED_Write(1u);
        CyDelay(500);
        LED_Write(0u);
        CyDelay(500);
    }

}






uint8 Set_Config(char String[]){
    //Pass a string to this function to send to the HC12 module for configuration
    //Check that the configuration sent without error
    //Return 1 if no error occured


    char rxCompString[15], txOutString[strlen(String)];
    uint8 i,OK=0;

    UART_Tx_ClearTxBuffer();  //ARE THESE NEEDED??
    UART_Tx_ClearRxBuffer(); //IS THIS EVEN A THING??

    for (i=0; i < Num_Test_Cycles; i++){

        //Output the string to the module
        UART_Tx_PutString(String);

        //reads in multiple bytes according to the size (number of elements) of the output
        for(i=0; i<strlen(String); i++){

           //wait until Rx Status states that there is data available
           while (UART_Tx_RX_STS_FIFO_NOTEMPTY != UART_Tx_ReadRxStatus()){}
           rxCompString[i] = UART_Tx_ReadRxData();

        }//end



        //If the first two characters are OK then continue to check
        if(strncmp(rxCompString,"OK",2)){

            //if the size of data is only 2 then it is just a test cycle and set ok = 1
            if (strlen(rxCompString) ==2)
                OK = 1;

            else{  //Compares only the second part of the strings (where the useful info is)
                for (i=0;i<=strlen(rxCompString)-2;i++){
                    txOutString[i] = String [i+2];
                    rxCompString[i] = rxCompString[i+2];
                }
                if (strcmp(rxCompString,txOutString)){
                    OK =1;
                }
                else
                    OK = 0;
            }

        }//end of string compare statement

        if (OK == 1){
          break;
        }

    }//end of retry loop

    return OK;    //GO THROUGH EACH FUNCTION AND MAKE IT CYCLE THROUGH UNTIL IT GETS THE OK
                   //ONLY MAKE IT GO THROUGH SO MANY TIMES BEFORE DISCARDING THE FUNCTION OR THROWING AN ERROR

}//end of Set_Config






//---------------------------------------------------------------------------------------------
//External Functions


int HC_Send(uint32 Data){


}


char HC_Receive(){


}



void HC_Test(){
    //Use to test for good communication with the HC-12 module.
    //CHANGE THIS FUNCTION LATER TO RETURN 1 OR 0 INSTEAD OF GETTING STUCK IN A LOOP

    uint8 i=0, OK=0;

    Set_Pin_Write(0u);  //When sending configuration commands to the module, the set pin must be low
    CyDelay(Mode_Delay);

    //Checks the communication and breaks if ok, else it goes into an error state for debug if check happens 3 or more times
        OK = Set_Config("AT");

        if (OK==0){
            Module_Communication_Error();
        }
    }


    return;
}





int HC_Send(uint32 Data){


    uint8 i = 0, OK=0;

    switch (Signal_Mode){
        case Direct_Communication:


           break;



        case Acknowledge_Mode:


            OK = Acknowledge();

            if (OK==0){
                  Module_Communication_Error();
            }


           break;


        case Bipolar_Mode:



           break;


        case Fair_Shake:



           break;


        case Multitask_Mode:



           break;


        default:

    }//end switch





    return;
}
