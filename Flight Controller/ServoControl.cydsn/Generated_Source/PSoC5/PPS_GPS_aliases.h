/*******************************************************************************
* File Name: PPS_GPS.h  
* Version 2.20
*
* Description:
*  This file contains the Alias definitions for Per-Pin APIs in cypins.h. 
*  Information on using these APIs can be found in the System Reference Guide.
*
* Note:
*
********************************************************************************
* Copyright 2008-2015, Cypress Semiconductor Corporation.  All rights reserved.
* You may use this file only in accordance with the license, terms, conditions, 
* disclaimers, and limitations in the end user license agreement accompanying 
* the software package with which this file was provided.
*******************************************************************************/

#if !defined(CY_PINS_PPS_GPS_ALIASES_H) /* Pins PPS_GPS_ALIASES_H */
#define CY_PINS_PPS_GPS_ALIASES_H

#include "cytypes.h"
#include "cyfitter.h"


/***************************************
*              Constants        
***************************************/
#define PPS_GPS_0			(PPS_GPS__0__PC)
#define PPS_GPS_0_INTR	((uint16)((uint16)0x0001u << PPS_GPS__0__SHIFT))

#define PPS_GPS_INTR_ALL	 ((uint16)(PPS_GPS_0_INTR))

#endif /* End Pins PPS_GPS_ALIASES_H */


/* [] END OF FILE */
