/* ========================================
Last edited 07/04/2019
 * "Flight_Controller.h"
 *
 * Author:  Nathan Miller
 *
 * Version: 2.0
 *
 * Description:
 * HEADER FILE - Contains all constants, functions and variables related to servo and esc control as 
 *             well as parsing flight data that is received or sent? 
 *              THIS VERSION UTILIZES INTERRUPTS
 *
 * Reasons for Revision:
 *    -
 *    - 
 *    -
 *
 *
**//* ======================================== */


#ifndef _FLIGHT_CONTROLLER_H
#define _FLIGHT_CONTROLLER_H

#include <project.h>
//#include "hc12_Comm.h"

//Rx Controll Functions
#define FC_PITCH            1
#define FC_ROLL             2
#define FC_YAW              4
#define FC_THROTTLE         8
#define FC_FLAPS            16
#define FC_TRIM_AILERONS    32
#define FC_TRIM_ELEVATORS   64
#define FC_MODE             0b10000000
    
//Flight Modes
#define MANUAL_MODE         (0b01000000)
#define AUTO_STABILIZE_MODE (0b10000000)
#define AUTO_COURSE_MODE    (0b11000000)    

//Constants
#define ELEVATOR_MAX_VALUE  200
#define ELEVATOR_MIN_VALUE  100
#define AILERON_MAX_VALUE   200
#define AILERON_MIN_VALUE   100  


//Functions
uint8 FC_Start();
//uint8 FC_Initialize();
void executeFlightCommand(uint8 Command, uint8 Value);

struct Flight_Control
{
    uint8 _Roll, _Pitch, _Flap, _Throttle;
    int8 _Yaw, _TrimLA, _TrimRA, _TrimLE, _TrimRE;
    int8 _TrimRoll, _TrimPitch, _TrimYaw;

} FC;
    


#endif 
/* [] END OF FILE */

