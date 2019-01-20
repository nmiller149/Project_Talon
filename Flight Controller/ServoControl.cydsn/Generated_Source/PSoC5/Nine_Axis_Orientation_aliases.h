/*******************************************************************************
* File Name: Nine_Axis_Orientation.h  
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

#if !defined(CY_PINS_Nine_Axis_Orientation_ALIASES_H) /* Pins Nine_Axis_Orientation_ALIASES_H */
#define CY_PINS_Nine_Axis_Orientation_ALIASES_H

#include "cytypes.h"
#include "cyfitter.h"


/***************************************
*              Constants        
***************************************/
#define Nine_Axis_Orientation_0			(Nine_Axis_Orientation__0__PC)
#define Nine_Axis_Orientation_0_INTR	((uint16)((uint16)0x0001u << Nine_Axis_Orientation__0__SHIFT))

#define Nine_Axis_Orientation_1			(Nine_Axis_Orientation__1__PC)
#define Nine_Axis_Orientation_1_INTR	((uint16)((uint16)0x0001u << Nine_Axis_Orientation__1__SHIFT))

#define Nine_Axis_Orientation_INTR_ALL	 ((uint16)(Nine_Axis_Orientation_0_INTR| Nine_Axis_Orientation_1_INTR))
#define Nine_Axis_Orientation_scl			(Nine_Axis_Orientation__scl__PC)
#define Nine_Axis_Orientation_scl_INTR	((uint16)((uint16)0x0001u << Nine_Axis_Orientation__0__SHIFT))

#define Nine_Axis_Orientation_sda			(Nine_Axis_Orientation__sda__PC)
#define Nine_Axis_Orientation_sda_INTR	((uint16)((uint16)0x0001u << Nine_Axis_Orientation__1__SHIFT))

#endif /* End Pins Nine_Axis_Orientation_ALIASES_H */


/* [] END OF FILE */
