/* =========================================
Last edited 04/03/2020
 * "interrupts.h"
 *
 * Author:  Nathan Miller
 *
 * Version: 1.0
 *
 * Description:
 * HEADER FILE - Contains interrupt handler for
 *
 * Reasons for Revision:
 *    -  
 *    - 
 *    -
 *
**/
 /* ======================================== */

#define DEFAULT_PRIORITY                    (3u)
#define HIGHER_PRIORITY                     (2u)

#include <project.h>

CY_ISR_PROTO(refreshHandler);
CY_ISR_PROTO(GPS_Handler);

_Bool refreshTimerFlag;
_Bool gpsRxFlag;

uint8 interrupts_init(void);

/* [] END OF FILE */
