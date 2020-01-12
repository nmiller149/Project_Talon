/* ========================================
Last edited 07/04/2019
 * "Flight_Controller.h"
 *
 * Author:  Nathan Miller
 *
 * Version: 2.1
 *
 * Description:
 * HEADER FILE - Contains all constants, functions and variables related to servo and esc control as 
 *             well as parsing flight data that is received or sent?
 *              THIS VERSION UTILIZES INTERRUPTS AND DOES NOT INCLUDE getFlightCommand Function
 *
 * Reasons for Revision:
 *    - Flipped the sign of _Roll, _Yaw, and _Flap in equations below and changed constant from 100 to 200
 *    - 
 *    -
 *
 *
**//* ======================================== */


#include "Flight_Controller.h"
    

uint8 FC_Start()
{    
    Elevator_Start();
    Aileron_Start();
    Motor_Speed_Start();
    return 0;
}


void executeFlightCommand(uint8 Command, uint8 Value) 
{
    //static uint8 _Roll, _Pitch, _Flap;
    //static int8 _Yaw, _TrimLA, _TrimRA, _TrimLE, _TrimRE;
    uint16 ElevL, ElevR, AilL, AilR;
    
    //Execute Command
    switch (Command)
    {    
        case FC_PITCH:
            FC._Pitch = Value; //Down=100, Neutral=50, Up=0
            break;

        case FC_ROLL:
            //FC._Roll = Value; //L=0, Neutral =50, R=100
            FC._Roll = Value/2 + 25; //Reduced control to half
            break;
       
        case FC_YAW:
            FC._Yaw = Value - 25; //Yaw => L=-25, R=25; Input=> L=0, R=50
            break;
       
        case FC_THROTTLE:
            FC._Throttle=Value;
            Motor_Speed_WriteCompare(Value+100);
            return;
            break;
            
        case FC_FLAPS:
            FC._Flap = Value;
            break;
       
        case FC_TRIM_AILERONS:
            FC._TrimLA = (Value>>4) & 0b00001111;
            FC._TrimRA = Value & 0b00001111;
            break;    
            
        case FC_TRIM_ELEVATORS:
            FC._TrimLE = (Value>>4) & 0b00001111;
            FC._TrimRE = Value & 0b00001111;
            break;
            
        case FC_MODE:
            //
            return;
            break;
            
        default:
            break;
     }
    ElevL = 200 - FC._Pitch + FC._Yaw;// - _TrimLE;
    ElevR = 100 + FC._Pitch + FC._Yaw;// + _TrimRE;
                  
    AilL =  200 - FC._Roll + FC._Flap;// - _TrimLA;
    AilR =  200 - FC._Roll - FC._Flap;// + _TrimRA;

    //Limit values from 1 to 2 ms;
    if(ElevL > ELEVATOR_MAX_VALUE) ElevL = ELEVATOR_MAX_VALUE;
    else if(ElevL < ELEVATOR_MIN_VALUE) ElevL = ELEVATOR_MIN_VALUE;
    if(ElevR > ELEVATOR_MAX_VALUE) ElevR = ELEVATOR_MAX_VALUE;
    else if(ElevR < ELEVATOR_MIN_VALUE) ElevR = ELEVATOR_MIN_VALUE;
    
    if(AilL > AILERON_MAX_VALUE) AilL = AILERON_MAX_VALUE;
    else if(AilL < AILERON_MIN_VALUE) AilL = AILERON_MIN_VALUE;
    if(AilR > AILERON_MAX_VALUE) AilR = AILERON_MAX_VALUE;
    else if(AilR < AILERON_MIN_VALUE) AilR = AILERON_MIN_VALUE;
    
    Elevator_WriteCompare1(ElevL); 
    Elevator_WriteCompare2(ElevR);
    Aileron_WriteCompare1(AilL);
    Aileron_WriteCompare2(AilR);
}



/* [] END OF FILE */