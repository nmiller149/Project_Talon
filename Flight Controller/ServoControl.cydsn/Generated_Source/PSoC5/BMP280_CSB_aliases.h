/*******************************************************************************
* File Name: BMP280_CSB.h  
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

#if !defined(CY_PINS_BMP280_CSB_ALIASES_H) /* Pins BMP280_CSB_ALIASES_H */
#define CY_PINS_BMP280_CSB_ALIASES_H

#include "cytypes.h"
#include "cyfitter.h"


/***************************************
*              Constants        
***************************************/
#define BMP280_CSB_0			(BMP280_CSB__0__PC)
#define BMP280_CSB_0_INTR	((uint16)((uint16)0x0001u << BMP280_CSB__0__SHIFT))

#define BMP280_CSB_INTR_ALL	 ((uint16)(BMP280_CSB_0_INTR))

#endif /* End Pins BMP280_CSB_ALIASES_H */


/* [] END OF FILE */
